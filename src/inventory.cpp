#include "inventory.h"
#include "item.h"
#include <utility>

Inventory::Inventory()
{
    capacity = 6;
    curent_item = 0;
    items = new Item[capacity];
}

Inventory::Inventory(short max_capacity)
{
    capacity = max_capacity;
    curent_item = 0;
    items = new Item[capacity];
}

Inventory::Inventory(const Inventory &other)
{
    this->capacity = other.capacity;
    this->curent_item = other.curent_item;
    this->items = new Item[this->capacity];
    for (int i = 0; i < other.curent_item; i++)
        this->items[i] = other.items[i];
}

Inventory &Inventory::operator=(const Inventory &other)
{
    if (this == &other) return *this;
    delete[] this->items;
    this->capacity = other.capacity;
    this->curent_item = other.curent_item;
    this->items = new Item[this->capacity];
    for (int i = 0; i < other.curent_item; i++)
        this->items[i] = other.items[i];
    return *this;
}

Inventory::~Inventory()
{
    delete[] items;
    capacity = curent_item = 0;
}

bool Inventory::Add_Item(const Item &obiect)
{
    if (curent_item == capacity) return false;
    items[curent_item++] = obiect;
    return true;
}

short Inventory::Use_Item(const std::string &name, short uzura)
{
    auto poz = static_cast<short>(Search_Item(name));
    if (poz == -1) return 0;
    short dur_inainte = items[poz].Get_Durability();
    bool intact = items[poz].Degradation(uzura);
    short dur_dupa = items[poz].Get_Durability();
    if (!intact) Delete_Item(poz);
    return (static_cast<short>(dur_inainte - dur_dupa));
}

Item Inventory::Extract_Item(const std::string &name)
{
    auto poz = static_cast<short>(Search_Item(name));
    if (poz == -1) return {};
    Item gasit = items[poz];
    Delete_Item(poz);
    return gasit;
}


void Inventory::Delete_Item(short poz)
{
    if(poz < 0 || poz >= curent_item) return;
    for(short i = poz; i < static_cast<short>(curent_item - 1); i++)
        items[i] = items[i + 1];
    curent_item--;
}

void Inventory::Swap(short poz1, short poz2)
{
    if (poz1 < 0 || poz2 < 0 || poz1 >= curent_item || poz2 >= curent_item) return;
    std::swap(items[poz1], items[poz2]);
}

int Inventory::Search_Item(const std::string &name) const
{
    for (short i = 0; i < curent_item; i++)
        if (items[i].Get_Name() == name) return i;
    return -1;
}

Item Inventory::Get_Item(short poz) const
{
    if (poz >= 0 && poz < curent_item) return items[poz];
    return {};
}

std::ostream &operator<<(std::ostream &os, const Inventory &inv)
{
    if (inv.curent_item == 0)
    {
        os << "curent_item is empty!\n";
        return os;
    }
    os << "curent_item (" << inv.curent_item << "/" << inv.capacity << " slots occupied):\n";
    for (short i = 0; i < inv.curent_item; i++)
        os << "  Slot " << i + 1 << ": " << inv.items[i];
    return os;
}

int Inventory::Confiscate_Contraband()
{
    int confiscated_items = 0;
    for (short i = 0; i < curent_item; i++)
        if (items[i].Is_Contraband())
        {
            confiscated_items++;
            for (short j = i; j < static_cast<short>(curent_item - 1); j++)
                items[j] = items[j + 1];
            curent_item--;
            i--;
        }
    return confiscated_items;
}
