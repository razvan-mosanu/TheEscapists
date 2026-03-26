#include <iostream>
#include "cell.h"
#include "player.h"
#include "guard.h"

int main()
{
    std::cout << "====== THE ESCAPISTS: A DAY IN PRISON ======\n\n";
    Cell celula_jucator(101);
    Player jucator("Rafa");
    Guard gardian("Sgt. Xslayder");
    std::cout << "Rafa was transferred to cell 101. Patrolling there is " << gardian << "\n";
    std::cout << "\n[08:00] Roll Call!\n";
    jucator.ParticipateCall(true);
    std::cout << "\n[10:00] Free time. Rafa searches for supplies.\n";
    Item soseta("Sock", false, false, 100);
    Item sapun("Soap", false, false, 100);
    Item furculita("Fork", true, true, 80);
    Item lingura("Spoon", true, true, 50);
    jucator.PickUp_Item(soseta);
    jucator.PickUp_Item(sapun);
    jucator.PickUp_Item(furculita);
    jucator.PickUp_Item(lingura);
    std::cout << jucator;
    std::cout << "\n[12:00] Rafa goes to the bathroom and improvises a weapon.\n";
    bool succes = jucator.CraftItem("Sock", "Soap", "Soap Weapon", true, false);
    if(succes)
    {
        std::cout << "Crafting successful! Rafa looks in his pockets:\n";
        std::cout << jucator;
    }
    std::cout << "\n[14:00] A guard is heard in the hallway! Rafa quickly hides the Fork in the cabinet.";
    Item furculita_extrasa = jucator.Extract_Item("Fork");
    celula_jucator.Hide_Item_In_Cabinet(furculita_extrasa);
    std::cout << "\n[16:00] The bell rings for the afternoon roll call, but Rafa refuses to leave the cell.\n";
    jucator.ParticipateCall(false);
    gardian.Inspect_Player(jucator);
    std::cout << "\n[16:15] Due to the high heat, the guards storm cell 101!\n";
    gardian.Search_Cell(celula_jucator);
    std::cout << "\n[23:00] The shakedown is over, Rafa is alone again.\n";
    std::cout << "Rafa finds a spoon and begins to scratch the wall behind the toilet.\n";
    jucator.PickUp_Item(lingura);
    std::cout << "\nCell status before breaking:\n";
    std::cout << celula_jucator;
    std::cout << "\nRafa hits the wall with the spoon repeatedly:\n";
    for(int i=0; i<4; i++)
        celula_jucator.Break_Wall(jucator, "Spoon");
    std::cout << "\n[23:30] Rafa does his night workout and sharpens a toothbrush.\n";
    jucator.Training(10, "speed");
    Item perie("Toothbrush", false, false, 100);
    perie.Sharpen_Item();
    std::cout << "He sharpened the toothbrush: " << perie << "\n";
    celula_jucator.Change_Door_Status(true);
    celula_jucator.Put_Poster();
    std::cout << "\n[23:50] The guard catches him awake! Rafa is beaten unconscious.\n";
    for(int i = 0; i < 3; i++)
        jucator.Get_Beaten();
    Inventory rule3(2);
    rule3.Add_Item(perie);
    Inventory copie = rule3;
    copie.Swap(0, 0);
    std::cout << "\n====== FINAL STATUS DAY 1 ======\n";
    std::cout << jucator;
    std::cout << celula_jucator;
    std::cout << "====== END OF DAY ======\n";
    return 0;
}
