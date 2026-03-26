#ifndef PLAYER_H
#define PLAYER_H
#include "inventory.h"
#include <ostream>
#include <string>

class Player {
private:
    short life, stamina, iq, speed, power, heat, money;
    std::string name;
    Inventory pocket;
public:
    explicit Player(std::string name);
    ~Player() = default;
    short Get_Heat() const { return heat; }
    short Get_Life() const { return life; }
    const std::string& Get_Name() const {return name;}
    void Respawn();
    bool PickUp_Item(const Item &object);
    short Use_Item(const std::string &name, short wear);
    Item Extract_Item(const std::string &name);
    void Training(short duration, const std::string& category);
    void Get_Beaten(short damage = 5);
    bool CraftItem(const std::string &item1, const std::string &item2, const std::string &rez, bool contraband, bool metal);
    void ParticipateCall(bool present);
    friend std::ostream &operator<<(std::ostream &os, const Player &p);
};

#endif // PLAYER_H
