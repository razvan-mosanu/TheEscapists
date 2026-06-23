#include "item.h"
#include <string>
#include <iostream>
#include <utility>

Item::Item() :
    name("Empty"), durability(0),
    isContraband(false), isMetal(false) {}

Item::~Item() = default;

Item::Item(std::string name, bool isContraband, bool isMetal, short durability)
{
    this->name = std::move(name);
    this->durability = durability;
    this->isContraband = isContraband;
    this->isMetal = isMetal;
}

bool Item::IsContraband() const
{
    return isContraband;
}

short Item::GetDurability() const
{
    return durability;
}

bool Item::Degrade(short percent)
{
    if(percent <= 0) percent = 10;
    durability = static_cast<short>(durability - percent);
    if(durability <= 0)
    {
        durability = 0;
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Item &ob)
{
    if(ob.isContraband && !ob.isMetal) os << "[Contraband] " << ob.name << " (" << ob.durability << ")%\n";
    else if(ob.isContraband && ob.isMetal) os << "[Contraband Metal] " << ob.name << " (" << ob.durability << ")%\n";
    else if(!ob.isContraband && ob.isMetal) os << "[Metal] " << ob.name << " (" << ob.durability << ")%\n";
    else os << ob.name << " (" << ob.durability << ")%\n";
    return os;
}

const std::string& Item::GetName() const
{
    return name;
}
