#ifndef SION_hpp
#define SION_hpp

#include "Army.hpp"

class sion : public Army {
public:
    sion(float x, float y);
    sion(float x, float y, int revive);
    
    void CreateBullet(Engine::Point pt) override;
    void Hit(float damage) override;
    void Update(float deltaTime) override;
    int death_time = 0;
private:
    bool target_is_wall = false;
    
};

#endif /* SION_hpp */
