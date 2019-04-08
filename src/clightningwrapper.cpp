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

bool fundChannel(const std::string& id, const unsigned int& sats, const unsigned int& feerate,
            const bool& announce, const unsigned int& minconf)
{
    std::string command = "fundchannel";
    Json::Value params(Json::arrayValue);
    params.append(id);
    params.append(sats);
    if (feerate)
        params.append(feerate);
    if (!announce)
        params.append(announce);
    if (minconf > 1)
        params.append(minconf); // TODO: check if 2 parameters above were defined so that the user don't bother to set them
    Json::Value res = sendCommand(command, params);
    return !res["txid"].empty();
}

bool pay(const std::string& bolt11, const unsigned int& msats, const std::string& label,
            const unsigned int& riskfactor, const float& maxFeePercent, const unsigned int& retryFor, 
            const unsigned int& maxDelay, const unsigned int exemptFee)
{
    std::string command = "pay";
    Json::Value params(Json::arrayValue);
    params.append(bolt11);
    if (msats)
        params.append(msats);
    if (label != "")
        params.append(label);
    params.append(riskfactor);
    params.append(maxFeePercent);
    params.append(retryFor);
    params.append(maxDelay);
    params.append(exemptFee);
    Json::Value res = sendCommand(command, params);
    return res["code"].empty();
}

bool ping(const std::string& id, const unsigned int& len, const unsigned int& pongbytes)
{
    std::string command = "ping";
    Json::Value params(Json::arrayValue);
    params.append(id);
    params.append(len);
    params.append(pongbytes);
    Json::Value res = sendCommand(command, params);
    return !res["totlen"].empty();
}

bool stop()
{
    std::string command = "stop";
    Json::Value params(Json::arrayValue);
    Json::Value res = sendCommand(command, params);
    return true;
}

bool waitAnyInvoice(const unsigned int& lastpayIndex)
{
    std::string command = "waitanyinvoice";
    Json::Value params(Json::arrayValue);
    if (lastpayIndex)
        params.append(lastpayIndex);
    Json::Value res = sendCommand(command, params);
    return res["complete"] == "true";
}
   
bool waitInvoice(const std::string& label)
{
    std::string command = "waitinvoice";
    Json::Value params(Json::arrayValue);
    params.append(label);
    Json::Value res = sendCommand(command, params);
    return res["status"] == "paid";
}
    
bool withdraw(const std::string& address, const unsigned int& sats, const unsigned int& feerate=0,
            const unsigned int& minconf=1)
{
    std::string command = "withdraw";
    Json::Value params(Json::arrayValue);
    params.append(address);
    params.append(sats);
    if (feerate)
        params.append(feerate);
    if (minconf > 1)
        params.append(minconf); // TODO: check if feerate is defined so that the user don't bother to set them
    Json::Value res = sendCommand(command, params);
    return !res["txid"].empty();
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
