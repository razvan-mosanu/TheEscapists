#include "inmate.h"
#include "player.h"
#include <iostream>
#include <utility>
#include <cmath>

Inmate::Inmate(std::string nameParam)
    : Entity(std::move(nameParam)),
    respect(50),
    inventory(5), wanderTimer(0.f),
    hasTargetZone(false), hasExactTarget(false),
    aggroPlayer(nullptr), breakfastState(0)
{
    health    = 100;
    maxHealth = 100;
    power     = 40;
}

void Inmate::Interact(Entity* other)
{
    if (auto* p = dynamic_cast<Player*>(other))
        std::cout << "[Inmate] " << name << " says: Hey " << p->GetName() << ", need any contraband?\n";
}

void Inmate::Update(float deltaTime, const PrisonMap& map)
{
    BaseUpdate(deltaTime); //from entity
    if (isKnockedOut) return;
    if (aggroPlayer)
    {
        if (aggroPlayer->GetHealth() <= 0)
        {
            aggroPlayer = nullptr;
            wanderPath.clear();
            return;
        }
        sf::Vector2f playerPos = aggroPlayer->GetPosition();
        sf::Vector2f diff = playerPos - position;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist > 250.f)
        {
            aggroPlayer = nullptr;
            wanderPath.clear();
        }
        else if (dist > 20.f)
        {
            if (wanderPath.empty() || GetRandomInt(0, 59) == 0)
                wanderPath = map.FindPath(this->GetCenter(), aggroPlayer->GetCenter());
        }
        else
        {
            if (attackTimer <= 0.f)
            {
                auto dmg = static_cast<short>(power / 4);
                aggroPlayer->TakeDamage(dmg);
                attackTimer = INMATE_ATTACK_COOLDOWN;
                combatTimer = 5.0f;
            }
        }
    }
    else if (hasExactTarget)
    {
        sf::Vector2f diff = exactTarget - this->GetCenter();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist > 4.f)
        {
            if (wanderPath.empty())
            {
                wanderPath = map.FindPath(this->GetCenter(), exactTarget);
                if (wanderPath.empty()) hasExactTarget = false;
            }
        }
        else
        {
            wanderPath.clear();
            if (breakfastState == 0 && hasTargetZone)
            {
                breakfastState = 1;
                SetExactTarget(sf::Vector2f(
                    (21.f + static_cast<float>(GetRandomInt(0, 3))) * 16.f + 8.f,
                    (6.f + static_cast<float>(GetRandomInt(0, 2))) * 16.f + 8.f
                    ));
            }
            else if (breakfastState == 1 && hasTargetZone)
            {
                breakfastState = 2;
                hasExactTarget = false;
            }
        }
    }
    else
    {
        if (wanderPath.empty())
        {
            wanderTimer -= deltaTime;
            if (wanderTimer <= 0.f)
            {
                wanderTimer = 2.0f + static_cast<float>(GetRandomInt(0, 99)) / 50.f;
                bool goodposition = false;
                float cx = 0.f;
                float cy = 0.f;
                while(!goodposition)
                {
                    int tileX = 0, tileY = 0;
                    goodposition = true;
                    if (hasTargetZone)
                    {
                        int startX = static_cast<int>(targetZone.left) / 16;
                        int startY = static_cast<int>(targetZone.top) / 16;
                        int width  = static_cast<int>(targetZone.width) / 16;
                        int height = static_cast<int>(targetZone.height) / 16;
                        tileX = startX + (width  > 0 ? GetRandomInt(0, width - 1)  : 0);
                        tileY = startY + (height > 0 ? GetRandomInt(0, height - 1) : 0);
                    }
                    else
                    {
                        tileX = 2 + GetRandomInt(0, (map.GetWidthInPixels() / 16) - 5);
                        tileY = 2 + GetRandomInt(0, (map.GetHeightInPixels() / 16) - 5);
                    }
                    cx = static_cast<float>(tileX) * 16.f + 8.f;
                    cy = static_cast<float>(tileY) * 16.f + 8.f;
                    if (map.IsSolidWall(cx, cy)) goodposition = false;
                }
                wanderPath = map.FindPath(this->GetCenter(), sf::Vector2f(cx, cy));
            }
        }
    }
    if (!wanderPath.empty())
    {
        sf::Vector2f target = wanderPath.front();
        sf::Vector2f center = this->GetCenter();
        sf::Vector2f dir = target - center;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (dist < 4.f)
        {
            position = target - sf::Vector2f(8.f, 12.f);
            sprite.setPosition(position);
            wanderPath.erase(wanderPath.begin());
        }
        else
        {
            dir /= dist;
            float speed = (aggroPlayer != nullptr) ? 90.f : 60.f;
            position = position + dir * speed * deltaTime;
            sprite.setPosition(position);
        }
    }
}

void Inmate::Draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

std::shared_ptr<Entity> Inmate::Clone() const
{
    return std::make_shared<Inmate>(*this);
}

void Inmate::Print(std::ostream& os) const
{
    os << "=== Inmate: " << name << " ===\n";
    os << "Respect level: " << respect << "%\n";
}
