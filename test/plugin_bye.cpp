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
