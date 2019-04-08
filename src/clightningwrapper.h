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
    ~CLightningWrapper();

    /** Sends a JSON-RPC command to the C-Lightning socket.
     * Used by all following functions to communicate with lightningd.
     */
    Json::Value sendCommand(const std::string& command, const Json::Value& arguments);

    /* Boolean functions are for calls that don't return anything usefull but for which it can be usefull to
     * return a confirmation (true) after execution (some of these commands can take time, like `pay`). */

    /** Set up autoclean of expired invoices. Perform cleanup every `cycleSeconds`, or disable autoclean if 0.
     * Clean up expired invoices that have expired for `expiredBy` seconds. */
    bool autoCleanInvoice(const unsigned int& cycleSeconds=3600, const unsigned int& expiredBy=86400);
    /** Don't run `command`, just verify parameters. */
    bool check(const std::string& command);
    /** Connect to host identified by pubkey@host:port */
    bool connect(const std::string& host);
    /** Delete unpaid invoice `label` with `status` */
    bool delInvoice(const std::string& label, const std::string& status);
    /** Delete all expired invoices that expired as of given `maxExpiryTime` (a UNIX epoch time) */
    bool delExpiredInvoice(const unsigned int& maxExpiryTime);
    /** Delete all expired invoices. */
    bool delExpiredInvoices();
    /** Disconnect from `id` that has previously been connected to using connect; with `force` set, even if it has a current channel */
    bool disconnect(const std::string& id, const bool& force=false);
    /** Send payment specified by bolt11. `msat` should __only__ be specified if not in bolt11. */
    bool pay(const std::string& bolt11, const unsigned int& msat=0, const std::string& label="",
            const unsigned int& riskfactor=10, const float& maxFeePercent=0.5,
            const unsigned int& retryFor=60, const unsigned int& maxDelay=500, const unsigned int exemptFee=5000);
    /** Send peer `id` a ping of length `len` asking for `pongbytes` */
    bool ping(const std::string& id, const unsigned int& len=128, const unsigned int& pongbytes=128);
    /** Shut down the lightningd process */
    bool stop();
    /** Wait for the next invoice to be paid, after `lastpay_index` (if specified) */
    bool waitAnyInvoice(const unsigned int& lastpayIndex=0);
    /** Wait for an incoming payment matching the invoice with `label`, or if the invoice expires */
    bool waitInvoice(const std::string& label);
    
    
    /* String functions are for calls which return a string as the only usefull information (like an address,
     * an invoice, or a txid).*/

    /** Fund channel with `id` using `sats` satoshis, at optional `feerate`.If `announce` is set to false, the channel is considered private.
     * Only use outputs that have `minconf` confirmations. */
    std::string fundChannel(const std::string& id, const unsigned int& sats, const unsigned int& feerate=0,
            const bool& announce=true, const unsigned int& minconf=1);
    std::string getConfig(const std::string& param);
    std::string getInvoice(const unsigned int& msat, const std::string& label, const std::string& description,
            const unsigned int& expiry=3600, const std::vector<std::string>& fallbacks={},
            const std::string& preimage="");
    std::string help();
    std::string helpOn(const std::string& command); // man page
    std::string newAddr(const std::string& type="bech32");
    std::vector<std::string> listNodes();
    /** Send to `address` `sats` satoshis via Bitcoin transaction, at optional `feerate`, using outputs with at least `minconf` confirmations. */
    std::string withdraw(const std::string& address, const unsigned int& sats, const unsigned int& feerate=0,
            const unsigned int& minconf=1);
    
    /* JSON Value functions are for calls whether complex, long, that need the JSON syntax (used in lightningd), or simply
     * not yet implemented.*/

    Json::Value close(const std::string& id, const bool& force=false, const unsigned int& timeout=30);
    Json::Value decodepay(const std::string& bolt11);
    Json::Value getFeerate(const std::string& style);
    Json::Value getInfo();
    Json::Value getLog(const std::string& level="info");
    Json::Value getRoute(const std::string& id, const unsigned int& msatoshi, const unsigned int& riskfactor, 
            const unsigned int& cltv=9, const std::string& fromid="", const float& fuzzPercent=5,
            const std::vector<std::string>& exlude={}, const unsigned int& maxhops=20);
    Json::Value listChannels(const std::string& shortChannelId="", const std::string& source="");
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
