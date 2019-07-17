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
}
