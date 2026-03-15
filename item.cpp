#include "item.h"
#include <string>
#include <iostream>

Item::Item() : nume("Gol"), durabilitate(0), contrabanda(false), metal(false) {}
Item::~Item() = default;

Item::Item(const std::string &nume, bool contrabanda, bool metal, short durabilitate)
{
    this->nume = nume;
    this->durabilitate = durabilitate;
    this->contrabanda = contrabanda;
    this->metal = metal;
}

bool Item::Este_Contrabanda() const
{
    return contrabanda;
}

bool Item::Degradare(short procent)
{
    durabilitate -= procent;
    if(durabilitate <= 0)
    {
        durabilitate = 0;
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Item &ob)
{
    if(ob.contrabanda && ob.metal==false)
        os << "[Contrabanda] " << ob.nume << " (" << ob.durabilitate << ")%\n";
    else if(ob.contrabanda && ob.metal)
        os << "[Contrabanda Metal] " << ob.nume << " (" << ob.durabilitate << ")%\n";
    else if(ob.contrabanda == false && ob.metal)
        os << "[Metal] " << ob.nume << " (" << ob.durabilitate << ")%\n";
    else
        os << ob.nume << " (" << ob.durabilitate << ")%\n";
    return os;
}

bool Item::Detectare_contrabanda() const
{
    return contrabanda;
}

bool Item::Detectare_mixt() const
{
    return (contrabanda && metal);
}

void Item::sharpen_item()
{
    if(nume != "Periuta" && nume != "Pieptene") return;
    nume = "Cutit din " + nume;
    contrabanda = true;
    metal = false;
}

