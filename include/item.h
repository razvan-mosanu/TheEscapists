#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <ostream>

class Item
{
private:
    std::string name;
    short durability;
    bool isContraband;
    bool isMetal;
public:
    Item();
    Item(std::string name, bool isContraband, bool isMetal, short durability=100);
    ~Item();
    bool IsContraband() const;
    bool Degrade(short percent);
    short GetDurability() const;
    void SharpenItem();
    const std::string& GetName() const;
    friend std::ostream& operator<<(std::ostream& os, const Item &ob);
};

#endif // ITEM_H
