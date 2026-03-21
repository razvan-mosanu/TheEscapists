#ifndef CELL_H
#define CELL_H
#include "inventory.h"

class Player;

class Cell {
private:
  short numar_celula;
  bool usa_blocata;
  bool afis_pe_usa;
  short grad_degradare_perete;
  Inventory dulap;

public:
  Cell();
  explicit Cell(short numar_celula);
  ~Cell() = default;
  void Pune_Afis();
  void Schimba_Stare_Usa(bool blocata);
  short GetNumar() const {return numar_celula;}
  bool Ascunde_Item_In_Dulap(const Item &ob);
  bool SpargerePerete(Player& p, const std::string& unealta);
  int Perchezitie();
  friend std::ostream &operator<<(std::ostream &os, const Cell &c);
};

#endif // CELL_H
