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

Item ItemFactory::CreateIronIngot()
{
    return {"Iron Ingot", true, true, 30};
}

Item ItemFactory::CreateStick()
{
    return {"Stick", false, false, 10};
}

Item ItemFactory::CreateRope()
{
    return {"Rope", true, false, 20};
}

Item ItemFactory::CreateDuctTape()
{
    return {"Duct Tape", false, false, 0};
}
