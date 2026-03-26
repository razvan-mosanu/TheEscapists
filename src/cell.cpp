#include "cell.h"
#include "player.h"
#include <iostream>

Cell::Cell() : cell_number(0), blocked_door(false), poster_on_door(false), wall_degradation(0), cabinet(8) {}

Cell::Cell(short cell_number) : cell_number(cell_number),
    blocked_door(false), poster_on_door(false), wall_degradation(0), cabinet(8) {}


void Cell::Put_Poster()
{
    poster_on_door = true;
}

void Cell::Change_Door_Status(bool status)
{
    blocked_door = status;
}

bool Cell::Hide_Item_In_Cabinet(const Item& ob)
{
    return cabinet.Add_Item(ob);
}

bool Cell::Break_Wall(Player& p, const std::string& tool)
{
    if (wall_degradation >= 100)
    {
        std::cout << "The wall is already broken!\n";
        return false;
    }
    short folosit = p.Use_Item(tool, 25);
    if(folosit > 0)
    {
        auto degr_adaugata = static_cast<short>((folosit * 34) / 25);
        wall_degradation = static_cast<short>(wall_degradation + degr_adaugata);
        if(wall_degradation > 100) wall_degradation = 100;
        std::cout << "You hit the wall! Wall degradation: " << wall_degradation << "%\n";
        if (wall_degradation == 100) std::cout << "The wall has been completely broken!\n";
        return true;
    }
    std::cout << "You don't have the tool '" << tool << "' in your inventory or it broke trying.\n";
    return false;
}

int Cell::Search()
{
    int suspiciune = 0;
    std::cout << "Guard in Cell " << cell_number << " performs a Search!\n";
    if (poster_on_door)
    {
        std::cout << "Poster removed from the bars! (Penalty)\n";
        poster_on_door = false;
        suspiciune += 10;
    }
    int iteme_confiscate = cabinet.Confiscate_Contraband();
    if (iteme_confiscate > 0)
    {
        std::cout << "Found " << iteme_confiscate << " contraband items in the cabinet!\n";
        suspiciune += (iteme_confiscate * 15);
    }
    else std::cout << "You are not clean but i can not prove it.\n";
    return suspiciune;
}

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
    os << "=== Cell " << c.cell_number << " ===\n";
    os << "Door status: " << (c.blocked_door ? "YES" : "NO") << "\n";
    os << "Poster on door: " << (c.poster_on_door ? "YES" : "NO") << "\n";
    os << "Wall: " << (c.wall_degradation == 100 ? "BROKEN (100%)" : std::to_string(c.wall_degradation) + "% broken") << "\n";
    os << "--- Cell cabinet ---\n";
    os << c.cabinet;
    return os;
}
