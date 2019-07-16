#include <clightningplugin.h>
#include <string>
#include <fstream>

int main(int argc, char *argv[]) {
    Plugin testPlugin;
    RpcMethod byeworld("bye", "[name]", "Launch me so I can say bye to someone", "A LONG DESC");
    byeworld.setMain([&](Json::Value &params) {
        std::string bye = "Bye bye ";
        if (!params[0]["name"].empty())
            return Json::Value(bye + params[0]["name"].asString());
        return Json::Value(bye + "world !");
    });
    testPlugin.addMethod(byeworld);
    testPlugin.start();
}
