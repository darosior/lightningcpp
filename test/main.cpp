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

    std::cout << "Calling getinfo (returns a boolean)" << std::endl;
    bool valid_command = lightning->check("getinfo");
    //bool invalid_command = lightning->check("getinfoxx");
    assert(valid_command == true);
    //assert(invalid_command == false);
    std::cout << "OK" << std::endl << std::endl;

    std::cout << "Calling getConfig (returns a string)" << std::endl;
    std::cout << lightning->getConfig("alias") << std::endl;

    delete lightning;
}
