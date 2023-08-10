#ifndef PikaArmy_hpp
#define PikaArmy_hpp

#include "Army.hpp"

class PikaArmy : public Army {
public:
    PikaArmy(int x, int y);
    void CreateBullet(Engine::Point pt) override;
    void Hit(float damage) override;
    void Die();
};

#endif /* PikaArmy_hpp */
