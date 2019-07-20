# lightning-cpp
*A C++11 wrapper to build plugins for C-lightning and access its JSON-RPC interface*  
  
## Installation
This library makes an extensive use of [libjson-rpc-cpp](https://github.com/cinemast/libjson-rpc-cpp). Recommended way is to build it from source (here are Debian instructions but you can find more [here](https://github.com/cinemast/libjson-rpc-cpp)) :  
```shell
apt install libcurl4 libmicrohttpd-dev libjsoncpp-dev libargtable2-dev cmake libhiredis-dev catch libcurl4-openssl-dev
git clone git://github.com/cinemast/libjson-rpc-cpp.git
mkdir -p libjson-rpc-cpp/build
cd libjson-rpc-cpp/build
cmake -DUNIX_DOMAIN_SOCKET_CLIENT=YES .. && make -j8
make install # as root
ldconfig # as root
```  
Then you can install lightningcpp :
```shell
git clone git@github.com:darosior/lightningcpp.git
cd lightningcpp
make
su -c "make install"
```  
  
## Usage
  
### RPC wrapper
```cpp
#include <clightningrpc.h>
#include <iostream>
#include <string>

int main (int argc, char *argv[])
{
    CLightningWrapper lightning = CLightningWrapper("/home/darosior/.lightning/lightning-rpc");
    std::cout << lightning.getInfo() << std::endl;
    
    return 0;
}
```  
  
### Plugin

You can write a plugin by whether inheriting the `RpcMethod` class :
```cpp
#include <clightningplugin.h>

class Helloworld: public RpcMethod {
public:
    Helloworld(): RpcMethod() {
        name = "hello";
        description = "launch me so that I can greet the world";
    }

    Json::Value main(Json::Value &params) {
        return Json::Value("Hello world !");
    }
};

int main(int argc, char *argv[]) {
    Plugin testPlugin;
    Helloworld helloworld;
    testPlugin.addMethod(helloworld);
    testPlugin.start();

    return 0;
}
```
  
Or you can instanciate an `RpcMethod` object and set the main function :
```cpp
#include <clightningplugin.h>
#include <string>

int main(int argc, char *argv[]) {
    Plugin testPlugin;
    RpcMethod byeworld("bye", "[name]", "Launch me so I can say bye to someone", "I am a long description");
    byeworld.setMain([&](Json::Value &params) {
        std::string bye = "Bye bye ";
        if (!params.empty())
            return Json::Value(bye + params.asString());
        return Json::Value(bye + "world !");
    });
    testPlugin.addMethod(byeworld);
    testPlugin.start();

    return 0;
}
```
   
#### Options 

You can add startup options, for example with the byebye plugin from above :  
```cpp
#include <clightningplugin.h>
#include <string>

int main(int argc, char *argv[]) {
    Plugin testPlugin;
    testPlugin.addOption("byename", "world", "who to say bye to");
    RpcMethod byeworld("bye", "[name]", "Launch me so I can say bye to someone", "A LONG DESC");
    byeworld.setMain([&](Json::Value &params) {
        std::string bye = "Bye bye ";
        if (params.size() == 1)
            return Json::Value(bye + params[0].asString() + " !");
        return Json::Value(bye + testPlugin.getOptionValue("byename").asString() + " !");
    });
    testPlugin.addMethod(byeworld);
    testPlugin.start();

    return 0;
}
```
You can use `addOption()` by whether passing it a `Json::Value`, or the option
name, default value, description, and type (which defaults to `string` and is
not passed in the above example).  
  
#### Notifications

You can subscribe to [`lightningd`'s notifications](https://lightning.readthedocs.io/PLUGINS.html#event-notifications) :  
```cpp
#include <clightningplugin.h>
#include <fstream>

class Helloworld: public RpcMethod {
public:
    Helloworld(): RpcMethod() {
        name = "hello";
        description = "launch me so that I can greet the world";
    }

    Json::Value main(Json::Value &params) {
        return Json::Value("Hello world !");
    }
};

void uselessLogger(Json::Value &params) {
    std::ofstream logFile;
    logFile.open("log_file.log", std::ios::app);
    std::string what = params["warning"]["log"].asString();
    logFile << what << std::endl;
    logFile.close();
}

int main(int argc, char *argv[]) {
    Plugin testPlugin;
    Helloworld helloworld;
    testPlugin.addMethod(helloworld);
    testPlugin.subscribe("warning", &uselessLogger);
    testPlugin.start();

    return 0;
}
```
  
## Design
  
### Brief

  Highly inspired by Christian Decker's [pylightning](https://github.com/ElementsProject/lightning/tree/master/contrib/pylightning).  

### Rpc wrapper
  
The RPC wrapper is just a class (`CLightningRpc`) with methods wrapping `lightningd`'s API.  
  
### Plugin interface

There are 2 classes for the Plugin management : `Plugin` and `RpcMethod`. Every RPC method added to `lightningd` must be an instance of
`RpcMethod`, which is composed of a method's attributes (namely the name, usage, description and long description) and a pointer to the
function to be executed when the method is called through `lightningd`.   

This pointer defaults to the `main(Json::Value&)` function of the same class.  
Any method's main function must take a `Json::Value` object as parameter and return a `Json::Value`.  
You can assign it after instanciation with the `setMain()` method (as per examples above).  
  
## More about C-lightning plugins

You can find more about C-lightning plugins :  
- On the [lightningd/plugins repo](https://github.com/lightningd/plugins)
- On the [doc](https://lightning.readthedocs.io/PLUGINS.html)
- On the [C-lightning repo](https://github.com/ElementsProject/lightning)
  
Plugins in other languages :
- [Python](https://github.com/ElementsProject/lightning/blob/master/contrib/pylightning)
- [C](https://github.com/ElementsProject/lightning/blob/master/plugins/libplugin.h)
- [Go](https://github.com/niftynei/glightning)
  
## LICENCE

BSD 3-clause clear
