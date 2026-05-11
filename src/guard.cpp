#include "guard.h"
#include <iostream>
#include <utility>
#include <cmath>

Guard::Guard(std::string nameParam)
    : Entity(std::move(nameParam)), confiscatedItems(10), patrolTimer(0.f),
    targetZone(), hasTargetZone(false), hasExactTarget(false),
    aggroPlayer(nullptr) {
    health = 100;
    maxHealth = 100;
    power = 50;
}

void Guard::InspectPlayer(Player &p)
{
    std::cout << "[Guard] " << name << " stopped the player for a routine inspection.\n";
    if (p.GetHeat() > 70)
    {
        std::cout << p.GetName() << " is panicked! " << name << " beats him and sends him to the infirmary.\n";
        p.TakeBeating();
    }
    else std::cout << "  -> Everything seems fine for now.\n";
}

void Guard::SearchCell(Cell &c)
{
    std::cout << "[Guard] " << name << " is searching cell " << c.GetNumber() << ".\n";
    c.SearchCell();
}

void Guard::Update(float deltaTime, const PrisonMap &map)
{
    BaseUpdate(deltaTime); //from entity
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
        sf::Vector2f diff = playerPos - position; //dif in x and y
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist > 250.f) //too far away
        {
            aggroPlayer = nullptr;
            currentPath.clear();
        }
        else if (dist > 20.f)
        {
            //repath one time for one second
            if (currentPath.empty() || GetRandomInt(0, 59) == 0)
                currentPath = map.FindPath(this->GetCenter(), aggroPlayer->GetCenter());
        }
        else
        {
            if (attackTimer <= 0.f) //cooldown is ready
            {
                auto dmg = static_cast<short>(power / 3);
                aggroPlayer->TakeDamage(dmg);
                attackTimer = GUARD_ATTACK_COOLDOWN; //rest cooldown
                combatTimer = 5.0f; //reset timer for regen
            }
        }
    }
    else if (hasExactTarget)
    {
        sf::Vector2f diff = exactTarget - this->GetCenter();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist > 4.f)
        {
            if (currentPath.empty())
            {
                currentPath = map.FindPath(this->GetCenter(), exactTarget);
                if (currentPath.empty()) hasExactTarget = false;
            }
        }
        else currentPath.clear(); /// needs improvment
    }
    else //random path
    {
        if (currentPath.empty())
        {
            patrolTimer -= deltaTime;
            if (patrolTimer <= 0.f) // if rested
            {
                patrolTimer = 1.5f + static_cast<float>(GetRandomInt(0, 99)) / 50.f; //wait time
                bool goodposition = false;
                float cx = 0.f;
                float cy = 0.f;
                while(!goodposition)
                {
                    goodposition = true;
                    int tileX = 0, tileY = 0;
                    if (hasTargetZone)
                    {
                        int startX = static_cast<int>(targetZone.left) / 16;
                        int startY = static_cast<int>(targetZone.top) / 16;
                        int width = static_cast<int>(targetZone.width) / 16;
                        int height = static_cast<int>(targetZone.height) / 16;
                        tileX = startX + (width > 0 ? GetRandomInt(0, width - 1) : 0);
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
                currentPath = map.FindPath(this->GetCenter(), sf::Vector2f(cx, cy));
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
            float speed = (aggroPlayer != nullptr) ? 88.f : 72.f;
            position = position + dir * speed * deltaTime;
            sprite.setPosition(position);
        }
    }
}

void Guard::Draw(sf::RenderWindow &window) const { window.draw(sprite); }

std::shared_ptr<Entity> Guard::Clone() const
{
    return std::make_shared<Guard>(*this);
}

void Guard::Print(std::ostream &os) const
{
    os << "=== Guard: " << name << " ===\n";
    os << "--- Guard's Desk (Confiscated Items) ---\n";
    os << confiscatedItems;
}
