#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H

#include "item.h"

// Design Pattern 2: Factory Pattern
class ItemFactory
{
public:
    static Item CreateWardenKey();
    static Item CreateWeapon();
    static Item CreateTool();
};

#endif // ITEM_FACTORY_H
