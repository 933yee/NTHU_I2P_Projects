#ifndef sionBullet_hpp
#define sionBullet_hpp

#include "ArmyBullet.hpp"
#include <unordered_map>
#include <unordered_set>
#include <utility>
class Army;
class Defense;
namespace Engine {
    struct Point;
}  // namespace Engine

class sionBullet : public ArmyBullet {
public:
    explicit sionBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Army* parent);
    void OnExplode(Defense* defense) override;
    void Update(float deltaTime) override;
    std::unordered_map<float, std::unordered_set<float>> rec_object_alive;
    std::unordered_set<float> rec_object_alive_son;
};


#endif /* sionBullet_hpp */
