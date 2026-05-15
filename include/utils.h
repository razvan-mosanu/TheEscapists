#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <type_traits>
#include <vector>

// Template class: Singleton
template <typename T> class Singleton
{
protected:
    Singleton() = default;
    ~Singleton() = default;

public:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
    static T &GetInstance()
    {
        static T instance;
        return instance;
    }
};

// Template function
template <typename T, typename Container>
std::shared_ptr<T> FindEntityByType(const Container &entities)
{
    for (const auto &entity : entities)
        if (auto casted = std::dynamic_pointer_cast<T>(entity)) return casted;
    return nullptr;
}

#endif // UTILS_H
