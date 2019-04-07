#include "clightningwrapper.h"

CLightningWrapper::CLightningWrapper(std::string& socket_path)
{
    socketClient = new jsonrpc::UnixDomainSocketClient(socket_path);
    client = new jsonrpc::Client(*socketClient, jsonrpc::JSONRPC_CLIENT_V1);   
}

CLightningWrapper::~CLightningWrapper()
{
    delete socketClient;
    delete client;
}

Json::Value CLightningWrapper::sendCommand(const std::string& command, const Json::Value& arguments)
{
    Json::Value res;
    //TODO: Handle exceptions
    res = client->CallMethod(command, arguments);
    return res;
}

bool CLightningWrapper::autoCleanInvoice(const unsigned int& cycleSeconds, const unsigned int& expiredBy)
{
    std::string command = "autocleaninvoice";
    Json::Value params(Json::arrayValue);
    params.append(cycleSeconds);
    params.append(expiredBy);
    Json::Value res = sendCommand(command, params);
    return res.empty();
}

bool CLightningWrapper::check(const std::string& commandToCheck)
{
    std::string command = "check";
    Json::Value params(Json::arrayValue);
    params.append(commandToCheck);
    Json::Value res = sendCommand(command, params);
    return res["command_to_check"] == commandToCheck.substr(0, commandToCheck.find(" ")); // The command can include multiple words
}

bool CLightningWrapper::connect(const std::string& host)
{
    std::string command = "connect";
    Json::Value params(Json::arrayValue);
    params.append(host);
    Json::Value res = sendCommand(command, params);
    return !res["id"].empty();
}

bool delInvoice(const std::string& label, const std::string& status)
{
    std::string command = "delinvoice";
    Json::Value params(Json::arrayValue);
    params.append(label);
    params.append(status);
    Json::Value res = sendCommand(command, params);
    return true; // TODO: A better error check
}

bool delExpiredInvoice(const unsigned int& maxExpiryTime)
{
    std::string command = "delexpiredinvoice";
    Json::Value params(Json::arrayValue);
    params.append(maxExpiryTime);
    Json::Value res = sendCommand(command, params);
    return res.empty();
}

bool delExpiredInvoices()
{
    std::string command = "delexpiredinvoice";
    Json::Value params(Json::arrayValue);
    Json::Value res = sendCommand(command, params);
    return res.empty();
}

bool disconnect(const std::string& id, const bool& force=false)
{
    std::string command = "disconnect";
    Json::Value params(Json::arrayValue);
    params.append(id);
    params.append(force);
    Json::Value res = sendCommand(command, params);
    return res.empty();
}

Json::Value CLightningWrapper::close(const std::string& id, const bool& force, const unsigned int& timeout)
{
    std::string command = "close";
    Json::Value params(Json::arrayValue);
    params.append(id);
    if (force)
    {
        params.append(force);
        params.append(timeout);
    }
    return sendCommand(command, params);
}
