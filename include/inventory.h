#ifndef INVENTORY_H
#define INVENTORY_H
#include "item.h"
#include <ostream>
#include <string>
#include <vector>

class Inventory
{
private:
    Item *items;
    short capacity, currentItemCount;
public:
    Inventory();
    Inventory(const Inventory &other);
    explicit Inventory(short maxCapacity);
    Inventory &operator=(const Inventory &other);
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
