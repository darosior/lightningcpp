#ifndef CPPLIGHTNING_H
#define CPPLIGHTNING_H

#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/unixdomainsocketclient.h>
#include <string>
#include <vector>

class CLightningWrapper
{
private:
    jsonrpc::UnixDomainSocketClient * socketClient;
    jsonrpc::Client * client;
    
public:
    CLightningWrapper(std::string& socket_path);
    ~CLightningWrapper;

    /** Sends a JSON-RPC command to the C-Lightning socket.
     * Used by all following functions to communicate with lightningd.
     */
    Json::Value sendCommand(const std::string& comand, const Json::Value& arguments);

    /* Boolean functions are for calls that don't return anything usefull but for which it can be usefull to
     * return a confirmation (true) after execution (some of these commands can take time, like `pay`). */

    bool autoCleanInvoice(const unsigned int& cycleSeconds, const unsigned int& expiredBy);
    bool check(const std::string& command);
    bool close(const std::string& id, const bool& force=false, const unsigned int& timeout=30);
    bool connect(const std::string& host);
    bool deleteInvoice(const std::string& label, const std::string& status);
    bool delExpiredInvoice(const unsigned int& maxExpiryTime);
    bool delExpiredInvoices();
    bool disconnect(const std::string& id, const bool& force=false);
    bool fundChannel(const std::string& id, const unsigned int& sats, const unsigned int& feerate=0,
            const unsigned int& minconf=0);
    bool pay(const std::string& bolt11, const unsigned int& msat=0, const std::string& label="",
            const unsigned int& riskfactor=10, const unsigned float& maxFeePercent=0.5,
            const unsigned int& retryFor=60, const unsigned int& maxDelay=500, const unsigned int exemptFee=5000);
    bool ping(const std::string& id, const unsigned int& len=128, const unsigned int& pongbytes=128);
    bool stop();
    bool waitAnyInvoice(const unsigned int& lastpayIndex);
    bool waitInvoice(const std::string& label);
    bool withdraw(const std::string& address, const unsigned int& sats, const unsigned int& feerate=0,
            const unsigned int& minconf=1);
    
    
    /* String functions are for calls which return a string as the only usefull information (like an address or 
     * an invoice).*/

    std::string getConfig(const std::string& param);
    std::string getInvoice(const unsigned int& msat, const std::string& label, const std::string& description,
            const unsigned int& expiry=3600, const std::vector<std::string>& fallbacks,
            const std::string& preimage="");
    std::string help();
    std::string helpOn(const std::string& command); // man page
    std::string newAddr(const std::string& type="bech32");
    std::vector<std::string> listNodes();
    
    
    /* JSON Value functions are for calls whether complex, long, that need the JSON syntax (used in lightningd), or simply
     * not yet implemented.*/

    Json::Value decodepay(const sts::string& bolt11);
    Json::Value getFeerate(const std::string& style);
    Json::Value getInfo();
    Json::Value getLog(const std::string& level="info");
    Json::Value getRoute(const std::string& id, const unsigned int& msatoshi, const unsigned int& riskfactor, 
            const unsigned int& cltv=9, const std::string& fromid="", const unsigned float& fuzzPercent=5,
            const std::vector<std::string>& exlude={}, const unsigned int& maxhops=20);
    Json::value listChannels(const std::string& shortChannelId="", const std::string& source="");
    Json::Value listConfigs();
    Json::Value listForwards();
    Json::Value listFunds();
    Json::Value listInvoices(const std::string& label="");
    Json::Value listPay(const std::string& bolt11="");
    Json::Value listPayments(const std::string& bolt11="", const std::string& payment_hash="");
    Json::Value listPeers(const std::string& id, const std::string& level);
    Json::Value listSendPays(const std::string& bolt11="", const std::string& payment_hash="");
    Json::Value payStatus(const std::string& bolt11="");
    Json::Value sendPay(const std::string& route, const std::string& payment_hash, const std::string& label="", 
            const unsigned int& msatoshi=0, const std::string& bolt11="");
};

#endif // CPPLIGHTNING_H
