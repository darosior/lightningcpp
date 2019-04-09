# lightning-cpp
*A C++11 wrapper to access the CLightning's JSON-RPC interface*  
  
## Installation
This library uses [libjson-rpc-cpp](https://github.com/cinemast/libjson-rpc-cpp) and a [Unix domain socket](https://en.wikipedia.org/wiki/Unix_domain_socket) (which default location is `~/.lightning`)to make the calls. Thus its installation is necessary to use lightning-cpp. Recommended way is to build it from source (here for Debian but dependencies are [here](https://github.com/cinemast/libjson-rpc-cpp)) :  
```shell
apt install libcurl4 libmicrohttpd-dev libjsoncpp-dev libargtable2-dev cmake libhiredis-dev catch
git clone git://github.com/cinemast/libjson-rpc-cpp.git
mkdir -p libjson-rpc-cpp/build
cd libjson-rpc-cpp/build
cmake -DUNIX_DOMAIN_SOCKET_CLIENT=YES .. && make -j8
make install # as root
ldconfig # as root
```  
  
Then you can install lightning-cpp :
```shell
git clone git@github.com:darosior/lightning-cpp.git
cd lightning-cpp
make
make install # as root
```  
  
## Usage

```cpp
#include <clightningwrapper.h>
#include <iostream>
#include <string>

int main (int argc, char * argv[])
{
    std::string socketPath = "/home/<username>/.lightning/lightning-rpc";
    CLightningWrapper lightning = CLightningWrapper(socketPath);
    std::cout << lightning.getInfo() << std::endl;
    
    return 0;
}
```  
  

