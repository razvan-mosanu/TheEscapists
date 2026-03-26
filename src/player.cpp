#include "player.h"
#include "item.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <utility>

Player::Player(std::string name_p) : name(std::move(name_p)), pocket(6)
{
    life = 15;
    stamina = 100;
    iq = speed = power = 30;
    heat = money = 0;
}

void Player::Respawn()
{
    life = 15;
    stamina = 100;
    heat = 0;
    money /= 2;
    iq = static_cast<short>(std::max(10, iq - 10));
    speed = static_cast<short>(std::max(10, speed - 10));
    power = static_cast<short>(std::max(10, power - 10));
    pocket.Confiscate_Contraband();
}

bool Player::PickUp_Item(const Item &object)
{
    return pocket.Add_Item(object);
}

short Player::Use_Item(const std::string &name_item, short uzura)
{
    return pocket.Use_Item(name_item, uzura);
}

Item Player::Extract_Item(const std::string &name_item)
{
    return pocket.Extract_Item(name_item);
}

bool Player::CraftItem(const std::string &item1, const std::string &item2, const std::string &rez, bool contraband, bool metal)
{
    auto poz1 = static_cast<short>(pocket.Search_Item(item1));
    if (poz1 == -1) return false;
    Item it1 = pocket.Get_Item(poz1);
    pocket.Delete_Item(poz1);
    auto poz2 = static_cast<short>(pocket.Search_Item(item2));
    if (poz2 == -1)
    {
        pocket.Add_Item(it1);
        return false;
    }
    pocket.Delete_Item(poz2);
    Item obj(rez, contraband, metal, 100);
    pocket.Add_Item(obj);
    std::cout << name << " successfully crafted: " << rez << "!\n";
    return true;
}

void Player::ParticipateCall(bool prezenta)
{
    if (!prezenta)
    {
        heat = static_cast<short>(std::min(100, heat + 90));
        std::cout << name << " missed the roll call! Heat has massively increased to " << heat << "%.\n";
        return;
    }
    heat = static_cast<short>(std::max(0, heat - 10));
    std::cout << name << " participated in the roll call.\n";
}

void Player::Training(short durata, const std::string& categorie)
{
    auto cost_energie = static_cast<short>(durata * 5);
    if (stamina < cost_energie) return;
    stamina = static_cast<short>(stamina - cost_energie);
    if(categorie == "speed") speed = static_cast<short>(std::min(100, speed + durata * 2));
    else if(categorie == "iq") iq = static_cast<short>(std::min(100, iq + durata * 2));
    else power = static_cast<short>(std::min(100, power + durata * 2));
}

void Player::Get_Beaten(short damage)
{
    life = static_cast<short>(std::max(0, life - damage));
    heat = 100;
    std::cout << name << " took a beating and lost " << damage << "HP! (life: " << life << "/15)\n";
    if (life == 0)
    {
        std::cout << name << " couldn't hold on anymore and ended up in the infirmary!\n";
        Respawn();
    }
}

std::ostream &operator<<(std::ostream &os, const Player &p)
{
    os << "=== Player: " << p.name << " ===\n";
    os << "HP: " << p.life << "/15 | Energy: " << p.stamina << "/100\n";
    os << "STR: " << p.power << " | SPD: " << p.speed << " | IQ: " << p.iq
       << "\n";
    os << "Heat: " << p.heat << "% | Money: " << p.money << "$\n";
    os << "--- pocket ---\n";
    os << p.pocket;
    return os;
}
