#include "clightningwrapper.h"
#include "exception.h"
#include <iostream>
CLightningWrapper::CLightningWrapper(std::string& socket_path)
{
    socketClient = new jsonrpc::UnixDomainSocketClient(socket_path);
    client = new jsonrpc::Client(*socketClient, jsonrpc::JSONRPC_CLIENT_V2);
}

CLightningWrapper::~CLightningWrapper()
{
    delete socketClient;
    delete client;
}

Json::Value CLightningWrapper::sendCommand(const std::string& command, const Json::Value& arguments)
{
    Json::Value res;
    try {
        res = client->CallMethod(command, arguments);
    } catch (jsonrpc::JsonRpcException& e) {
        throw CLightningWrapperException(e.GetCode(), e.GetMessage());
    } catch (std::exception& e) {
        throw CLightningWrapperException(1, e.what());
    } catch (...) {
        throw CLightningWrapperException(1, std::string("C-Lightning wrapper crashed when sending : " + command));
    }
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
    try {
        Json::Value res = sendCommand(command, params);
        return res["command_to_check"].asString() == commandToCheck.substr(0, commandToCheck.find(" ")); // The command can include multiple words
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::connect(const std::string& host)
{
    std::string command = "connect";
    Json::Value params(Json::arrayValue);
    params.append(host);
    try {
        Json::Value res = sendCommand(command, params);
        return !res["id"].empty();
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::delInvoice(const std::string& label, const std::string& status)
{
    std::string command = "delinvoice";
    Json::Value params(Json::arrayValue);
    params.append(label);
    params.append(status);
    try {
        Json::Value res = sendCommand(command, params);
        return true;
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::delExpiredInvoice(const unsigned int& maxExpiryTime)
{
    std::string command = "delexpiredinvoice";
    Json::Value params(Json::arrayValue);
    params.append(maxExpiryTime);
    try {
        Json::Value res = sendCommand(command, params);
        return res.empty();
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::delExpiredInvoices()
{
    std::string command = "delexpiredinvoice";
    Json::Value params(Json::arrayValue);
    try {
        Json::Value res = sendCommand(command, params);
        return res.empty();
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::disconnect(const std::string& id, const bool& force)
{
    std::string command = "disconnect";
    Json::Value params(Json::arrayValue);
    params.append(id);
    params.append(force);
    try {
        Json::Value res = sendCommand(command, params);
        return res.empty();
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::pay(const std::string& bolt11, const unsigned int& msats, const std::string& label,
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
    try {
        Json::Value res = sendCommand(command, params);
        return res["code"].empty();
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::ping(const std::string& id, const unsigned int& len, const unsigned int& pongbytes)
{
    std::string command = "ping";
    Json::Value params(Json::arrayValue);
    params.append(id);
    params.append(len);
    params.append(pongbytes);
    try {
        Json::Value res = sendCommand(command, params);
        return !res["totlen"].empty();
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::stop()
{
    std::string command = "stop";
    Json::Value params(Json::arrayValue);
    try {
        Json::Value res = sendCommand(command, params);
        return res.asString() == "Shutting down";
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

bool CLightningWrapper::waitAnyInvoice(const unsigned int& lastpayIndex)
{
    std::string command = "waitanyinvoice";
    Json::Value params(Json::arrayValue);
    if (lastpayIndex)
        params.append(lastpayIndex);
    try {
        Json::Value res = sendCommand(command, params);
        return res["complete"].asString() == "true";
    } catch (const CLightningWrapperException&) {
        return false;
    }
}
   
bool CLightningWrapper::waitInvoice(const std::string& label)
{
    std::string command = "waitinvoice";
    Json::Value params(Json::arrayValue);
    params.append(label);
    try {
        Json::Value res = sendCommand(command, params);
        return res["status"].asString() == "paid";
    } catch (const CLightningWrapperException&) {
        return false;
    }
}

std::string CLightningWrapper::fundChannel(const std::string& id, const unsigned int& sats, const unsigned int& feerate,
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
    return res["txid"].asString();
}

std::string CLightningWrapper::withdraw(const std::string& address, const unsigned int& sats, const unsigned int& feerate,
        const unsigned int& minconf)
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
    return res["txid"].asString();
}

std::string CLightningWrapper::getConfig(const std::string& param)
{
    std::string command = "listconfigs";
    Json::Value params(Json::arrayValue);
    params.append(param);
    Json::Value res = sendCommand(command, params);
    return res[param.c_str()].asString();
}

std::string CLightningWrapper::genInvoice(const unsigned int& msat, const std::string& label, const std::string& description,
        const unsigned int& expiry, const std::vector<std::string>& fallbacks, const std::string& preimage)
{
    std::string command = "invoice";
    Json::Value params(Json::arrayValue);
    params.append(msat);
    params.append(label);
    params.append(description);
    params.append(expiry);
    if (!fallbacks.empty())
    {
        Json::Value addresses(Json::arrayValue);
        for(auto const& fallback: fallbacks) {
            addresses.append(fallback);
        }
        params.append(addresses);
    }
    if (preimage != "")
        params.append(preimage);
    Json::Value res = sendCommand(command, params);
    return res["bolt11"].asString();
}

std::string CLightningWrapper::newAddr(const std::string& type)
{
    std::string command = "newaddr";
    Json::Value params(Json::arrayValue);
    params.append(type);
    return sendCommand(command, params).asString();
}

Json::Value CLightningWrapper::close(const std::string& id, const bool& force, const unsigned int& timeout)
{
    std::string command = "close";
    Json::Value params(Json::arrayValue);
    params.append(force);
    if (force)
        params.append(timeout);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::decodepay(const std::string& bolt11)
{
    std::string command = "decodepay";
    Json::Value params(Json::arrayValue);
    params.append(bolt11);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::getFeerates(const std::string& style)
{
    std::string command = "feerates";
    Json::Value params(Json::arrayValue);
    params.append(style);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::getInfo()
{
    std::string command = "getinfo";
    Json::Value params(Json::arrayValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::getLog(const std::string& level)
{
    std::string command = "getlog";
    Json::Value params(Json::arrayValue);
    params.append(level);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::getRoute(const std::string& id, const unsigned int& msats, const unsigned int& riskfactor,
        const unsigned int& cltv, const std::string& fromid, const float& fuzzPercent,
        const std::vector<std::string>& exclude, const unsigned int& maxhops)
{
    std::string command = "getroute";
    Json::Value params(Json::arrayValue);
    params.append(id);
    params.append(msats);
    params.append(riskfactor);
    params.append(cltv);
    if (fromid != "")
        params.append(fromid);
    params.append(fuzzPercent);
    if (!exclude.empty())
    {
        Json::Value ids(Json::arrayValue);
        for (auto const& id: exclude) {
            ids.append(id);
        }
        params.append(ids);
    }
    params.append(maxhops);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::help()
{
    std::string command = "help";
    Json::Value params(Json::arrayValue);
    return sendCommand(command, params)["help"];
}

Json::Value CLightningWrapper::helpOn(const std::string& call)
{
    std::string command = "help";
    Json::Value params(Json::arrayValue);
    params.append(call);
    return sendCommand(command, params)["help"][0];
}

Json::Value CLightningWrapper::listChannels(const std::string& shortChannelId, const std::string& source)
{
    std::string command = "listchannels";
    Json::Value params(Json::arrayValue);
    if (shortChannelId != "")
        params.append(shortChannelId);
    if (source != "")
        params.append(source);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listConfigs()
{
    std::string command = "listconfigs";
    Json::Value params(Json::arrayValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listForwards()
{
    std::string command = "listforwards";
    Json::Value params(Json::arrayValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listFunds()
{
    std::string command = "listfunds";
    Json::Value params(Json::arrayValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listInvoices(const std::string& label)
{
    std::string command = "listinvoices";
    Json::Value params(Json::arrayValue);
    if (label != "")
        params.append(label);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listNodes()
{
    std::string command = "listnodes";
    Json::Value params(Json::arrayValue);
    return sendCommand(command, params)["nodes"];
}

Json::Value CLightningWrapper::listPay(const std::string& bolt11)
{
    std::string command = "listpays";
    Json::Value params(Json::arrayValue);
    if (bolt11 != "")
        params.append(bolt11);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listPayments(const std::string& bolt11, const std::string& payment_hash)
{
    std::string command = "listpayments";
    Json::Value params(Json::arrayValue);
    if (bolt11 != "")
        params.append(bolt11);
    if (payment_hash != "")
        params.append(payment_hash);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listPeers(const std::string& id, const std::string& level)
{
     std::string command = "listpeers";
    Json::Value params(Json::arrayValue);
    if (id != "")
        params.append(id);
    if (level != "")
        params.append(level);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listSendPays(const std::string& bolt11, const std::string& payment_hash)
{
    std::string command = "listsendpays";
    Json::Value params(Json::arrayValue);
    if (bolt11 != "")
        params.append(bolt11);
    if (payment_hash != "")
        params.append(payment_hash);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::payStatus(const std::string& bolt11)
{
    std::string command = "paystatus";
    Json::Value params(Json::arrayValue);
    if (bolt11 != "")
        params.append(bolt11);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::sendPay(const std::string& route, const std::string& payment_hash, const std::string& label,
        const unsigned int& msats, const std::string& bolt11)
{
    std::string command = "sendpay";
    Json::Value params(Json::arrayValue);
    params.append(route);
    params.append(payment_hash);
    if (label != "")
        params.append(label);
    if (msats)
        params.append(msats);
    if (bolt11 != "")
        params.append(bolt11);
    return sendCommand(command, params);
}
