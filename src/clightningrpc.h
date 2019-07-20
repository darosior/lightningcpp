#ifndef LIGHTNINGCPP_RPC_H
#define LIGHTNINGCPP_RPC_H

#include "rpcexception.h"

#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/unixdomainsocketclient.h>
#include <string>
#include <vector>

class CLightningRpc
{
private:
    jsonrpc::UnixDomainSocketClient *socketClient;
    jsonrpc::Client *client;

public:
    CLightningRpc(const std::string &socket_path);
    ~CLightningRpc();

    /**
     * Sends a JSON-RPC command to the C-Lightning socket. Used by all methods to communicate with lightningd.
     */
    Json::Value sendCommand(const std::string &command, const Json::Value &arguments);

    /**
     * Sets up automatic cleaning of expired invoices.
     *
     * @param cycleSeconds sets the cleaning frequency in seconds (defaults to 3600).
     * @param expiredBy sets the minimum time an invoice should have been expired for to be cleaned in seconds (defaults to 86400).
     */
    Json::Value autoCleanInvoice(const unsigned int &cycleSeconds=3600, const unsigned int &expiredBy=86400);

    /**
     * Verify the validity of the command.
     *
     * @param command The command to check.
     * @param parameters The arguments passed to the command, an object with the arg name as key and the arg value as value.
     */
    Json::Value check(const std::string &command, const Json::Value &parameters={});

    /**
     * Close a channel.
     *
     * @param id Either a peer ID, channel ID, or short channel ID.
     * @param force If set to true, forces a unilateral close.
     * @param timeout Seconds beforce forcing an unilateral close.
     */
    Json::Value close(const std::string &id, const bool &force=false, const unsigned int &timeout=30);

    /**
     * Connect to host identified by pubkey@host:port
     *
     * @param id The host id, can be either only a pubkey or "pubkey@ip:port"
     * @param host If not specified in the `id` arg, the host ip
     * @param port If `host` is specified, the according port
     */
    Json::Value connect(const std::string &id, const std::string &host="", const unsigned int &port=9735);

    /**
     * Decode a bolt11-encoded invoice.
     *
     * @param invoice The invoice to be decoded
     * @param description If set, use this description to decode invoice (defaults to "")
     */
    Json::Value decodepay(const std::string &invoice, const std::string &description="");

    /**
     * Delete an unpaid invoice.
     *
     * @param label The label of the invoice to delete
     * @param status The satus of the invoice to delete (default: unpaid)
     */
    Json::Value delInvoice(const std::string &label, const std::string &status="unpaid");

    /**
     * Delete all expired invoices.
     *
     * @param maxExpiryTime a UNIX epoch time after which consider an invoice expired (default:0)
     */
    Json::Value delExpiredInvoices(const unsigned int &maxExpiryTime=0);

    /**
     * Disconnect from a peer.
     *
     * @param id The peer id
     * @param force If set to true, disconnect from the peer even if we have an established channel with it
     */
    Json::Value disconnect(const std::string &id, const bool &force=false);

    /**
     * Get feerate estimates.
     *
     * @param style either perkw (sat per kilo weight) perkb (sat per kilo bytes)
     */
    Json::Value feerates(const std::string &style="perkb");

    /**
     * Fund a channel with a connected node.
     *
     * @param id The peer id
     * @param sats Channel capacity in satoshis
     * @param feerate If set, manually set the feerate.
     * @param announce If set to false, the channel is considered private.
     * @param minconf The number of confirmations the outputs used should have.
     */
    Json::Value fundChannel(const std::string &id, const unsigned int &sats, const unsigned int &feerate=0,
            const bool &announce=true, const unsigned int &minconf=1);

    /** Show some informations about this node */
    Json::Value getInfo();

    /**
     * Show logs.
     *
     * @param level Sets log level between "info"||"unusual"||"debug"||"io" (default to "info")
     */
    Json::Value getLog(const std::string &level="info");

    /**
     * Show a payment route to a connected node.
     *
     * @param id The destination peer
     * @param msats The amount to send through this route
     * @param riskfactor The percentage of yearly stuck funds
     * @param cltv number of blocks to spare between the route source and the destination (default 9).
     * @param fromid If specified, use this peer as the route source (defaults to this node).
     * @param fuzzPercent A floating-point number representing the fee distortion between source and destination (default 5.0)
     * @param seed A string vector of channel to exclude from route. (default : none)
     * @param maxhops The maximum number of channels. (defaults to 20)
     */
    Json::Value getRoute(const std::string &id, const unsigned int &msats, const unsigned int &riskfactor, const unsigned int &cltv=9,
            const std::string &fromId="", const float &fuzzPercent=5.0, const Json::Value &exclude={}, const unsigned int &maxhops=20);

    /**
     * Create an invoice.
     *
     * @param msats Invoice amount in millisatoshis
     * @param label Invoice label (must be unique !)
     * @param description Invoice description
     * @param expiry Expiration time in seconds (default 1 week)
     * @param fallbacks An array of Bitcoin addresses (default empty list)
     * @param preimage Better to not set this (default autogenerated)
     */
    Json::Value invoice(const unsigned int &msats, const std::string &label, const std::string &description,
            const unsigned int &expiry=604800, const Json::Value &fallbacks={}, const std::string &preimage="");

    /** List available calls along with a small description for each */
    Json::Value help(const std::string &command);

    /**
     * Show known channels.
     *
     * @param short_channel_id If specified show only this channel
     * @param source If specified then only channels leading from that node id are returned.
     */
    Json::Value listChannels(const std::string &scid="", const std::string &source="");

    /**
     * List this node's configuration.
     *
     * @param param If set, return only this entry of the configuration
     */
    Json::Value listConfigs(const std::string &param="");

    /** List all forwarded payments and their information */
    Json::Value listForwards();

    /** Show available funds from the internal wallet */
    Json::Value listFunds();

    /**
     * Show an or all invoices
     *
     * @param label If specified, show the invoice with this label
     */
    Json::Value listInvoices(const std::string &label="");

    /**
     * Show known nodes
     *
     * @param id If specified show informations about only this node
     */
    Json::Value listNodes();

    /**
     * List payments result
     *
     * @param bolt11 If specified, show only payment result related to this invoice
     */
    Json::Value listPays(const std::string &bolt11="");

    /**
     * Show outgoing payments
     *
     * @param bolt11 If specified, show only payments with this bolt11
     * @param payment_hash If specified, show only payments with this hash
     */
    Json::Value listPayments(const std::string &bolt11="", const std::string &payment_hash="");

    /**
     * Show current peers
     *
     * @param id If set, show only informations about this peer.
     * @param level If set, show logs as well
     */
    Json::Value listPeers(const std::string &id="", const std::string &level="");

    /**
     * Show sendpays results, old and current.
     *
     * @param bolt11 If set show only sendpays with this invoice
     * @param payment_hash If set show only sendpays with this payment hash.
     */
    Json::Value listSendPays(const std::string &bolt11="", const std::string &payment_hash="");

    /**
     * Get a new Bitcoin address to fund a channel.
     *
     * @param type The address type, which can be one of {bech32, p2sh-segwit} (default is bech32)
     */
    Json::Value newAddr(const std::string &type="bech32");

    /**
     * Send payment specified by bolt11.
     *
     * @param msat Payment amount, should __only__ be specified if not in bolt11.
     * @param label Payment unique string identifier
     * @param riskfactor The percentage of yearly stuck funds (defaults 10).
     * @param maxFeePercent Limits the money paid in fees, and defaults to 0.5.
     * @param retryFor The maximum number of seconds we should try to find new routes and pay through them (defaults 60).
     * @param maxDelay Maximum delay for route randomization (default 500).
     * @param exemptFee Exempt fees for payments with amounts which are less than this value in msat (defaults to 5000)
     */
    Json::Value pay(const std::string &bolt11, const unsigned int &msat=0, const std::string &label="", const unsigned int &riskfactor=10,
            const float &maxFeePercent=0.5, const unsigned int &retryFor=60, const unsigned int &maxDelay=500,
            const unsigned int exemptFee=5000);

     /** Detail status of attempts to pay `bolt11`, or all if not bolt11 specified.*/
    Json::Value payStatus(const std::string &bolt11="");

    /** Send peer `id` a ping of length `len` asking for `pongbytes` */
    Json::Value ping(const std::string &id, const unsigned int &len=128, const unsigned int &pongbytes=128);

    /** Send along `route` in return for preimage of `payment_hash` */
    Json::Value sendPay(const std::string &route, const std::string &payment_hash, const std::string &label="", 
            const unsigned int &msats=0, const std::string &bolt11="");

    /**
     * Set routing fees for a given channel, or all channels.
     *
     * @param id A short channel id, channel id, or peer id of the channel to be modified.
     * @param baseFee An optional value in millisatoshi which is added as base fee to any routed payment.
     * @param ppm An optional value that is added proportionally per-millionths to any routed payment volume in satoshi.
     */
    Json::Value setChannelFee(const std::string &id, const unsigned int &baseFee=0, const unsigned int &ppmFee=0);

    /** Shut down the lightningd process */
    Json::Value stop();

    /**
     * Wait for the next invoice to be paid
     *
     * @param lastpay_index It will not return for any invoices paid prior to or including this pay index
     */
    Json::Value waitAnyInvoice(const unsigned int &lastpayIndex=0);

    /** Wait for the payment an invoice
     *
     * @param label The invoice identifier
     */
    Json::Value waitInvoice(const std::string &label);

    /** Send on chain bitcoins
     * @param address The destination address
     * @param sats The amount to send in satoshis
     * @param feerate Optional feerate to useu
     * @param minconf Don't use output with less confirmations than this param (defaults to 1)
     */
    Json::Value withdraw(const std::string &address, const unsigned int &sats, const unsigned int &feerate=0,
            const unsigned int &minconf=1);
};

#endif // LIGHTNINGCPP_WRAPPER_H
