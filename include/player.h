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
    explicit Player(std::string nume);
    ~Player() = default;
    short GetHeat() const { return heat; }
    const std::string& GetNume() const {return nume;}
    void Respawn();
    bool Culege_Item(const Item &obiect);
    short Foloseste_Item(const std::string &nume_item, short uzura);
    Item Extrage_Item(const std::string &nume_item);
    void Antrenament(short durata);
    void Incasa_Bataie();
    bool CraftItem(const std::string &item1, const std::string &item2, const std::string &rez, bool contrabanda, bool metal);
    void ParticipareApel(bool prezenta);
    friend std::ostream &operator<<(std::ostream &os, const Player &p);
};

#endif // PLAYER_H
