#include "player.h"
#include "item.h"
#include <algorithm>
#include <iostream>
#include <ostream>

#include <utility>

Player::Player(std::string nume_p) : nume(std::move(nume_p)), buzunare(6) {
  viata = 15;
  stamina = 100;
  iq = speed = putere = 30;
  heat = money = 0;
}

void Player::Respawn() {
  viata = 15;
  stamina = 100;
  heat = 0;
  money /= 2;
  iq = static_cast<short>(std::max(10, iq - 10));
  speed = static_cast<short>(std::max(10, speed - 10));
  putere = static_cast<short>(std::max(10, putere - 10));
  buzunare.Confisca_Contrabanda();
}

bool Player::Culege_Item(const Item &obiect) {
  return buzunare.Add_item(obiect);
}

short Player::Foloseste_Item(const std::string &nume_item, short uzura) {
  return buzunare.Foloseste_Item(nume_item, uzura);
}

bool Player::CraftItem(const std::string &item1, const std::string &item2,
                       const std::string &rez, bool contrabanda, bool metal) {
  auto poz1 = static_cast<short>(buzunare.Cauta_Item(item1));
  if (poz1 == -1)
    return false;
  Item it1 = buzunare.GetItem(poz1);
  buzunare.Sterge_item(poz1);
  auto poz2 = static_cast<short>(buzunare.Cauta_Item(item2));
  if (poz2 == -1) {
    buzunare.Add_item(it1);
    return false;
  }
  buzunare.Sterge_item(poz2);
  Item obj(rez, contrabanda, metal, 100);
  buzunare.Add_item(obj);
  std::cout << nume << " a craftat cu succes: " << rez << "!\n";
  return true;
}

void Player::ParticipareApel(bool prezenta) {
  if (!prezenta) {
    heat = static_cast<short>(std::min(100, heat + 80 + 10));
    std::cout << nume << " a ratat apelul! Heat-ul a crescut masiv la " << heat
              << "%.\n";
    return;
  }
  heat = static_cast<short>(std::max(0, heat - 10));
  std::cout << nume << " a participat la apel.\n";
}

void Player::Antrenament(short durata) {
  auto cost_energie = static_cast<short>(durata * 5);
  if (stamina < cost_energie) return;
  stamina = static_cast<short>(stamina - cost_energie);
  putere = static_cast<short>(std::min(100, putere + durata * 2));
}

void Player::Incasa_Bataie() {
  viata = 0;
  heat = 100;
  Respawn();
}

std::ostream &operator<<(std::ostream &os, const Player &p) {
  os << "=== Jucator: " << p.nume << " ===\n";
  os << "HP: " << p.viata << "/15 | Energie: " << p.stamina << "/100\n";
  os << "STR: " << p.putere << " | SPD: " << p.speed << " | IQ: " << p.iq
     << "\n";
  os << "Heat: " << p.heat << "% | Bani: " << p.money << "$\n";
  os << "--- Buzunare ---\n";
  os << p.buzunare;
  return os;
}
