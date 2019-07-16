#include <clightningplugin.h>
#include <string>

class Helloworld: public RpcMethod {
public:
    Helloworld() {
        name = "hello";
        description = "launch me so that I can greet the world";
        setMain([&](Json::Value &parameters) {
            return Json::Value("Hello world!");
        });
    }
};

int main(int argc, char *argv[]) {
    Plugin testPlugin;
    Helloworld helloworld;
    testPlugin.addMethod(helloworld);
    testPlugin.start();
}
