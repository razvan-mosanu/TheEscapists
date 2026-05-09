#include "inventory.h"
#include "item.h"
#include <utility>
#include "exceptions.h"

Inventory::Inventory():
    capacity(6),
    currentItemCount(0),
    items(new Item[6]) {}

Inventory::Inventory(short maxCapacity):
    capacity(maxCapacity), currentItemCount(0),
    items(new Item[maxCapacity]) {}

Inventory::Inventory(const Inventory &other):
    capacity(other.capacity),
    currentItemCount(other.currentItemCount),
    items(new Item[other.capacity])
{
    for (short i = 0; i < currentItemCount; i++)
        items[i] = other.items[i];
}

void swap(Inventory& first, Inventory& second) noexcept
{
    using std::swap;
    swap(first.capacity, second.capacity);
    swap(first.currentItemCount, second.currentItemCount);
    swap(first.items, second.items);
}

Inventory& Inventory::operator=(Inventory other)
{
    swap(*this, other);
    return *this;
}

Inventory::~Inventory()
{
    delete[] items;
    capacity = currentItemCount = 0;
}

bool Inventory::AddItem(const Item &object)
{
    if (currentItemCount == capacity) return false;
    items[currentItemCount++] = object;
    return true;
}

short Inventory::UseItem(const std::string &name, short wear)
{
    auto pos = static_cast<short>(FindItem(name));
    if (pos == -1) throw InvalidActionException("Item not found in inventory: " + name);
    short durBefore = items[pos].GetDurability();
    bool intact = items[pos].Degrade(wear);
    short durAfter = items[pos].GetDurability();
    if (!intact) RemoveItem(pos);
    return static_cast<short>(durBefore - durAfter);
}

Item Inventory::ExtractItem(const std::string &name)
{
    auto pos = static_cast<short>(FindItem(name));
    if (pos == -1) return {};
    Item found = items[pos];
    RemoveItem(pos);
    return found;
}

void Inventory::RemoveItem(short pos)
{
    if(pos < 0 || pos >= currentItemCount) return;
    for(short i = pos; i < static_cast<short>(currentItemCount - 1); i++)
        items[i] = items[i + 1];
    currentItemCount--;
}

void Inventory::Swap(short pos1, short pos2)
{
    if (pos1 < 0 || pos2 < 0 || pos1 >= currentItemCount || pos2 >= currentItemCount) return;
    std::swap(items[pos1], items[pos2]);
}

int Inventory::FindItem(const std::string &name) const
{
    for (short i = 0; i < currentItemCount; i++)
        if (items[i].GetName() == name) return i;
    return -1;
}

Item Inventory::GetItem(short pos) const
{
    if (pos >= 0 && pos < currentItemCount) return items[pos];
    return {};
}

std::vector<Item> Inventory::GetItems() const
{
    return {items, items + currentItemCount};
}

void Inventory::SetItems(const std::vector<Item>& newItems)
{
    currentItemCount = 0;
    for (const auto& item : newItems)
        if (currentItemCount < capacity)
            items[currentItemCount++] = item;
}

std::ostream &operator<<(std::ostream &os, const Inventory &inv)
{
    if (inv.currentItemCount == 0)
    {
        os << "Inventory is empty!\n";
        return os;
    }
    os << "Inventory (" << inv.currentItemCount << "/" << inv.capacity << " slots occupied):\n";
    for (short i = 0; i < inv.currentItemCount; i++)
        os << "  Slot " << i + 1 << ": " << inv.items[i];
    return os;
}

int Inventory::ConfiscateContraband()
{
    int confiscatedItems = 0;
    for (short i = 0; i < currentItemCount; i++)
    {
        if (items[i].IsContraband())
        {
            confiscatedItems++;
            for (short j = i; j < static_cast<short>(currentItemCount - 1); j++)
                items[j] = items[j + 1];
            currentItemCount--;
            i--;
        }
    }
    return confiscatedItems;
}
