#include "item_factory.h"

Item ItemFactory::CreateWardenKey()
{
    return {"Warden Key", true, true, 100};
}

Item ItemFactory::CreateWeapon()
{
    return {"Shiv", true, true, 50};
}

Item ItemFactory::CreateTool()
{
    return {"Screwdriver", true, true, 80};
}
