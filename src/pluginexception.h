#ifndef LIGHTNINGCPP_PLUGIN_EXCEPTION_H
#define LIGHTNINGCPP_PLUGIN_EXCEPTION_H

#include <string>

class CLightningPluginException : public std::exception
{
public:
    explicit CLightningPluginException(int code, const std::string &message);
    ~CLightningPluginException() throw();
    
    int getCode();
    const std::string getMessage();
    const char *what() const throw();

private:
    int code;
    const std::string message;
};

#endif // LIGHTNINGCPP_PLUGIN_EXCEPTION_H
