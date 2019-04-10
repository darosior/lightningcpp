#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <clightningwrapper.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
    std::string socketPath = std::string(getenv("HOME")) + "/.lightning/lightning-rpc";
    CLightningWrapper * lightning = new CLightningWrapper(socketPath);
    std::cout << "Testing RPC interface" << std::endl;
    std::cout << "Calling getinfo (returns a Json::Value)" << std::endl;
    std::cout << lightning->getInfo() << std::endl << std::endl;

    std::cout << "Calling check (returns a boolean)" << std::endl;
    bool valid_command = lightning->check("getinfo");
    bool invalid_command = lightning->check("getinfoxx");
    assert(valid_command == true);
    std::cout << "Valid command OK" << std::endl;
    assert(invalid_command == false);
    std::cout << "Invalid command OK" << std::endl << std::endl;

    std::cout << "Calling getConfig (returns a string)" << std::endl;
    std::cout << lightning->getConfig("alias") << std::endl << std::endl;

    std::cout << "Ping a connected node" << std::endl;
    Json::Value nodes = lightning->listPeers();
    if (nodes["peers"].empty())
    {
        std::cout << "You are not connected to any node. Connecting to 03678b4f041fbfbeebcafc076469df75decf81da20a53bd490172e83ce532df1fa." << std::endl;
        if (lightning->connect("03678b4f041fbfbeebcafc076469df75decf81da20a53bd490172e83ce532df1fa@82.255.152.32:9735"))
        {
            std::cout << "Connection OK, trying to ping : " << std::endl;
            assert(lightning->ping("03678b4f041fbfbeebcafc076469df75decf81da20a53bd490172e83ce532df1fa") == true);
            std::cout << "ping OK" << std::endl;
        }
        else
        {
            std::cout << "Connection failed. Passing the test." << std::endl;
        }
    }
    else
    {
        std::string node_id = nodes["peers"][0]["id"].asString();
        assert(lightning->ping(node_id) == true);
        std::cout << "OK" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Testing help on a given command (pay)" << std::endl;
    Json::Value help = lightning->helpOn("pay");
    assert(help["command"].asString().substr(0, 3) == "pay");
    std::cout << "OK" << std::endl << std::endl;

    std::cout << "Testing help" << std::endl;
    std::cout << lightning->help() << std::endl;
    std::cout << "OK" << std::endl << std::endl;

    std::cout << "Tests OK" << std::endl;
    delete lightning;
}
