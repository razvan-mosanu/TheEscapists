#ifndef GUARD_H
#define GUARD_H

#include "inventory.h"
#include "player.h"
#include "cell.h"
#include <string>
#include <ostream>

class Guard {
private:
    std::string nume;
    Inventory obiecte_confiscate;
public:
    explicit Guard(std::string nume);
    ~Guard() = default;
    void Patruleaza() const;
    void InspecteazaJucator(Player& p);
    void PerchezitieCelula(Cell& c);
    friend std::ostream& operator<<(std::ostream& os, const Guard& g);
};

#endif // GUARD_H
