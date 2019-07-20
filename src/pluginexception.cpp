#include "pluginexception.h"

CLightningPluginException::CLightningPluginException(int code, const std::string &message):
    code(code),
    message(message)
{}

CLightningPluginException::~CLightningPluginException() throw() {}

int CLightningPluginException::getCode()
{
    return code;
}

const std::string CLightningPluginException::getMessage()
{
    return std::string("C-Lightning plugin error : ") + message;
}

const char *CLightningPluginException::what() const throw()
{
    return message.c_str();
}
