#ifndef LIGHTNINGCPP_RPC_EXCEPTION_H
#define LIGHTNINGCPP_RPC_EXCEPTION_H

#include <string>

class CLightningRpcException : public std::exception
{
public:
    explicit CLightningRpcException(int code, const std::string &message);
    ~CLightningRpcException() throw();
    
    int getCode();
    const std::string getMessage();
    const char *what() const throw();

private:
    int code;
    const std::string message;
};

#endif // CPPLIGHTNING_EXCEPTION_H
