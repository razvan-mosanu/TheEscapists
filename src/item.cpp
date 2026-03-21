#include "item.h"
#include <string>
#include <iostream>
#include <utility>

Item::Item() : nume("Gol"), durabilitate(0), contrabanda(false), metal(false) {}
Item::~Item() = default;

Item::Item(std::string nume, bool contrabanda, bool metal, short durabilitate)
{
    this->nume = std::move(nume);
    this->durabilitate = durabilitate;
    this->contrabanda = contrabanda;
    this->metal = metal;
}

bool Item::Este_Contrabanda() const
{
    return contrabanda;
}

short Item::GetDurabilitate() const
{
    return durabilitate;
}

bool Item::Degradare(short procent)
{
    durabilitate = static_cast<short>(durabilitate - procent);
    if(durabilitate <= 0)
    {
        durabilitate = 0;
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Item &ob)
{
    if(ob.contrabanda && ob.metal==false) os << "[Contrabanda] " << ob.nume << " (" << ob.durabilitate << ")%\n";
    else if(ob.contrabanda && ob.metal) os << "[Contrabanda Metal] " << ob.nume << " (" << ob.durabilitate << ")%\n";
    else if(ob.contrabanda == false && ob.metal) os << "[Metal] " << ob.nume << " (" << ob.durabilitate << ")%\n";
    else os << ob.nume << " (" << ob.durabilitate << ")%\n";
    return os;
}

void Item::sharpen_item()
{
    if(nume != "Periuta" && nume != "Pieptene") return;
    nume = "Cutit din " + nume;
    contrabanda = true;
    metal = false;
}

const std::string& Item::GetNume() const
{
    return nume;
}
