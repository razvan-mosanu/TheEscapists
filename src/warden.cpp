#include "warden.h"
#include <iostream>

#include "player.h"
#include "utils.h"
#include <cmath>

Warden::Warden(std::string nameParam) : Entity(std::move(nameParam)), aggroPlayer(nullptr)
{
    health = 200;
    maxHealth = 200;
    power = 30;
}

void Warden::TakeDamage(short amount)
{
    Entity::TakeDamage(amount);
    if (health <= 0) Notify(this, Event::WardenDefeated);
}

void Warden::Update(float deltaTime, const PrisonMap & map)
{
    BaseUpdate(deltaTime);
    if (isKnockedOut) return;

    if (aggroPlayer)
    {
        if (aggroPlayer->GetHealth() <= 0)
        {
            aggroPlayer = nullptr;
            currentPath.clear();
            return;
        }
        sf::Vector2f playerPos = aggroPlayer->GetPosition();
        sf::Vector2f diff = playerPos - position;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        
        if (dist > 300.f)
        {
            aggroPlayer = nullptr;
            currentPath.clear();
        }
        else if (dist > 20.f)
        {
            if (currentPath.empty() || RandomGenerator::GetInstance().GetInt(0, 59) == 0)
                currentPath = map.FindPath(this->GetCenter(), aggroPlayer->GetCenter());
        }
        else
        {
            if (attackTimer <= 0.f)
            {
                auto dmg = static_cast<short>(power / 2);
                aggroPlayer->TakeDamage(dmg);
                attackTimer = 1.2f;
                combatTimer = 5.0f;
            }
        }
    }
    
    if (!currentPath.empty())
    {
        sf::Vector2f target = currentPath.front();
        sf::Vector2f center = this->GetCenter();
        sf::Vector2f dir = target - center;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (dist < 4.f)
        {
            position = target - sf::Vector2f(8.f, 12.f);
            sprite.setPosition(position);
            currentPath.erase(currentPath.begin());
        }
        else
        {
            dir /= dist;
            float speed = (aggroPlayer != nullptr) ? 80.f : 0.f; // He only moves if aggro'd
            position = position + dir * speed * deltaTime;
            sprite.setPosition(position);
        }
    }
}

void Warden::Draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

std::shared_ptr<Entity> Warden::Clone() const
{
    return std::make_shared<Warden>(*this);
}

void Warden::Print(std::ostream &os) const
{
    os << "=== Warden: " << name << " ===\n";
    os << "The Boss of the Prison.\n";
}
