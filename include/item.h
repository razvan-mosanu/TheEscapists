#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <ostream>

class Item
{
private:
    std::string name;
    short durability;
    bool contraband;
    bool metal;
public:
    Item();
    Item(std::string name, bool contraband, bool metal, short durability=100);
    ~Item();
    bool Is_Contraband() const;
    bool Degradation(short procent);
    short Get_Durability() const;
    void Sharpen_Item();
    const std::string& Get_Name() const;
    friend std::ostream& operator<<(std::ostream& os, const Item &ob);
};

#endif // ITEM_H
