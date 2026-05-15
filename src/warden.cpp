#include "warden.h"
#include <iostream>

Warden::Warden(std::string nameParam) : Entity(std::move(nameParam))
{
    health = 200;
    maxHealth = 200;
    power = 100;
}

void Warden::TakeDamage(short amount)
{
    Entity::TakeDamage(amount);
    if (health <= 0) Notify(this, Event::WardenDefeated);
}

void Warden::Update(float deltaTime, const PrisonMap & /*map*/)
{
    BaseUpdate(deltaTime);
    if (isKnockedOut) return;
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
