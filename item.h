#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <ostream>

class Item
{
private:
    std::string nume;
    short durabilitate;
    bool contrabanda;
    bool metal;
public:
    Item();
    Item(const std::string &nume, bool contrabanda, bool metal, short durabilitate=100);
    ~Item();
    bool Este_Contrabanda() const;
    bool Degradare(short procent);
    bool Detectare_contrabanda() const;
    bool Detectare_mixt() const;
    void sharpen_item();
    friend std::ostream& operator<<(std::ostream& os, const Item &ob);
};

#endif // ITEM_H
