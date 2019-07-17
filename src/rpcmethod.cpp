#include "rpcmethod.h"

RpcMethod::RpcMethod():
    name(""),
    usage(""),
    description(""),
    longDescription("")
{
    setMain([this](Json::Value& param){return this->main(param);});
}

RpcMethod::RpcMethod(std::string _name, std::string _usage, std::string _desc, std::string _longDesc):
    name(_name),
    usage(_usage),
    description(_desc),
    longDescription(_longDesc)
{
    setMain([this](Json::Value& param){return this->main(param);});
}

Json::Value RpcMethod::main(Json::Value& parameters) {
    return Json::Value(Json::objectValue);
}

void RpcMethod::setMain(std::function<Json::Value(Json::Value&)> func) {
    mainFunc = func;
}

void RpcMethod::setMain(Json::Value& (*func)(Json::Value&)) {
    mainFunc = [&](Json::Value& param){return func(param);};
}

std::string RpcMethod::getName() {
    return name;
}

std::string RpcMethod::getUsage() {
    return usage;
}

std::string RpcMethod::getDescription() {
    return description;
}

std::string RpcMethod::getLongDescription() {
    return longDescription;
}
