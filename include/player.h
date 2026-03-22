#ifndef PLAYER_H
#define PLAYER_H
#include "inventory.h"
#include "prison_map.h"
#include <SFML/Graphics.hpp>
#include <ostream>
#include <string>

class Player {
private:
    short viata, stamina, iq, speed, putere, heat, money;
    std::string nume;
    Inventory buzunare;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f poz;
    float moveSpeed;
public:
    explicit Player(std::string nume);
    ~Player() = default;
    short GetHeat() const { return heat; }
    const std::string& GetNume() const {return nume;}
    void Respawn();
    bool Culege_Item(const Item &obiect);
    short Foloseste_Item(const std::string &nume_item, short uzura);
    Item Extrage_Item(const std::string &nume_item);
    void Antrenament(short durata, const std::string& categorie);
    void Incasa_Bataie();
    bool CraftItem(const std::string &item1, const std::string &item2, const std::string &rez, bool contrabanda, bool metal);
    void ParticipareApel(bool prezenta);
    void InitGraphics(const std::string& texturePath, float startX, float startY);
    void UpdateSFML(float deltaTime, const PrisonMap& harta);
    void Draw(sf::RenderWindow& window) const;
    sf::Vector2f GetPoziție() const { return poz; }
    friend std::ostream &operator<<(std::ostream &os, const Player &p);
};

#endif // PLAYER_H
