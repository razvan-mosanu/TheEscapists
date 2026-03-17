#include <iostream>
#include "cell.h"
#include "player.h"

int main()
{
    std::cout << "====== THE ESCAPISTS: O ZI IN INCHISOARE ======\n\n";
    Cell celula_jucator(101);
    Player jucator("Rafa");
    std::cout << "Rafa a fost transferat in celula 101.\n";
    std::cout << "\n[ORA 08:00] Roll Call!\n";
    jucator.ParticipareApel(true); 
    std::cout << "\n[ORA 10:00] Timp liber. Rafa cauta provizii.\n";
    Item soseta("Soseta", false, false, 100);
    Item sapun("Sapun", false, false, 100);
    Item furculita("Furculita", true, true, 80);
    Item lingura("Lingura", true, true, 50);
    jucator.Culege_Item(soseta);
    jucator.Culege_Item(sapun);
    jucator.Culege_Item(furculita);
    jucator.Culege_Item(lingura);
    std::cout << jucator;
    std::cout << "\n[ORA 12:00] Rafa merge in baie si improvizeaza o arma.\n";
    bool succes = jucator.CraftItem("Soseta", "Sapun", "Arma cu Sapun", true, false);
    if(succes)
    {
        std::cout << "Crafting reusit! Rafa se uita in buzunare:\n";
        std::cout << jucator;
    }
    std::cout << "\n[ORA 14:00] Se aude un paznic pe hol! Rafa ascunde rapid Furculita in dulap.\n";
    celula_jucator.Ascunde_Item_In_Dulap(Item("Furculita", true, true, 80));
    std::cout << "\n[ORA 16:00] Suna clopotul pentru apelul de dupa-amiaza, dar Rafa refuza sa iasa din celula.\n";
    jucator.ParticipareApel(false);
    std::cout << "\n[ORA 16:15] Din cauza heat-ului mare, gardienii navalesc in celula 101!\n";
    celula_jucator.Perchezitie();
    std::cout << "\n[ORA 23:00] Perchezitia s-a terminat, Rafa e din nou singur.\n";
    std::cout << "Rafa are in inventar o lingura si incepe sa zgarie peretele din spatele toaletei.\n";
    std::cout << "\nStare celula inainte de spart:\n";
    std::cout << celula_jucator;
    std::cout << "\n-> Rafa loveste peretele cu lingura repetat:\n";
    for(int i=0; i<4; i++)
        celula_jucator.SpargerePerete(jucator, "Lingura");
    std::cout << "\n[ORA 23:30] Rafa isi face antrenamentul de noapte si ascute o perie.\n";
    jucator.Antrenament(10);
    Item perie("Periuta", false, false, 100);
    perie.sharpen_item();
    std::cout << "A ascutit peria: " << perie << "\n";
    celula_jucator.Schimba_Stare_Usa(true);
    celula_jucator.Pune_Afis();
    std::cout << "\n[ORA 23:50] Garda il prinde treaz! Rafa incasa o bataie.\n";
    jucator.Incasa_Bataie();
    Inventory rule3(2);
    rule3.Add_item(perie);
    Inventory copie = rule3;
    copie.Swap(0, 0);
    std::cout << "\n====== STARE FINAL ZIUA 1 ======\n";
    std::cout << jucator;
    std::cout << celula_jucator;
    std::cout << "====== END OF DAY ======\n";
    return 0;
}
