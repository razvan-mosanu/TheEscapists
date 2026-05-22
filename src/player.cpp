#include "player.h"
#include "exceptions.h"
#include "item.h"
#include "item_factory.h"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <ostream>
#include <utility>


Player::Player(std::string nameParam)
    : Entity(std::move(nameParam)), stamina(100), iq(50), speed(50), heat(0),
    money(50), inventory(6)
{
    health = 100;
    maxHealth = 100;
    power = 100; // temporar pentru testare
    moveSpeed = 100.f;
    inventory.AddItem(Item("Duct Tape", false, false, 0));
    inventory.AddItem(Item("Shiv", true, true, 20));
    inventory.AddItem(Item("Pickaxe", true, true, 100));
    inventory.AddItem(ItemFactory::GetInstance().CreateWardenKey()); // Adaugat temporar pentru testare
}

void Player::TakeDamage(short amount)
{
    if (health <= 0) return;
    Entity::TakeDamage(amount); // folosim clasa de baza
}

void Player::Respawn()
{
    health = 15;
    stamina = 100;
    heat = 0;
    money /= 2;
    iq = static_cast<short>(std::max(10, (int)iq - 10));
    speed = static_cast<short>(std::max(10, (int)speed - 10));
    power = static_cast<short>(std::max(10, (int)power - 10));
    combatTimer = 0.f;
    attackTimer = 0.f;
    isKnockedOut = false;
    inventory.ConfiscateContraband();
}

bool Player::CollectItem(const Item &object)
{
    return inventory.AddItem(object);
}

short Player::UseItem(const std::string &itemName, short wear)
{
    return inventory.UseItem(itemName, wear);
}

Item Player::ExtractItem(const std::string &itemName)
{
    return inventory.ExtractItem(itemName);
}

bool Player::CraftItem(const std::vector<std::string> &ingredients,
                       const std::string &result, bool isContraband,
                       bool isMetal)
{
    std::vector<short> positionsToRemove;
    std::vector<Item> currentItems = inventory.GetItems();
    std::vector<bool> used(currentItems.size(), false);

    for (const auto& ing : ingredients)
    {
        bool found = false;
        for (size_t i = 0; i < currentItems.size(); ++i)
        {
            if (!used[i] && currentItems[i].GetName() == ing)
            {
                found = true;
                used[i] = true;
                positionsToRemove.push_back(static_cast<short>(i));
                break;
            }
        }
        if (!found) return false;
    }

    std::sort(positionsToRemove.rbegin(), positionsToRemove.rend());
    for (short pos : positionsToRemove)
    {
        inventory.RemoveItem(pos);
    }

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

void Player::Train(short duration, const std::string &category)
{
    auto energyCost = static_cast<short>(duration * 5);
    if (stamina < energyCost) return;
    stamina = static_cast<short>(stamina - energyCost);
    if (category == "speed")
        speed = static_cast<short>(std::min(100, speed + duration * 2));
    else if (category == "iq")
        iq = static_cast<short>(std::min(100, iq + duration * 2));
    else
        power = static_cast<short>(std::min(100, power + duration * 2));
}

void Player::TakeBeating()
{
    Respawn();
}

void Player::Update(float deltaTime, const PrisonMap &map)
{
    BaseUpdate(deltaTime);

    // Regenerate stamina
    if (stamina < 100)
    {
        staminaAccumulator += deltaTime * 5.0f; // 5 stamina per second
        if (staminaAccumulator >= 1.0f)
        {
            auto stRegen = static_cast<short>(staminaAccumulator);
            stamina = std::min((short)100, (short)(stamina + stRegen));
            staminaAccumulator -= static_cast<float>(stRegen);
        }
    }
    else staminaAccumulator = 0.f;
    sf::Vector2f moveDir(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        moveDir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        moveDir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        moveDir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        moveDir.x += 1.f;
    if (moveDir.x != 0.f || moveDir.y != 0.f)
    {
        float length = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        moveDir /= length;
        facingDir = moveDir;
    }
    sf::Vector2f movement = moveDir * moveSpeed * deltaTime;
    // hitbox
    float left = 3.f;
    float right = 13.f;
    float top = 10.f;
    float bottom = 22.f;

    // x-colision
    if (movement.x != 0.f)
    {
        float nextX = position.x + movement.x;
        bool blockedByWardenRoom = false;
        if (map.IsWardenDoor(nextX + left, position.y + top) ||
            map.IsWardenDoor(nextX + right, position.y + top) ||
            map.IsWardenDoor(nextX + left, position.y + bottom) ||
            map.IsWardenDoor(nextX + right, position.y + bottom))
        {
            if (inventory.FindItem("Warden Key") == -1) blockedByWardenRoom = true;
        }

        bool currentInWall =
            map.IsSolidWall(position.x + left, position.y + top) ||
            map.IsSolidWall(position.x + right, position.y + top) ||
            map.IsSolidWall(position.x + left, position.y + bottom) ||
            map.IsSolidWall(position.x + right, position.y + bottom);

        bool nextInWall = map.IsSolidWall(nextX + left, position.y + top) ||
                          map.IsSolidWall(nextX + right, position.y + top) ||
                          map.IsSolidWall(nextX + left, position.y + bottom) ||
                          map.IsSolidWall(nextX + right, position.y + bottom);

        if (!blockedByWardenRoom && (!nextInWall || currentInWall)) position.x = nextX;
    }
    // y-colision
    if (movement.y != 0.f)
    {
        float nextY = position.y + movement.y;
        bool blockedByWardenRoom = false;
        if (map.IsWardenDoor(position.x + left, nextY + top) ||
            map.IsWardenDoor(position.x + right, nextY + top) ||
            map.IsWardenDoor(position.x + left, nextY + bottom) ||
            map.IsWardenDoor(position.x + right, nextY + bottom))
        {
            if (inventory.FindItem("Warden Key") == -1) blockedByWardenRoom = true;
        }

        bool currentInWall =
            map.IsSolidWall(position.x + left, position.y + top) ||
            map.IsSolidWall(position.x + right, position.y + top) ||
            map.IsSolidWall(position.x + left, position.y + bottom) ||
            map.IsSolidWall(position.x + right, position.y + bottom);

        bool nextInWall = map.IsSolidWall(position.x + left, nextY + top) ||
                          map.IsSolidWall(position.x + right, nextY + top) ||
                          map.IsSolidWall(position.x + left, nextY + bottom) ||
                          map.IsSolidWall(position.x + right, nextY + bottom);

        if (!blockedByWardenRoom && (!nextInWall || currentInWall)) position.y = nextY;
    }
    sprite.setPosition(position);
}

void Player::Draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

std::shared_ptr<Entity> Player::Clone() const
{
    return std::make_shared<Player>(*this);
}

void Player::Print(std::ostream &os) const
{
    os << "=== Player: " << name << " ===\n";
    os << "HP: " << health << "/" << maxHealth << " | Energy: " << stamina
       << "/100\n";
    os << "STR: " << power << " | SPD: " << speed << " | IQ: " << iq << "\n";
    os << "Heat: " << heat << "% | Money: " << money << "$\n";
    os << "--- Inventory ---\n";
    os << inventory;
}
