#ifndef GUARD_H
#define GUARD_H

#include "inventory.h"
#include "player.h"
#include "cell.h"
#include <string>
#include <ostream>

class Guard {
private:
    std::string name;
    Inventory confiscated_items;
public:
    explicit Guard(std::string name);
    ~Guard() = default;
    void Inspect_Player(Player& p);
    void Search_Cell(Cell& c);
    friend std::ostream& operator<<(std::ostream& os, const Guard& g);
};

#endif // GUARD_H
