#ifndef Defense_hpp
#define Defense_hpp

#include <allegro5/base.h>
#include <list>
#include <string>

#include "Role.hpp"

class Army;
class PlayScene;

class Defense: public Role {
protected:
    
    
    float reload = 0;
    float rotateRadian = 2 * ALLEGRO_PI;    
    std::list<Defense*>::iterator lockedDefenseIterator;
    virtual void CreateBullet(Engine::Point pt) = 0;
    float shootRadius;
    
    
public:
    
    bool cooled = false;
    float coolDown;
    int id;
    bool debug = false;
    bool Enabled = true;
    bool Preview = false;
    Army* Target = nullptr;
    std::list<Army*> lockedArmies;
    Defense(std::string imgDefense, float x, float y, float radius, float coolDown, int hp, int id, float shootRadius);
    void Hit(float damage) override;
    void Update(float deltaTime) override;
    void Draw() const override;
    bool InShootingRange(Engine::Point obj);
};

#endif /* Defense_hpp */
