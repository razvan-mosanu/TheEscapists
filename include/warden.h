#ifndef WARDEN_H
#define WARDEN_H

#include "entity.h"

class Warden : public Entity
{
protected:
    void Print(std::ostream& os) const override;

public:
    explicit Warden(std::string nameParam);
    ~Warden() override = default;
    void Update(float deltaTime, const PrisonMap& map) override;
    void Draw(sf::RenderWindow& window) const override;
    std::shared_ptr<Entity> Clone() const override;
};

#endif // WARDEN_H
