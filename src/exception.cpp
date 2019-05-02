#include "exception.h"

CLightningWrapperException::CLightningWrapperException(int code, const std::string& message):
    code(code),
    message(message) {}

CLightningWrapperException::~CLightningWrapperException() throw() {}

int CLightningWrapperException::getCode()
{
    return code;
}

const std::string CLightningWrapperException::getMessage()
{
    return std::string("C-Lightning error : ") + message;
}

const char *CLightningWrapperException::what() const throw()
{
    return message.c_str();
}
