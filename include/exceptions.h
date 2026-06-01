#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class GameException : public std::exception
{
protected:
    std::string message;
public:
    explicit GameException(std::string  msg);
    /// if the program crashes we might run out of RAM
    /// and if we use string which wants to allocate memory
    /// s-ar produce o alta eroare
    [[nodiscard]] const char* what() const noexcept override;
};

class ResourceLoadException : public GameException
{
public:
    explicit ResourceLoadException(const std::string& resourceName);
};

class InvalidActionException : public GameException
{
public:
    explicit InvalidActionException(const std::string& actionDetail);
};

#endif // EXCEPTIONS_H
