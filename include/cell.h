#ifndef CELL_H
#define CELL_H
#include "inventory.h"

class Player;

class Cell {
private:
  short cell_number;
  bool blocked_door;
  bool poster_on_door;
  short wall_degradation;
  Inventory cabinet;

public:
  Cell();
  explicit Cell(short cell_number);
  ~Cell() = default;
  void Put_Poster();
  void Change_Door_Status(bool status);
  short Get_Cell_Number() const {return cell_number;}
  bool Hide_Item_In_Cabinet(const Item &ob);
  bool Break_Wall(Player& p, const std::string& tool);
  int Search();
  friend std::ostream &operator<<(std::ostream &os, const Cell &c);
};

#endif // CELL_H
