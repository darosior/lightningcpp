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

Json::Value dbDump(Json::Value &params) {
    std::ofstream logFile;
    logFile.open("db.log", std::ios::app);
    std::string operation = params["writes"][0].asString();
    logFile << operation << std::endl;
    logFile.close();
    return Json::Value(true);
}

int main(int argc, char *argv[]) {
    Plugin testPlugin;
    Helloworld helloworld;
    testPlugin.addMethod(helloworld);
    testPlugin.subscribe("warning", &uselessLogger);
    testPlugin.hookSubscribe("db_write", &dbDump);
    testPlugin.start();

    return 0;
}
