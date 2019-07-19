#include "rpcexception.h"

CLightningRpcException::CLightningRpcException(int code, const std::string& message):
    code(code),
    message(message)
{}

CLightningRpcException::~CLightningRpcException() throw() {}

int CLightningRpcException::getCode()
{
    return code;
}

const std::string CLightningRpcException::getMessage()
{
    return std::string("C-Lightning error : ") + message;
}

const char *CLightningRpcException::what() const throw()
{
    return message.c_str();
}
