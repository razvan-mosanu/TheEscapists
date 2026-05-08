#ifndef CELL_H
#define CELL_H
#include "inventory.h"

class Player;

class Cell {
private:
    short cellNumber;
    bool isDoorLocked;
    bool hasPosterOnDoor;
    short wallDamage;
    Inventory cabinet;

public:
    Cell();
    explicit Cell(short cellNumber);
    ~Cell() = default;
    void PlacePoster();
    void ChangeDoorState(bool locked);
    short GetNumber() const {return cellNumber;}
    bool HideItemInCabinet(const Item &ob);
    bool BreakWall(Player& p, const std::string& tool);
    int SearchCell();
    friend std::ostream &operator<<(std::ostream &os, const Cell &c);
};

#endif // CELL_H
