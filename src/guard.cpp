#include "guard.h"
#include <iostream>
#include <utility>

Guard::Guard(std::string name_g): name(std::move(name_g)), confiscated_items(10) {}

void Guard::Inspect_Player(Player &p)
{
    std::cout << "[Guard] " << name << " stopped the player for a routine inspection.\n";
    if (p.Get_Heat() > 70)
    {
        std::cout << p.Get_Name() << " is panicked! " << name << " beats him and sends him to the infirmary.\n";
        p.Get_Beaten(p.Get_Life());
    }
    else std::cout << "  -> Everything seems fine for now.\n";
}

void Guard::Search_Cell(Cell &c)
{
    std::cout << "[Guard] " << name << " performs a Search on cell " << c.Get_Cell_Number() << ".\n";
    c.Search();
}

std::ostream &operator<<(std::ostream &os, const Guard &g)
{
    os << "=== Guard: " << g.name << " ===\n";
    os << "--- Guard's Desk (Confiscated Items) ---\n";
    os << g.confiscated_items;
    return os;
}
