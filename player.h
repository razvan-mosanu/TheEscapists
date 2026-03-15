#ifndef PLAYER_H
#define PLAYER_H
#include "inventory.h"
#include <ostream>
#include <string>


class Player {
private:
  short viata, stamina, iq, speed, putere, heat, money;
  std::string nume;
  Inventory buzunare;

public:
  explicit Player(const std::string &nume);
  ~Player() = default;
  bool mort() const { return (viata == 0); }
  void Respawn();
  bool Culege_Item(const Item &obiect);
  void Antrenament(short durata);
  void Incasa_Bataie();
  friend std::ostream &operator<<(std::ostream &os, const Player &p);
};

#endif // PLAYER_H
