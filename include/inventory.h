#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>

class Inventory
{
private:
    short capacity;
    short currentItemCount;
    Item *items;

public:
    Inventory();
    explicit Inventory(short maxCapacity);
    Inventory(const Inventory &other);
    friend void swap(Inventory& first, Inventory& second) noexcept;
    Inventory &operator=(Inventory other);
    ~Inventory();
    bool AddItem(const Item &object);
    short UseItem(const std::string& name, short wear);
    Item ExtractItem(const std::string& name);
    void RemoveItem(short pos);
    void Swap(short pos1, short pos2);
    int FindItem(const std::string& name) const;
    Item GetItem(short pos) const;
    short GetCount() const { return currentItemCount; }
    short GetCapacity() const { return capacity; }
    std::vector<Item> GetItems() const;
    void SetItems(const std::vector<Item>& newItems);
    friend std::ostream &operator<<(std::ostream &os, const Inventory &inv);
    int ConfiscateContraband();
};

#endif // INVENTORY_H
