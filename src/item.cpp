#include "item.h"
#include <string>
#include <iostream>
#include <utility>

Item::Item() : name("Empty"), durability(0), contraband(false), metal(false) {}
Item::~Item() = default;

Item::Item(std::string name, bool contraband, bool metal, short durability)
{
    this->name = std::move(name);
    this->durability = durability;
    this->contraband = contraband;
    this->metal = metal;
}

bool Item::Is_Contraband() const
{
    return contraband;
}

short Item::Get_Durability() const
{
    return durability;
}

bool Item::Degradation(short procent)
{
    if(procent <= 0) procent = 10;
    durability = static_cast<short>(durability - procent);
    if(durability <= 0)
    {
        durability = 0;
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Item &ob)
{
    if(ob.contraband && ob.metal==false) os << "[Contraband] " << ob.name << " (" << ob.durability << ")%\n";
    else if(ob.contraband && ob.metal) os << "[Metal Contraband] " << ob.name << " (" << ob.durability << ")%\n";
    else if(ob.contraband == false && ob.metal) os << "[Metal] " << ob.name << " (" << ob.durability << ")%\n";
    else os << ob.name << " (" << ob.durability << ")%\n";
    return os;
}

void Item::Sharpen_Item()
{
    if(name != "Toothbrush" && name != "Comb") return;
    name = "Knife from " + name;
    contraband = true;
    metal = false;
}

const std::string& Item::Get_Name() const
{
    return name;
}
