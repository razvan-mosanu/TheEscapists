#ifndef CELL_H
#define CELL_H
#include "inventory.h"

class Cell
{
private:
private:
    short numar_celula;
    bool usa_blocata;
    bool afis_pe_usa;
    Inventory dulap;
public:
    Cell();
    explicit Cell(short numar_celula);
    ~Cell() = default;
    void Pune_Afis();
    void Schimba_Stare_Usa(bool blocata);
    bool Ascunde_Item_In_Dulap(const Item& ob);
    int Perchezitie();
    friend std::ostream& operator<<(std::ostream& os, const Cell& c);
};

#endif // CELL_H
