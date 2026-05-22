#include "exceptions.h"

#include <utility>

GameException::GameException(std::string msg) : message(std::move(msg)) {}

const char *GameException::what() const noexcept { return message.c_str(); }

ResourceLoadException::ResourceLoadException(const std::string &resourceName)
    : GameException("Failed to load resource: " + resourceName) {}

InvalidActionException::InvalidActionException(const std::string &actionDetail)
    : GameException("Invalid action: " + actionDetail) {}

