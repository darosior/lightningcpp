#include <clightningplugin.h>
#include <string>

int main(int argc, char *argv[]) {
    Plugin testPlugin;
    RpcMethod byeworld("bye", "[name]", "Launch me so I can say bye to someone", "A LONG DESC");
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
