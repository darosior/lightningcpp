#include "clightningplugin.h"
#include "pluginexception.h"

#include <iostream>

Plugin::Plugin():
    rpc(nullptr),
    options(Json::Value(Json::arrayValue))
{}

Plugin::~Plugin()
{
    if (rpc)
        delete rpc;
}

void Plugin::addMethod(const RpcMethod &method)
{
    rpcMethods.push_back(method);
}

void Plugin::addOption(const Json::Value &option)
{
    options.append(option);
}

void Plugin::addOption(const std::string &name, const std::string &defaultValue,
                    const std::string &description, const std::string &type)
{
    Json::Value option(Json::objectValue);
    option["name"] = name;
    option["default"] = defaultValue;
    option["description"] = description;
    option["type"] = type;
    options.append(option);
}

void Plugin::printResponseSuccess(const Json::Value &result, const std::string &reqId)
{
    Json::FastWriter writer;
    Json::Value response;
    response["jsonrpc"] = "2.0";
    response["result"] = result;
    response["id"] = reqId;
    std::cout << writer.write(response);
}

RpcMethod Plugin::generateManifest()
{
    RpcMethod getManifest("getmanifest", "", "", "");
    getManifest.setMain([&](Json::Value &params) {
        Json::Value manifest(Json::objectValue);
        Json::Value methods(Json::arrayValue);

        manifest["options"] = options;
        for (unsigned int i = 0; i < rpcMethods.size(); i++) {
            if (rpcMethods[i].getName() == "init" || rpcMethods[i].getName() == "getmanifest")
                continue;
            Json::Value method(Json::objectValue);
            method["name"] = rpcMethods[i].getName();
            method["usage"] = rpcMethods[i].getUsage();
            method["description"] = rpcMethods[i].getDescription();
            method["long_description"] = rpcMethods[i].getLongDescription();
            methods.append(method);
        }
        manifest["rpcmethods"] = methods;
        manifest["subscriptions"] = Json::Value(Json::arrayValue);
        for (const auto &it : subscriptions)
            manifest["subscriptions"].append(it.first);
        for (const auto &it : hookSubscriptions)
            manifest["hooks"].append(it.first);
        
        return manifest;
    });
    return getManifest;
}

RpcMethod Plugin::generateInit()
{
    RpcMethod init("init", "", "", "");
    init.setMain([this](Json::Value &initParams) {
        Json::Value config = initParams["configuration"];
        if (!config)
            throw CLightningPluginException(1, "'init' JSONRPC request doesn't have a 'configuration' field.");
        std::string socketPath = config["lightning-dir"].asString() + "/" + config["rpc-file"].asString();
        rpc = new CLightningRpc(socketPath);
        // Add a field to each option json to store its value
        for (unsigned int i = 0; i < options.size(); i++)
            options[i]["value"] = initParams["options"][options[i]["name"].asString()];
        // init return is currently discarded by lightningd
        return Json::Value(Json::objectValue);
    });
    return init;
}

Json::Value Plugin::getOptionValue(const std::string &name)
{
    for (unsigned int i = 0; i < options.size(); i++)
        if (options[i]["name"].asString() == name)
            return options[i]["value"];
    return Json::Value(Json::objectValue);
}

void Plugin::hookSubscribe(const std::string name, std::function<Json::Value(Json::Value&)> handler)
{
    hookSubscriptions[name] = handler;
}

void Plugin::start()
{
    Json::Value msg;
    Json::Reader reader;
    std::string line;

    rpcMethods.push_back(generateManifest());
    rpcMethods.push_back(generateInit());

    while (getline(std::cin, line)) {
        if (!reader.parse(line, msg, false))
            continue;
        if (!msg["method"] || !msg["params"] || !msg["jsonrpc"])
            // Discard noise
            continue;
        Json::Value method = msg["method"];
        if (!method)
            continue;
        Json::Value params = msg["params"];
        // If this is a notification
        if (!msg["id"]) {
            auto it = subscriptions.find(method.asString());
            if (it != subscriptions.end())
                it->second(params);
            continue;
        }
        auto it = hookSubscriptions.find(method.asString());
        if (it != hookSubscriptions.end()) {
            printResponseSuccess(it->second(params), msg["id"].asString());
            continue;
        }
        for (unsigned int i = 0; i < rpcMethods.size(); i++) {
            if (rpcMethods[i].getName() == method.asString()) {
                Json::Value parameters = params;
                printResponseSuccess(rpcMethods[i].mainFunc(parameters), msg["id"].asString());
                break;
            }
        }
    }
}

void Plugin::subscribe(const std::string name, std::function<void(Json::Value&)> handler)
{
    // overrides if entry is already present
    subscriptions[name] = handler;
}
