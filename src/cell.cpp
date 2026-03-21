#include "cell.h"
#include "player.h"
#include <iostream>

Cell::Cell() : numar_celula(0), usa_blocata(false), afis_pe_usa(false), grad_degradare_perete(0), dulap(8) {}

Cell::Cell(short numar_celula) : numar_celula(numar_celula),
usa_blocata(false), afis_pe_usa(false), grad_degradare_perete(0), dulap(8) {}


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

bool Cell::SpargerePerete(Player& p, const std::string& unealta)
{
    if (grad_degradare_perete >= 100)
    {
        std::cout << "Peretele este deja spart!\n";
        return false;
    }
    short folosit = p.Foloseste_Item(unealta, 25);
    if(folosit > 0)
    {
        short degr_adaugata = (folosit * 34) / 25;
        grad_degradare_perete += degr_adaugata;
        if(grad_degradare_perete > 100) grad_degradare_perete = 100;
        std::cout << "Ai lovit peretele! Degradare perete: " << grad_degradare_perete << "%\n";
        if (grad_degradare_perete == 100) std::cout << "Peretele a fost spart complet!\n";
        return true;
    }
    std::cout << "Nu ai unealta '" << unealta << "' in inventar sau s-a rupt incercand.\n";
    return false;
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
    os << "=== Celula " << c.numar_celula << " ===\n";
    os << "Usa blocata: " << (c.usa_blocata ? "DA" : "NU") << "\n";
    os << "Afis pe usa: " << (c.afis_pe_usa ? "DA" : "NU") << "\n";
    os << "Perete: " << (c.grad_degradare_perete == 100 ? "SPART (100%)" : std::to_string(c.grad_degradare_perete) + "% spart") << "\n";
    os << "--- Dulap Celula ---\n";
    os << c.dulap;
    return os;
}
