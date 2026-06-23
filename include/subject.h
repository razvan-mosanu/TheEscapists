#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"
#include <vector>

class Subject
{
private:
    std::vector<Observer*> observers;
public:
    void AddObserver(Observer* observer)
    {
        observers.push_back(observer);
    }
    void RemoveObserver(Observer* observer)
    {
        for (auto it = observers.begin(); it != observers.end(); ++it)
            if (*it == observer)
            {
                observers.erase(it);
                break;
            }
    }
protected:
    void Notify(Entity* entity, Event event)
    {
        for (Observer* observer : observers)
            observer->OnNotify(entity, event);
    }
};

#endif // SUBJECT_H
