#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <type_traits>
#include <vector>
#include <random>

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


// Template class: Singleton Instance 2
class RandomGenerator : public Singleton<RandomGenerator>
{
    friend class Singleton<RandomGenerator>;
private:
    std::mt19937 gen;
    RandomGenerator() { 
        std::random_device rd; 
        gen = std::mt19937(rd()); 
    }
public:
    int GetInt(int min, int max) {
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(gen);
    }
};

#endif // UTILS_H
