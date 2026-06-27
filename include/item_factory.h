#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H

#include "item.h"

#include "utils.h"

// Design Pattern 2: Factory Pattern
class ItemFactory : public Singleton<ItemFactory>
{
    friend class Singleton<ItemFactory>;
private:
    ItemFactory() = default;
public:
    static Item CreateWardenKey();
    static Item CreateWeapon();
    static Item CreateTool();
    static Item CreateIronIngot();
    static Item CreateStick();
    static Item CreateRope();
    static Item CreateDuctTape();
};

#endif // ITEM_FACTORY_H
