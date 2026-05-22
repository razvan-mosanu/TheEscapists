#ifndef PLAYER_H
#define PLAYER_H
#include "entity.h"
#include "inventory.h"
#include "prison_map.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <memory>
#include <ostream>
#include <string>


static constexpr float PLAYER_ATTACK_COOLDOWN = 0.8f;

class Player : public Entity
{
private:
    short stamina, iq, speed, heat, money;
    float staminaAccumulator = 0.f;
    Inventory inventory;
    sf::Vector2f facingDir = sf::Vector2f(0.f, 1.f); // Default facing down
public:
    sf::Vector2f GetFacingDir() const { return facingDir; }
    explicit Player(std::string name);
    ~Player() override = default;
    Inventory &GetInventory() { return inventory; }
    short GetStamina() const { return stamina; }
    short GetMoney() const { return money; }
    void SpendMoney(short amount) { money = std::max((short)0, (short)(money - amount)); }
    short GetHeat() const { return heat; }
    // clamp un fel de interval care nu te lasa sa iesi din el
    // toate datele trebuie sa fie de acelasi tip
    void SetHeat(short value) { heat = std::clamp(value, (short)0, (short)100); }
    void SetHealth(short h) { health = h; }
    void TakeDamage(short amount) override;
    void Respawn();
    bool CollectItem(const Item &object);
    short UseItem(const std::string &itemName, short wear);
    Item ExtractItem(const std::string &itemName);
    void Train(short duration, const std::string &category);
    bool CraftItem(const std::vector<std::string> &ingredients,
                   const std::string &result, bool isContraband,
                   bool isMetal);
    void TakeBeating();
    void AttendRollcall(bool present);

    void Update(float deltaTime, const PrisonMap &map) override;
    void Draw(sf::RenderWindow &window) const override;
    std::shared_ptr<Entity> Clone() const override;

protected:
    void Print(std::ostream &os) const override;
};

#endif // PLAYER_H
