#include "clightningplugin.h"

#include <chrono>
#include <iostream>
#include <thread>

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
        for (auto const &it : subscriptions)
            manifest["subscriptions"].append(it.first);

        return manifest;
    });
    return getManifest;
}

RpcMethod Plugin::generateInit()
{
    RpcMethod init("init", "", "", "");
    init.setMain([this](Json::Value &initObject) {
        Json::Value config = initObject["configuration"];
        if (!config)
            abort();
        std::string socketPath = config["lightning-dir"].asString() + "/" + config["rpc-file"].asString();
        this->rpc = new CLightningRpc(socketPath);
        // init return is currently discarded by lightningd
        return Json::Value(Json::objectValue);
    });
    return init;
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
        // If this is a notification
        if (!msg["id"]) {
            auto it = subscriptions.find(msg["method"].asString());
            if (it != subscriptions.end())
                it->second(msg["params"]);
            continue;
        }
        Json::Value method = msg["method"];
        if (!method)
            continue;
        for (unsigned int i = 0; i < rpcMethods.size(); i++) {
            if (rpcMethods[i].getName() == method.asString()) {
                Json::Value parameters = msg["params"];
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
