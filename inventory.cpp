#include "inventory.h"
#include "item.h"

Inventory::Inventory()
{
  capacitate = 6;
  item_curent = 0;
  inventar = new Item[capacitate];
}

Inventory::Inventory(short capacitate_maxima)
{
  capacitate = capacitate_maxima;
  item_curent = 0;
  inventar = new Item[capacitate];
}

Inventory::Inventory(const Inventory &other)
{
  this->capacitate = other.capacitate;
  this->item_curent = other.item_curent;
  this->inventar = new Item[this->capacitate];
  for (int i = 0; i < other.item_curent; i++)
      this->inventar[i] = other.inventar[i];
}

Inventory &Inventory::operator=(const Inventory &other)
{
  if (this == &other) return *this;
  delete[] this->inventar;
  this->capacitate = other.capacitate;
  this->item_curent = other.item_curent;
  this->inventar = new Item[this->capacitate];
  for (int i = 0; i < other.item_curent; i++)
      this->inventar[i] = other.inventar[i];
  return *this;
}

Inventory::~Inventory()
{
  delete[] inventar;
  capacitate = item_curent = 0;
}

bool Inventory::Add_item(const Item &obiect)
{
  if (item_curent == capacitate) return false;
  inventar[item_curent++] = obiect;
  return true;
}

bool Inventory::Foloseste_Item(const std::string &nume, short uzura)
{
  auto poz = static_cast<short>(Cauta_Item(nume));
  if (poz == -1) return false;
  bool intact = inventar[poz].Degradare(uzura);
  if (!intact)
  {
    Sterge_item(poz);
    return true;
  }
  return true;
}

void Inventory::Sterge_item(short poz)
{
  if (poz < 0 || poz >= item_curent) return;
  for (short i = poz; i < static_cast<short>(item_curent - 1); i++)
    inventar[i] = inventar[i + 1];
  item_curent--;
}

void Inventory::Swap(short poz1, short poz2)
{
  if (poz1 < 0 || poz2 < 0 || poz1 >= item_curent || poz2 >= item_curent) return;
  std::swap(inventar[poz1], inventar[poz2]);
}

int Inventory::Cauta_Item(const std::string &nume) const
{
  for (short i = 0; i < item_curent; i++)
    if (inventar[i].GetNume() == nume) return i;
  return -1;
}

Item Inventory::GetItem(short poz) const
{
  if (poz >= 0 && poz < item_curent) return inventar[poz];
  return {};
}

std::ostream &operator<<(std::ostream &os, const Inventory &inv)
{
  if (inv.item_curent == 0)
  {
    os << "Inventarul este gol!\n";
    return os;
  }
  os << "Inventar (" << inv.item_curent << "/" << inv.capacitate << " sloturi ocupate):\n";
  for (short i = 0; i < inv.item_curent; i++)
    os << "  Slot " << i + 1 << ": " << inv.inventar[i];
  return os;
}

int Inventory::Confisca_Contrabanda()
{
  int obiecte_confiscate = 0;
  for (short i = 0; i < item_curent; i++)
    if (inventar[i].Este_Contrabanda())
    {
      obiecte_confiscate++;
      for (short j = i; j < static_cast<short>(item_curent - 1); j++)
          inventar[j] = inventar[j + 1];
      item_curent--;
      i--;
    }
  return obiecte_confiscate;
}
