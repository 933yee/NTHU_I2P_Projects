#ifndef PikaBullet_hpp
#define PikaBullet_hpp

#include "ArmyBullet.hpp"

class Army;
class Defense;
namespace Engine {
    struct Point;
}  // namespace Engine

class PikaBullet : public ArmyBullet {
public:
    explicit PikaBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Army* parent);
    void OnExplode(Defense* defense) override;
    void Update(float deltaTime) override;
};


#endif /* ArcherBullet_hpp */
