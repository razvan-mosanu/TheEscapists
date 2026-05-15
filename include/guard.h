#ifndef GUARD_H
#define GUARD_H

#include "inventory.h"
#include "player.h"
#include "cell.h"
#include "entity.h"
#include <string>
#include <ostream>
#include <memory>

static constexpr float GUARD_ATTACK_COOLDOWN = 1.2f;

class Guard : public Entity
{
private:
    Inventory confiscatedItems;
    std::vector<sf::Vector2f> currentPath; // lista de coordonate catre target
    float patrolTimer;
    sf::FloatRect targetZone;
    bool hasTargetZone;
    sf::Vector2f exactTarget;
    bool hasExactTarget;
    std::shared_ptr<Player> aggroPlayer;

public:
    explicit Guard(std::string name);
    ~Guard() override = default; /// override sa verifice daca destructorul din clasa de baza are virtual

    void SetPath(const std::vector<sf::Vector2f>& path) { currentPath = path; }
    void SetTargetZone(const sf::FloatRect& zone) { targetZone = zone; hasTargetZone = true; hasExactTarget = false; patrolTimer = 0.f; }
    void SetExactTarget(const sf::Vector2f& target) { exactTarget = target; hasExactTarget = true; hasTargetZone = false; currentPath.clear(); }
    void ClearTargets() { hasTargetZone = false; hasExactTarget = false; }
    void SetAggro(std::shared_ptr<Player> p)
    {
        ///bug fix
        ///doar daca se schimba tinta se face clear
        if (aggroPlayer != p) currentPath.clear();
        aggroPlayer = p;
        combatTimer = 5.0f;
    }
    void ClearAggro() override { aggroPlayer = nullptr; currentPath.clear(); }
    Inventory& GetInventory() { return confiscatedItems; }
    bool RollForWardenKeyDrop() const {return GetRandomInt(1, 100) < 10;}
    void InspectPlayer(Player& p);
    void SearchCell(Cell& c);

    void Update(float deltaTime, const PrisonMap& map) override;
    void Draw(sf::RenderWindow& window) const override;
    std::shared_ptr<Entity> Clone() const override;

protected:
    void Print(std::ostream& os) const override;
};

#endif // GUARD_H
