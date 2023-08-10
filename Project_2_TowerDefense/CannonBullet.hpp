#ifndef CannonBullet_hpp
#define CannonBullet_hpp

#include "Bullet.hpp"

class Army;
class Defense;
namespace Engine {
struct Point;
}  // namespace Engine

class CannonBullet : public Bullet {
public:
    explicit CannonBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent);
};

class fat_cannon_Bullet : public Bullet {
public:
    explicit fat_cannon_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent);
    explicit fat_cannon_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent, bool second);
    explicit fat_cannon_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent, bool second, bool third);
    void Update(float deltaTime) override;
};

class multi_turrent_Bullet : public Bullet {
public:
    explicit multi_turrent_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent);
    void Update(float deltaTime) override;
};


class wizard_bullet : public Bullet {
public:
    explicit wizard_bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent);
    void Update(float deltaTime) override;
};
#endif /* CannonBullet_hpp */
