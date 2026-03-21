#include "guard.h"
#include <iostream>
#include <utility>

Guard::Guard(std::string nume_g): nume(std::move(nume_g)), obiecte_confiscate(10) {}

void Guard::InspecteazaJucator(Player &p)
{
    std::cout << "[Guard] " << nume << " a oprit jucatorul pentru o inspectie de rutina.\n";
    if (p.GetHeat() > 70)
    {
        std::cout << p.GetNume() << " este panicat! " << nume << " il bate si il trimite la infirmerie.\n";
        p.Incasa_Bataie();
    }
    else std::cout << "  -> Totul pare in regula momentan.\n";
}

void Guard::PerchezitieCelula(Cell &c)
{
    std::cout << "[Guard] " << nume << " face o perchezitie la celula " << c.GetNumar() << ".\n";
    c.Perchezitie();
}

std::ostream &operator<<(std::ostream &os, const Guard &g)
{
    os << "=== Gardian: " << g.nume << " ===\n";
    os << "--- Biroul Gardianului (Obiecte Confiscate) ---\n";
    os << g.obiecte_confiscate;
    return os;
}
