#include "cell.h"
#include <iostream>

Cell::Cell(short numar_celula) : numar_celula(numar_celula),
usa_blocata(false), afis_pe_usa(false), dulap(8) {}


void Cell::Pune_Afis()
{
    afis_pe_usa = true;
}

void Cell::Schimba_Stare_Usa(bool blocata)
{
    usa_blocata = blocata;
}

bool Cell::Ascunde_Item_In_Dulap(const Item& ob)
{
    return dulap.Add_item(ob);
}

int Cell::Perchezitie()
{
    int suspiciune = 0;
    std::cout << "Gardian in Celula " << numar_celula << " face perchezitie!\n";
    if (afis_pe_usa)
    {
        std::cout << "Afis de pe gratii scos! (Penalizare)\n";
        afis_pe_usa = false;
        suspiciune += 10;
    }
    int iteme_confiscate = dulap.Confisca_Contrabanda();
    if (iteme_confiscate > 0)
    {
        std::cout << "S-au gasit " << iteme_confiscate << " obiecte de contrabanda in dulap!\n";
        suspiciune += (iteme_confiscate * 15);
    }
    else std::cout << "You are not clean but i can not prove it.\n";
    return suspiciune;
}

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
    os << "Celula " << c.numar_celula << "\n";
    os << "Usa blocata: " << (c.usa_blocata ? "DA" : "NU") << "\n";
    os << "Afis pe usa: " << (c.afis_pe_usa ? "DA" : "NU") << "\n";
    os << "Dulap Celula \n";
    os << c.dulap;
    return os;
}
