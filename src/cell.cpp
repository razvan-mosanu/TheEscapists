#include "cell.h"
#include "exceptions.h"
#include "player.h"
#include <iostream>


Cell::Cell():
    cellNumber(0), isDoorLocked(false),
    hasPosterOnDoor(false), wallDamage(0),
    cabinet(8) {}

Cell::Cell(short cellNumber):
    cellNumber(cellNumber), isDoorLocked(false),
    hasPosterOnDoor(false),
    wallDamage(0), cabinet(8) {}

void Cell::PlacePoster() { hasPosterOnDoor = true; }

void Cell::ChangeDoorState(bool locked) { isDoorLocked = locked; }

bool Cell::HideItemInCabinet(const Item &ob) { return cabinet.AddItem(ob); }

bool Cell::BreakWall(Player &p, const std::string &tool)
{
    if (wallDamage >= 100)
    {
        std::cout << "The wall is already broken!\n";
        return false;
    }
    short used = p.UseItem(tool, 25);
    if (used > 0)
    {
        auto damageAdded = static_cast<short>((used * 34) / 25);
        wallDamage = static_cast<short>(wallDamage + damageAdded);
        if (wallDamage > 100) wallDamage = 100;
        std::cout << "You hit the wall! Wall damage: " << wallDamage << "%\n";
        if (wallDamage == 100) std::cout << "The wall has been completely broken!\n";
        return true;
    }
    std::cout << "You don't have the tool '" << tool << "' in your inventory or it broke while trying.\n";
    return false;
}

int Cell::SearchCell(std::vector<Item>& outItems)
{
    int suspicion = 0;
    std::cout << "Guard in Cell " << cellNumber << " is searching!\n";
    if (hasPosterOnDoor)
    {
        std::cout << "Poster removed from bars! (Penalty)\n";
        hasPosterOnDoor = false;
        suspicion += 10;
    }
    outItems = cabinet.ConfiscateContraband(); // preluam obiectele
    int itemsConfiscated = outItems.size();
    if (itemsConfiscated > 0)
    {
        std::cout << "Found " << itemsConfiscated << " contraband items in the cabinet!\n";
        suspicion += (itemsConfiscated * 15);
    }
    else std::cout << "You are not clean but I cannot prove it.\n";
    return suspicion;
}


std::ostream &operator<<(std::ostream &os, const Cell &c)
{
    os << "=== Cell " << c.cellNumber << " ===\n";
    os << "Door locked: " << (c.isDoorLocked ? "YES" : "NO") << "\n";
    os << "Poster on door: " << (c.hasPosterOnDoor ? "YES" : "NO") << "\n";
    os << "Wall: " << (c.wallDamage == 100 ? "BROKEN (100%)" : std::to_string(c.wallDamage)) + "% broken.\n";
    os << "--- Cell Cabinet ---\n";
    os << c.cabinet;
    return os;
}
