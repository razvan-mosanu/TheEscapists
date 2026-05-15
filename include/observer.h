#ifndef OBSERVER_H
#define OBSERVER_H

class Entity;

enum class Event
{
    WardenDefeated,
    GuardDefeated
};

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void OnNotify(Entity* entity, Event event) = 0;
};

#endif // OBSERVER_H
