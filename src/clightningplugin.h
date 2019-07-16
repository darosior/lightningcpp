#ifndef LIGHTNINGCPP_PLUGIN_H
#define LIGHTNINGCPP_PLUGIN_H

#include "clightningwrapper.h"
#include "rpcmethod.h"

#include <jsonrpccpp/client.h>
#include <vector>

class Plugin {

public:
    Plugin();
    ~Plugin();
    
    /**
     * The Plugin main loop.
     *
     * It listens for incoming JSONRPC requests on stdout.
     */
    void start();

    /**
     * Add a new RPC method to the plugin.
     */
    void addMethod(const RpcMethod &method);

    /**
     * Add a new lightningd startup option.
     */
    void addOption(const Json::Value &option);

protected:
    // Our RPC wrapper
    CLightningWrapper *rpc;
    // Our RPC methods added to lightningd
    std::vector<RpcMethod> rpcMethods;
    // Our options added to lightningd startup
    Json::Value options;
    // Our subscriptions to lightningd notifications
    Json::Value subscriptions;

    /**
     * Creates the callback function for the "manifest" method
     */
    RpcMethod generateManifest();

    /**
     * Creates the callback function for the "init" method
     *
     * It will initialize the RPC wrapper thanks to the informations sent by
     * lightningd
     */
    RpcMethod generateInit();

    /**
     * Prints a JSONRPC v2 response on stdout
     *
     * @param result The JSON object return after request's method execution
     * @param reqId The JSON id of the request
     */
    void printResponseSuccess(const Json::Value &result, const std::string &reqId);

};

#endif // LIGHTNINGCPP_PLUGIN_H
