#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H

#include "item.h"

#include "utils.h"

// Design Pattern 2: Factory Pattern + Tema 3: Singleton Instance 1
class ItemFactory : public Singleton<ItemFactory>
{
    friend class Singleton<ItemFactory>;
private:
    ItemFactory() = default;
public:
    Item CreateWardenKey();
    Item CreateWeapon();
    Item CreateTool();
    Item CreateIronIngot();
    Item CreateStick();
    Item CreateRope();
    Item CreateDuctTape();
};

#endif // ITEM_FACTORY_H
