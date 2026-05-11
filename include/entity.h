#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <ostream>

class PrisonMap;

class Entity {
protected:
    std::string name;
    sf::Vector2f position;
    sf::Texture texture;
    sf::Sprite sprite;
    float moveSpeed;
    short health;
    short maxHealth;
    float regenAccumulator;
    short power;
    float combatTimer;
    float knockoutTimer;
    float attackTimer;
    bool isKnockedOut;
    static int GetRandomInt(int min, int max);
public:
    explicit Entity(std::string name);
    virtual ~Entity();

    static int activeEntities;
    virtual void InitGraphics(const std::string& texturePath, float startX, float startY, sf::Color tint);

    virtual void Update(float deltaTime, const PrisonMap& map) = 0;
    virtual void Draw(sf::RenderWindow& window) const = 0;
    virtual std::shared_ptr<Entity> Clone() const = 0;

    // Non-Virtual Interface
    void Display(std::ostream& os) const { Print(os); }

    virtual void Interact(Entity* /*other*/) {}
    const std::string& GetName() const { return name; }
    sf::Vector2f GetPosition() const { return position; }
    void SetPosition(sf::Vector2f pos) { position = pos; sprite.setPosition(pos); }
    sf::Sprite GetSprite() const { return sprite; }

    virtual short GetHealth() const { return health; }
    virtual short GetMaxHealth() const { return maxHealth; }
    virtual short GetPower() const { return power; }
    //8.f and 12.f because is half of 16.f and 24.f (the sprite)
    sf::Vector2f GetCenter() const { return position + sf::Vector2f(8.f, 12.f); }
    virtual void TakeDamage(short amount);
    void BaseUpdate(float deltaTime);

    bool IsKnockedOut() const { return isKnockedOut; }
    float GetAttackTimer() const { return attackTimer; }
    float GetKnockoutTimer() const { return knockoutTimer; }

    void SetAttackCooldown(float cd) { attackTimer = cd; }
    void SetCombatActive() { combatTimer = 5.0f; }
    virtual void ClearAggro() {}

protected:
    virtual void Print(std::ostream& os) const = 0;
};

#endif // ENTITY_H
