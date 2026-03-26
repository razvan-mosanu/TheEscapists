#ifndef INVENTORY_H
#define INVENTORY_H
#include "item.h"
#include <ostream>

class Inventory {
private:
    Item *items;
    short capacity, curent_item;
public:
    Inventory();
    Inventory(const Inventory &other);
    explicit Inventory(short max_capacity);
    Inventory &operator=(const Inventory &other);
    ~Inventory();
    bool Add_Item(const Item &object);
    short Use_Item(const std::string& name, short wear);
    Item Extract_Item(const std::string& name);
    void Delete_Item(short poz);
    void Swap(short poz1, short poz2);
    int Search_Item(const std::string& name) const;
    Item Get_Item(short poz) const;
    friend std::ostream &operator<<(std::ostream &os, const Inventory &inv);
    int Confiscate_Contraband();
};

#endif // INVENTORY_H
