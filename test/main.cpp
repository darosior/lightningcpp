#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <clightningwrapper.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
    std::string socketPath = std::string(getenv("HOME")) + "/test-dir/lightning-rpc";
    CLightningWrapper * lightning = new CLightningWrapper(socketPath);
    std::cout << "Some pseudo-random calls to test interface" << std::endl;
    try {
        std::cout << "getInfo" << std::endl;
        lightning->getInfo();
        std::cout << "feerates" << std::endl;
        lightning->feerates();
        /*lightning->check();
        lightning->help();
        lightning->invoice();
        lightning->list*;
        lightning->newAddr();
        lightning->payStatus();*/
    } catch (CLightningWrapperException &e) {
        std::cerr << std::endl << "TESTS DID NOT PASS" << std::endl;
        std::cerr << e.getMessage() << std::endl;
    }
    delete lightning;
}
