#ifndef INMATE_H
#define INMATE_H

#include "entity.h"
#include "inventory.h"
#include "prison_map.h"
#include <string>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <memory>

class Player;

static constexpr float INMATE_ATTACK_COOLDOWN = 1.5f;

class Inmate : public Entity {
private:
    int respect;
    Inventory inventory;
    std::vector<sf::Vector2f> wanderPath;
    float wanderTimer;
    sf::FloatRect targetZone;
    bool hasTargetZone;
    sf::Vector2f exactTarget;
    bool hasExactTarget;
    std::shared_ptr<Player> aggroPlayer;
    int breakfastState;
public:
    explicit Inmate(std::string name);
    ~Inmate() override = default;

    Inventory& GetInventory() { return inventory; }
    void SetTargetZone(const sf::FloatRect& zone) { targetZone = zone; hasTargetZone = true; hasExactTarget = false; wanderTimer = 0.f; }
    void SetExactTarget(const sf::Vector2f& target) { exactTarget = target; hasExactTarget = true; hasTargetZone = false; wanderPath.clear(); }
    void ClearTargets() { hasTargetZone = false; hasExactTarget = false; breakfastState = 0; }
    void SetAggro(std::shared_ptr<Player> p) { aggroPlayer = p; wanderPath.clear(); combatTimer = 5.0f; }
    void ClearAggro() override { aggroPlayer = nullptr; wanderPath.clear(); }
    void IncrementBreakfastState() { breakfastState++; }
    int GetBreakfastState() const { return breakfastState; }
    short GetRespect() const { return respect; }
    void Interact(Entity* other) override;

    void Update(float deltaTime, const PrisonMap& map) override;
    void Draw(sf::RenderWindow& window) const override;
    std::shared_ptr<Entity> Clone() const override;

protected:
    void Print(std::ostream& os) const override;
};

#endif // INMATE_H
