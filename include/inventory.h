#ifndef INVENTORY_H
#define INVENTORY_H
#include "item.h"
#include <ostream>

class Inventory {
private:
    Item *inventar;
    short capacitate, item_curent;
public:
    Inventory();
    Inventory(const Inventory &other);
    explicit Inventory(short capacitate_maxima);
    Inventory &operator=(const Inventory &other);
    ~Inventory();
    bool Add_item(const Item &obiect);
    short Foloseste_Item(const std::string& nume, short uzura);
    void Sterge_item(short poz);
    void Swap(short poz1, short poz2);
    int Cauta_Item(const std::string& nume) const;
    Item GetItem(short poz) const;
    friend std::ostream &operator<<(std::ostream &os, const Inventory &inv);
    int Confisca_Contrabanda();
};

#endif // INVENTORY_H
