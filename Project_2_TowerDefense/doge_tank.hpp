#ifndef doge_tank_hpp
#define doge_tank_hpp

#include "Army.hpp"

class doge_tank : public Army {
public:
    float timeTicks = 0;
    float timeSpan = 8;
    doge_tank(int x, int y);
    doge_tank(float x, float y, int revive);
    void CreateBullet(Engine::Point pt) override;
    void Hit(float damage) override;
    void Update(float deltatime) override;
    int death_time = 0;
};

class priest : public Army {
public:
    float timeTicks = 0;
    float CD = 7;
    priest(int x, int y);
    void CreateBullet(Engine::Point pt) override;
    void Hit(float damage) override;
    void Update(float deltatime) override;
    int death_time = 0;
};
#endif /* doge_tank_hpp */
