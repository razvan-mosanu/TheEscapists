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
    /// daca se blocheaza programul s-ar putea sa ramanem fara RAM
    /// si daca folosim string care ar vrea sa aloce memorie
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

class CraftingException : public GameException
{
public:
    explicit CraftingException(const std::string& item);
};

#endif // EXCEPTIONS_H
