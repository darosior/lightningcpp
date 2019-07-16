#include <assert.h>
#include <clightningplugin.h>
#include <clightningwrapper.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char * argv[])
{
    char cwd[50];
    getcwd(cwd, sizeof(cwd));
    std::string bitcoin_dir = std::string(cwd) + "/test/bitcoin-dir";
    std::string lightning_dir = std::string(cwd) + "/test/lightning-dir";
    // We test two plugins that do the same thing with different syntax
    std::string hello_plugin_path = std::string(cwd) + "/test/plugin_hello.exx";
    std::string bye_plugin_path = std::string(cwd) + "/test/plugin_bye.exx";
    std::string bitcoin_cmd = "bitcoind -daemon -regtest -server=1 -rpcuser=test -rpcpassword=test -datadir=" + bitcoin_dir;
    std::string lightning_cmd = "lightningd --network=regtest --addr=127.0.0.1:12345 --lightning-dir=" + lightning_dir;
    lightning_cmd += " --plugin=" + hello_plugin_path + " --plugin=" + bye_plugin_path;
    std::cout << "Starting Bitcoin daemon in regtest" << std::endl;
    system(bitcoin_cmd.c_str());
    std::cout << "Starting Lightning daemon on top of it" << std::endl;
    system(lightning_cmd.c_str());
    std::string socketPath = lightning_dir + "/lightning-rpc";
    CLightningWrapper * lightning = new CLightningWrapper(socketPath);
    std::cout << std::endl << "Some pseudo-random calls to test interface" << std::endl;
    try {
        std::cout << "getInfo" << std::endl;
        lightning->getInfo();
        std::cout << "feerates" << std::endl;
        lightning->feerates();
        std::cout << "check" << std::endl;
        Json::Value params(Json::objectValue);
        params["command"] = "sendpay";
        lightning->check("help", params);
        std::cout << "help" << std::endl;
        lightning->help("waitanyinvoice");
        std::cout << "invoice" << std::endl;
        lightning->invoice(1000, "abc", "abc");
        std::cout << "delinvoice" << std::endl;
        lightning->delInvoice("abc");
        std::cout << "listchannels" << std::endl;
        lightning->listChannels();
        std::cout << "listconfigs" << std::endl;
        lightning->listConfigs("alias");
        std::cout << "listforwards" << std::endl;
        lightning->listFunds();
        std::cout << "listinvoices" << std::endl;
        lightning->listInvoices();
        std::cout << "listnodes" << std::endl;
        lightning->listNodes();
        std::cout << "listpays" << std::endl;
        lightning->listPays();
        std::cout << "listpayments" << std::endl;
        lightning->listPayments();
        std::cout << "listpeers" << std::endl;
        lightning->listPeers();
        std::cout << "listsendpays" << std::endl;
        lightning->listSendPays();
        std::cout << "newaddr" << std::endl;
        lightning->newAddr();
        std::cout << "paystatus" << std::endl;
        lightning->payStatus();
        std::cout << std::endl << "ALL TESTS PASSED" << std::endl;
    } catch (CLightningWrapperException &e) {
        std::cerr << std::endl << "TESTS DID NOT PASS" << std::endl;
        std::cerr << e.getMessage() << std::endl;
    }
    std::cout << std::endl << "Calls to test plugins" << std::endl;
    try {
        lightning->sendCommand("hello", Json::Value(Json::objectValue));
    } catch (CLightningWrapperException &e) {
        std::cerr << std::endl << "FAILURE" << std::endl;
        std::cerr << e.getMessage() << std::endl;
    }
    std::cout << "Stopping lightningd" << std::endl;
    lightning->stop();
    std::cout << "Stopping bitcoind" << std::endl;
    std::string stop_cmd = "bitcoin-cli -regtest -rpcuser=test -rpcpassword=test stop";
    system(stop_cmd.c_str());
    delete lightning;
    return 0;
}
