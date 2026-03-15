#include "player.h"
#include "item.h"
#include <algorithm>
#include <iostream>
#include <ostream>

Player::Player(const std::string &nume) : nume(nume), buzunare(6)
{
  viata = 15;
  stamina = 100;
  iq = speed = putere = 30;
  heat = money = 0;
}

void Player::Respawn()
{
  viata = 15;
  stamina = 100;
  heat = 0;
  money /= 2;
  iq = std::max(10, iq - 10);
  speed = std::max(10, speed - 10);
  putere = std::max(10, putere - 10);
  buzunare.Confisca_Contrabanda();
}

bool Player::Culege_Item(const Item &obiect)
{
  return buzunare.Add_item(obiect);
}

void Player::Antrenament(short durata)
{
  short cost_energie = durata * 5;
  if (stamina < cost_energie)
    return;
  stamina -= cost_energie;
  putere = std::min(100, putere + durata * 2);
}

void Player::Incasa_Bataie()
{
  viata = 0;
  heat = 100;
  Respawn();
}

std::ostream &operator<<(std::ostream &os, const Player &p)
{
  os << "=== Jucator: " << p.nume << " ===\n";
  os << "HP: " << p.viata << "/15 | Energie: " << p.stamina << "/100\n";
  os << "STR: " << p.putere << " | SPD: " << p.speed << " | IQ: " << p.iq
     << "\n";
  os << "Heat: " << p.heat << "% | Bani: " << p.money << "$\n";
  os << "--- Buzunare ---\n";
  os << p.buzunare;
  return os;
}
