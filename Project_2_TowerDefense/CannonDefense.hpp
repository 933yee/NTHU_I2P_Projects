#ifndef CannonDefense_hpp
#define CannonDefense_hpp

#include "Defense.hpp"

class CannonDefense: public Defense {
public:
    CannonDefense(float x, float y);
    void CreateBullet(Engine::Point pt) override;
};
class fat_cannon_Defense : public Defense {
public:
    fat_cannon_Defense(float x, float y);
    void CreateBullet(Engine::Point pt) override;
};
class multi_turrent_Defense : public Defense {
    int first = 1;
public:
    multi_turrent_Defense(float x, float y);
    void CreateBullet(Engine::Point pt) override;
};

class mines_Defense : public Defense {
    int first = 1;
public:
    mines_Defense(float x, float y);
    void CreateBullet(Engine::Point pt) override;
    void Update(float deltaTime) override;
};

class wizard : public Defense {
    int first = 1;
public:
    wizard(float x, float y);
    void CreateBullet(Engine::Point pt) override;
};
#endif /* CannonDefense_hpp */
