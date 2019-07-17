#ifndef LIGHTNINGCPP_RPCMETHOD_H
#define LIGHTNINGCPP_RPCMETHOD_H

#include <functional>
#include <jsonrpccpp/client.h>
#include <string>

class RpcMethod {

public:
    
    RpcMethod();
    RpcMethod(std::string name, std::string usage, std::string desc, std::string longDesc);

    /**
     * A pointer to the function that should be executed when the method is called
     * through lightningd. This defaults to the 'main()' function below.
     */
    std::function<Json::Value(Json::Value&)> mainFunc;

    /**
     * The default function to be executed when the method is called through lightningd.
     * Should be overriden when inheriting.
     *
     * @param parameters The method parameters
     */
    Json::Value main(Json::Value &parameters);

    /**
     * Sets the default function to be executed when the method is called through lightningd.
     *
     * @param func A function pointer taking a Json::Value as parameter and returnin a Json::Value
     */
    void setMain(std::function<Json::Value(Json::Value&)> func);
    void setMain(Json::Value& (*func)(Json::Value&));

    std::string getName();
    std::string getUsage();
    std::string getDescription();
    std::string getLongDescription();

protected:
    // The method name
    std::string name;
    // The parameters to be passed to this method
    std::string usage;
    // A short description of the method
    std::string description;
    // A long description of the method
    std::string longDescription;

};

#endif // LIGHTNINGCPP_RPCMETHOD_H
