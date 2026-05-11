#include "entity.h"
#include "exceptions.h"
#include <algorithm>
#include <random>

//for events
//better than rand()
int Entity::GetRandomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

int Entity::activeEntities = 0;

//sa arate fix ce e activ nu tot ce s-a creat
Entity::~Entity()
{
    activeEntities--;
}


Entity::Entity(std::string name):
    name(std::move(name)), moveSpeed(0.0f), health(100), maxHealth(100),
    regenAccumulator(0.f), power(10), combatTimer(0.f), knockoutTimer(0.f),
    attackTimer(0.f), isKnockedOut(false) { activeEntities++; }

void Entity::InitGraphics(const std::string &texturePath, float startX, float startY, sf::Color tint)
{
    if (!texture.loadFromFile(texturePath)) throw ResourceLoadException(texturePath);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 16, 24));
    sprite.setColor(tint);
    position = sf::Vector2f(startX, startY);
    sprite.setPosition(position);
}

void Entity::TakeDamage(short amount)
{
    if (isKnockedOut) return;
    health = static_cast<short>(health - amount);
    combatTimer = 5.0f;
    if (health <= 0)
    {
        health = 0;
        isKnockedOut = true;
        knockoutTimer = 15.0f;
        ClearAggro(); //so that no one can chase me after I reappear
    }
}

void Entity::BaseUpdate(float deltaTime)
{
    //cooldown for attack
    if (attackTimer > 0.f) attackTimer -= deltaTime;
    if (isKnockedOut)
    {
        knockoutTimer -= deltaTime;
        if (knockoutTimer <= 0.f)
        {
            isKnockedOut = false;
            health = maxHealth;
            combatTimer = 0.f;
        }
        return;
    }
    if (combatTimer > 0.f) combatTimer -= deltaTime;
    else if (health > 0 && health < maxHealth)
    {
        //regen if no combat
        regenAccumulator += 5.f * deltaTime;
        if (regenAccumulator >= 1.f)
        {
            auto healAmount = static_cast<short>(regenAccumulator);
            health = static_cast<short>(std::min(static_cast<int>(maxHealth), static_cast<int>(health) + healAmount));
            regenAccumulator -= static_cast<float>(healAmount);
        }
    }
}
