#include <allegro5/base.h>
#include <random>
#include <string>

#include "AudioHelper.hpp"
#include "DirtyEffect.hpp"
#include "CannonBullet.hpp"
#include "Group.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Army.hpp"
#include "Bullet.hpp"
#include "Collider.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "IScene.hpp"
#include "Sprite.hpp"
#include "ExplosionEffect.hpp"
#include "sion.hpp"
#include "doge_tank.hpp"
class Defense;

//Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* defenseParent);
CannonBullet::CannonBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent) :
    Bullet("play/bullet-9.png", 500, 5, position, forwardDirection, rotation, parent) {
}

fat_cannon_Bullet::fat_cannon_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent) :
    Bullet("play/bullet-4.png", 500, 0, position, forwardDirection, rotation, parent) {
}
fat_cannon_Bullet::fat_cannon_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent, bool first) :
    Bullet("play/bullet-5.png", 500, 0, position, forwardDirection, rotation, parent) {
}
fat_cannon_Bullet::fat_cannon_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent, bool second, bool third) :
    Bullet("play/bullet-6.png", 500, 0, position, forwardDirection, rotation, parent) {
}

void fat_cannon_Bullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();
    
    for (auto& it : scene->ArmyGroup->GetObjects()) {
        Army* army = dynamic_cast<Army*>(it);
        if (!army->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, army->Position, army->CollisionRadius)) {
            if (army->id == 3) {
                sion* tmp = dynamic_cast<sion*>(army);
                if (tmp->death_time == 1) {
                    getPlayScene()->BulletGroup->RemoveObject(objectIterator);
                    return;
                }
            }
            if (army->id == 4) {
                doge_tank* tmp = dynamic_cast<doge_tank*>(army);
                if (tmp->death_time == 1) {
                    getPlayScene()->BulletGroup->RemoveObject(objectIterator);
                    return;
                }
            }
            if (army->Size.x <= 300) {
                army->Size.x *= 1.005;
                army->Size.y *= 1.005;
                army->speed *= 0.98;
                army->CollisionRadius *= 1.005;
            }
            else {
                army->Hit(INT_MAX);
            }

            
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }
    // Check if out of boundary.
    if (Position.x < 0 || Position.x > PlayScene::GetClientSize().x || Position.y < 0 || Position.y > PlayScene::GetClientSize().y)
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}

multi_turrent_Bullet::multi_turrent_Bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent) :
    Bullet("play/missile.png", 300, 10, position, forwardDirection, rotation, parent) {
}

void multi_turrent_Bullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();

    for (auto& it : scene->ArmyGroup->GetObjects()) {
        Army* army = dynamic_cast<Army*>(it);
        if (!army->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, army->Position, army->CollisionRadius)) {
            army->Hit(damage);
            getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect2(army->Position.x, army->Position.y));
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }
    // Check if out of boundary.
    if (Position.x < 0 || Position.x > PlayScene::GetClientSize().x || Position.y < 0 || Position.y > PlayScene::GetClientSize().y)
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}

wizard_bullet::wizard_bullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Defense* parent) :
    Bullet("play/ice_ball.png", 300, 5, position, forwardDirection, rotation, parent) {
}

void wizard_bullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();

    for (auto& it : scene->ArmyGroup->GetObjects()) {
        Army* army = dynamic_cast<Army*>(it);
        if (!army->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, army->Position, army->CollisionRadius)) {
            
            getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect3(army->Position.x, army->Position.y));
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            AudioHelper::PlayAudio("wizard_explode.wav");
            return;
        }
    }
    // Check if out of boundary.
    if (Position.x < 0 || Position.x > PlayScene::GetClientSize().x || Position.y < 0 || Position.y > PlayScene::GetClientSize().y)
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}
