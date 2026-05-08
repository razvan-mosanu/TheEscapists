#include "player.h"
#include "item.h"
#include "exceptions.h"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <utility>

Player::Player(std::string nameParam) : Entity(std::move(nameParam)),
    stamina(100), iq(50), speed(50), heat(0), money(50), inventory(6)
{
    health    = 100;
    maxHealth = 100;
    power     = 30;
    moveSpeed = 100.f;
    inventory.AddItem(Item("Duct Tape", false, false, 0));
    inventory.AddItem(Item("Shiv", true, true, 20));
}

void Player::TakeDamage(short amount)
{
    if (health <= 0) return;
    health = static_cast<short>(health - amount);
    combatTimer = 5.0f;
    if (health <= 0) health = 0;
}

void Player::Respawn()
{
    health    = 15;
    stamina   = 100;
    heat      = 0;
    money    /= 2;
    iq        = static_cast<short>(std::max(10, (int)iq - 10));
    speed     = static_cast<short>(std::max(10, (int)speed - 10));
    power     = static_cast<short>(std::max(10, (int)power - 10));
    combatTimer   = 0.f;
    attackTimer   = 0.f;
    isKnockedOut  = false;
    inventory.ConfiscateContraband();
}

bool Player::CollectItem(const Item& object)
{
    return inventory.AddItem(object);
}

short Player::UseItem(const std::string& itemName, short wear)
{
    return inventory.UseItem(itemName, wear);
}

Item Player::ExtractItem(const std::string& itemName)
{
    return inventory.ExtractItem(itemName);
}

bool Player::CraftItem(const std::string& item1, const std::string& item2, const std::string& result, bool isContraband, bool isMetal)
{
    auto pos1 = static_cast<short>(inventory.FindItem(item1));
    if (pos1 == -1) return false;
    Item it1 = inventory.GetItem(pos1);
    inventory.RemoveItem(pos1);
    auto pos2 = static_cast<short>(inventory.FindItem(item2));
    if (pos2 == -1)
    {
        inventory.AddItem(it1);
        return false;
    }
    inventory.RemoveItem(pos2);
    Item obj(result, isContraband, isMetal, 100);
    inventory.AddItem(obj);
    std::cout << name << " successfully crafted: " << result << "!\n";
    return true;
}

void Player::AttendRollcall(bool present)
{
    if (!present)
    {
        heat = static_cast<short>(std::min(100, (int)heat + 90));
        std::cout << name << " missed rollcall! Heat increased massively to " << heat << "%.\n";
        return;
    }
    heat = static_cast<short>(std::max(0, (int)heat - 10));
    std::cout << name << " attended rollcall.\n";
}

void Player::Train(short duration, const std::string& category)
{
    auto energyCost = static_cast<short>(duration * 5);
    if (stamina < energyCost) return;
    stamina = static_cast<short>(stamina - energyCost);
    if(category == "speed") speed = static_cast<short>(std::min(100, speed + duration * 2));
    else if(category == "iq") iq = static_cast<short>(std::min(100, iq + duration * 2));
    else power = static_cast<short>(std::min(100, power + duration * 2));
}

void Player::TakeBeating()
{
    Respawn();
}

void Player::Update(float deltaTime, const PrisonMap& map)
{
    BaseUpdate(deltaTime);
    sf::Vector2f movement(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= moveSpeed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += moveSpeed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= moveSpeed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += moveSpeed * deltaTime;
    sf::Vector2f tryX = position;
    tryX.x += movement.x;

    //hitbox
    float left   = 3.f;
    float right  = 13.f;
    float top    = 14.f;
    float bottom = 22.f;

    if (!map.IsSolidWall(tryX.x + left,  tryX.y + top)    &&
        !map.IsSolidWall(tryX.x + right, tryX.y + bottom) &&
        !map.IsSolidWall(tryX.x + right, tryX.y + top)    &&
        !map.IsSolidWall(tryX.x + left,  tryX.y + bottom))
    {
        position.x = tryX.x;
    }
    sf::Vector2f tryY = position;
    tryY.y += movement.y;
    if (!map.IsSolidWall(position.x + left,  tryY.y + top)    &&
        !map.IsSolidWall(position.x + right, tryY.y + bottom) &&
        !map.IsSolidWall(position.x + right, tryY.y + top)    &&
        !map.IsSolidWall(position.x + left,  tryY.y + bottom))
    {
        position.y = tryY.y;
    }
    sprite.setPosition(position);
}

void Player::Draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

std::shared_ptr<Entity> Player::Clone() const
{
    return std::make_shared<Player>(*this);
}

void Player::Print(std::ostream& os) const
{
    os << "=== Player: " << name << " ===\n";
    os << "HP: " << health << "/" << maxHealth << " | Energy: " << stamina << "/100\n";
    os << "STR: " << power << " | SPD: " << speed << " | IQ: " << iq << "\n";
    os << "Heat: " << heat << "% | Money: " << money << "$\n";
    os << "--- Inventory ---\n";
    os << inventory;
}
