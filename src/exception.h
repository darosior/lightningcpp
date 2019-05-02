#ifndef CPPLIGHTNING_EXCEPTION_H
#define CPPLIGHTNING_EXCEPTION_H

#include <string>

class CLightningWrapperException : public std::exception
{
public:
    explicit CLightningWrapperException(int code, const std::string& message);
    ~CLightningWrapperException() throw();
    
    int getCode();
    const std::string getMessage();
    const char *what() const throw();

private:
    int code;
    const std::string message;
};

#endif // CPPLIGHTNING_EXCEPTION_H
