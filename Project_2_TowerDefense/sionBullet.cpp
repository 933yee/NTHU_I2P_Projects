#include <allegro5/base.h>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "DirtyEffect.hpp"
#include "sionBullet.hpp"
#include "Group.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Defense.hpp"
#include "ThunderEffect.hpp"
#include "Collider.hpp"
class Defense;

const float BlockSize = 64.0;
const float dir[4][2] = { {1.0, 0.0}, {-1.0, 0.0}, {0.0, 1.0}, {0.0, -1.0} };
sionBullet::sionBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Army* parent) :
    ArmyBullet("play/fireball.png", 400, 3, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
}

void sionBullet::OnExplode(Defense* defense) {
    getPlayScene()->EffectGroup->AddNewObject(new fire_explosion_effect(defense->Position.x, defense->Position.y));
}

void sionBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();
    // Can be improved by Spatial Hash, Quad Tree, ...
    // However simply loop through all enemies is enough for this program.
    for (auto& it : scene->DefenseGroup->GetObjects()) {
        Defense* defense = dynamic_cast<Defense*>(it);
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, defense->Position, defense->CollisionRadius)) {
            
            auto k = rec_object_alive.find(defense->Position.x);
            if (k != rec_object_alive.end()) {
                int t = rec_object_alive_son.count(defense->Position.y);
                if (t == 0) {
                    OnExplode(defense);
                    defense->Hit(damage);
                }
            }else {
                OnExplode(defense);
                defense->Hit(damage);
            }
            rec_object_alive_son.insert({ defense->Position.y});
            rec_object_alive.insert({ defense->Position.x, rec_object_alive_son });
            
            return;
        }
    }
    for (auto& it : scene->WallGroup->GetObjects()) {
        Defense* defense = dynamic_cast<Defense*>(it);
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, defense->Position, defense->CollisionRadius)) {
            OnExplode(defense);
            defense->Hit(50);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }

    // Check if out of boundary.
    if (Position.x < 0 || Position.x > PlayScene::GetClientSize().x || Position.y < 0 || Position.y > PlayScene::GetClientSize().y)
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}