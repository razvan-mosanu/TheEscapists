#ifndef WARDEN_H
#define WARDEN_H

#include "entity.h"

class Warden : public Entity
{
protected:
    void Print(std::ostream& os) const override;
private:
    std::shared_ptr<class Player> aggroPlayer;
    std::vector<sf::Vector2f> currentPath;

public:
    explicit Warden(std::string nameParam);
    ~Warden() override = default;
    void Update(float deltaTime, const PrisonMap& map) override;
    void Draw(sf::RenderWindow& window) const override;
    std::shared_ptr<Entity> Clone() const override;
    void TakeDamage(short amount) override;
    void SetAggro(std::shared_ptr<class Player> p) { aggroPlayer = p; currentPath.clear(); combatTimer = 5.0f; }
    void ClearAggro() override { aggroPlayer = nullptr; currentPath.clear(); }
};

#endif // WARDEN_H
