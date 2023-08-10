#ifndef IceArmy_hpp
#define IceArmy_hpp

#include "Army.hpp"

class IceArmy : public Army {
public:
    IceArmy(float x, float y);
    void Update(float deltaTime) override;
    void CreateBullet(Engine::Point pt) override;
    void Hit(float damage) override;
private:
    bool target_is_wall = false;
};

#endif /* IceArmy_hpp */
