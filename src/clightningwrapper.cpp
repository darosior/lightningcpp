#include "clightningwrapper.h"
#include "exception.h"

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

Json::Value CLightningWrapper::autoCleanInvoice(const unsigned int& cycleSeconds, const unsigned int& expiredBy)
{
    std::string command = "autocleaninvoice";
    Json::Value params(Json::objectValue);
    params["cycle_seconds"] = cycleSeconds;
    params["expired_by"] = expiredBy;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::check(const std::string& commandToCheck, const Json::Value& parameters)
{
    std::string command = "check";
    Json::Value params(Json::objectValue);
    params["command_to_check"] = commandToCheck;
    if (!parameters.empty())
        params.append(parameters);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::close(const std::string& id, const bool& force, const unsigned int& timeout)
{
    std::string command = "close";
    Json::Value params(Json::objectValue);
    params["id"] = id;
    params["force"] = force;
    params["timeout"] = timeout;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::connect(const std::string& id, const std::string& host, const unsigned int& port)
{
    std::string command = "connect";
    Json::Value params(Json::objectValue);
    params["id"] = id;
    if (host != "") {
        params["host"] = host;
        params["port"] = port;
    }
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::decodepay(const std::string& bolt11, const std::string& description)
{
    std::string command = "decodepay";
    Json::Value params(Json::objectValue);
    params["bolt11"] = bolt11;
    if (description != "")
        params["description"] = description;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::delExpiredInvoices(const unsigned int& maxExpiryTime)
{
    std::string command = "delexpiredinvoice";
    Json::Value params(Json::objectValue);
    params["maxexpirytime"] = maxExpiryTime;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::delInvoice(const std::string& label, const std::string& status)
{
    std::string command = "delinvoice";
    Json::Value params(Json::objectValue);
    params["label"] = label;
    params["status"] = status;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::disconnect(const std::string& id, const bool& force)
{
    std::string command = "disconnect";
    Json::Value params(Json::objectValue);
    params["id"] = id;
    params["force"] = force;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::feerates(const std::string& style)
{
    std::string command = "feerates";
    Json::Value params(Json::objectValue);
    params["style"] = style;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::fundChannel(const std::string& id, const unsigned int& sats, const unsigned int& feerate,
        const bool& announce, const unsigned int& minconf)
{
    std::string command = "fundchannel";
    Json::Value params(Json::objectValue);
    params["id"] = id;
    params["satoshi"] = sats;
    if (feerate)
        params["feerate"] = feerate;
    params["announce"] = announce;
    params["minconf"] = minconf;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::getInfo()
{
    std::string command = "getinfo";
    Json::Value params(Json::objectValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::getLog(const std::string& level)
{
    std::string command = "getlog";
    Json::Value params(Json::objectValue);
    params["level"] = level;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::getRoute(const std::string& id, const unsigned int& msats, const unsigned int& riskfactor,
        const unsigned int& cltv, const std::string& fromId, const float& fuzzPercent, const Json::Value& exclude,
        const unsigned int& maxhops)
{
    std::string command = "getroute";
    Json::Value params(Json::objectValue);
    params["id"] = id;
    params["msatoshi"] = msats;
    params["riskfactor"] = riskfactor;
    params["cltv"] = cltv;
    if (fromId != "")
        params["fromid"] = fromId;
    params["fuzzpercent"] = fuzzPercent;
    if (!exclude.empty())
        params["exclude"] = exclude;
    params["maxhops"] = maxhops;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::help(const std::string& cmd)
{
    std::string command = "help";
    Json::Value params(Json::objectValue);
    if (command != "")
        params["command"] = cmd;
    return sendCommand(command, params)["help"];
}

Json::Value CLightningWrapper::invoice(const unsigned int& msat, const std::string& label, const std::string& description,
        const unsigned int& expiry, const Json::Value& fallbacks, const std::string& preimage)
{
    std::string command = "invoice";
    Json::Value params(Json::objectValue);
    params["msatoshi"] = msat;
    params["label"] = label;
    params["description"] = description;
    params["expiry"] = expiry;
    if (!fallbacks.empty())
        params["fallbacks"] = fallbacks;
    if (preimage != "")
        params["preimage"] = preimage;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listChannels(const std::string& scid, const std::string& source)
{
    std::string command = "listchannels";
    Json::Value params(Json::objectValue);
    if (scid != "")
        params["short_channel_id"] = scid;
    if (source != "")
        params["source"] = source;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listConfigs(const std::string& param)
{
    std::string command = "listconfigs";
    Json::Value params(Json::objectValue);
    if (param != "")
        params["config"] = param;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listForwards()
{
    std::string command = "listforwards";
    Json::Value params(Json::objectValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listFunds()
{
    std::string command = "listfunds";
    Json::Value params(Json::objectValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listInvoices(const std::string& label)
{
    std::string command = "listinvoices";
    Json::Value params(Json::objectValue);
    if (label != "")
        params["label"] = label;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listNodes()
{
    std::string command = "listnodes";
    Json::Value params(Json::objectValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listPays(const std::string& bolt11)
{
    std::string command = "listpays";
    Json::Value params(Json::objectValue);
    if (bolt11 != "")
        params["bolt11"] = bolt11;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listPayments(const std::string& bolt11, const std::string& payment_hash)
{
    std::string command = "listpayments";
    Json::Value params(Json::objectValue);
    if (bolt11 != "")
        params["bolt11"] = bolt11;
    if (payment_hash != "")
        params["payment_hash"] = payment_hash;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listPeers(const std::string& id, const std::string& level)
{
    std::string command = "listpeers";
    Json::Value params(Json::objectValue);
    if (id != "")
        params["id"] = id;
    if (level != "")
        params["level"] = level;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::listSendPays(const std::string& bolt11, const std::string& payment_hash)
{
    std::string command = "listsendpays";
    Json::Value params(Json::arrayValue);
    if (bolt11 != "")
        params["bolt11"] = bolt11;
    if (payment_hash != "")
        params["payment_hash"] = payment_hash;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::newAddr(const std::string& type)
{
    std::string command = "newaddr";
    Json::Value params(Json::objectValue);
    params["type"] = type;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::pay(const std::string& bolt11, const unsigned int& msats, const std::string& label,
        const unsigned int& riskfactor, const float& maxFeePercent, const unsigned int& retryFor, 
        const unsigned int& maxDelay, const unsigned int exemptFee)
{
    std::string command = "pay";
    Json::Value params(Json::objectValue);
    params.append(bolt11);
    if (msats)
        params["msat"] = msats;
    if (label != "")
        params["label"] = label;
    if (riskfactor)
        params["riskfactor"] = riskfactor;
    if (maxFeePercent)
        params["maxfeepercent"] = maxFeePercent;
    if (retryFor)
        params["retry_for"] = retryFor;
    if (maxDelay)
        params["maxdelay"] = maxDelay;
    if (exemptFee)
        params["exemptfee"] = exemptFee;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::payStatus(const std::string& bolt11)
{
    std::string command = "paystatus";
    Json::Value params(Json::objectValue);
    if (bolt11 != "")
        params["bolt11"] = bolt11;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::ping(const std::string& id, const unsigned int& len, const unsigned int& pongbytes)
{
    std::string command = "ping";
    Json::Value params(Json::objectValue);
    params["id"] = id;
    params["len"] = len;
    params["pongbytes"] = pongbytes;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::sendPay(const std::string& route, const std::string& payment_hash, const std::string& label,
        const unsigned int& msats, const std::string& bolt11)
{
    std::string command = "sendpay";
    Json::Value params(Json::objectValue);
    params["route"] = route;
    params["payment_hash"] = payment_hash;
    if (label != "")
        params["label"] = label;
    if (msats)
        params["msatoshi"] = msats;
    if (bolt11 != "")
        params["bolt11"] = bolt11;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::setChannelFee(const std::string& id, const unsigned int& baseFee, const unsigned int& ppmFee)
{
    std::string command = "setchannelfee";
    Json::Value params(Json::objectValue);
    params["id"] = id;
    if (baseFee)
        params["base"] = baseFee;
    if (ppmFee)
        params["ppm"] = ppmFee;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::stop()
{
    std::string command = "stop";
    Json::Value params(Json::objectValue);
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::waitAnyInvoice(const unsigned int& lastpayIndex)
{
    std::string command = "waitanyinvoice";
    Json::Value params(Json::objectValue);
    if (lastpayIndex)
        params["lastpay_index"] = lastpayIndex;
    return sendCommand(command, params);
}
   
Json::Value CLightningWrapper::waitInvoice(const std::string& label)
{
    std::string command = "waitinvoice";
    Json::Value params(Json::objectValue);
    params["label"] = label;
    return sendCommand(command, params);
}

Json::Value CLightningWrapper::withdraw(const std::string& address, const unsigned int& sats, const unsigned int& feerate,
        const unsigned int& minconf)
{
    std::string command = "withdraw";
    Json::Value params(Json::objectValue);
    params["destination"] = address;
    params["satoshi"] = sats;
    if (feerate)
        params["feerate"] = feerate;
    if (minconf > 1)
        params["minconf"] = minconf;
    return sendCommand(command, params);
}
