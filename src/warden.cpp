#include "warden.h"
#include <iostream>

Warden::Warden(std::string nameParam) : Entity(std::move(nameParam))
{
    health = 200;
    maxHealth = 200;
    power = 100;
}

void Warden::Update(float deltaTime, const PrisonMap &/*map*/)
{
    BaseUpdate(deltaTime);
    if (isKnockedOut)
    return;
    // Momentan!
    // Directorul închisorii doar stă și supervizează centrul închisorii.
    // Pe viitor:
    // Vreau sa fac atunci cand bati gardieni sa existe o sansa sa iti pice o
    // cheie si wardenul sa stea intr-o incapere diferita si cu acea cheie sa
    // intri la el daca reusesti sa il bati e ca si cum ai evada din inchisoare
}

void Warden::Draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

std::shared_ptr<Entity> Warden::Clone() const
{
    return std::make_shared<Warden>(*this);
}

void Warden::Print(std::ostream &os) const
{
    os << "=== Warden: " << name << " ===\n";
    os << "The Boss of the Prison.\n";
}
