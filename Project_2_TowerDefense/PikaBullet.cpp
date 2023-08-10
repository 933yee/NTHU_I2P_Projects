#include <allegro5/base.h>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>

#include "DirtyEffect.hpp"
#include "PikaBullet.hpp"
#include "Group.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Defense.hpp"
#include "ThunderEffect.hpp"
#include "Collider.hpp"
class Defense;

const float BlockSize = 64.0;
const float dir[8][2] = { {1.0, 0.0}, {-1.0, 0.0}, {0.0, 1.0}, {0.0, -1.0} , {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };
const float dir2[4][2] = { {2.0, 0.0}, {-2.0, 0.0}, {0.0, 2.0}, {0.0, -2.0}};
PikaBullet::PikaBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Army* parent) :
    ArmyBullet("play/Rasengan.png", 100, 3, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
}

void PikaBullet::OnExplode(Defense* defense) {
    getPlayScene()->EffectGroup->AddNewObject(new ThunderEffect(Position.x, Position.y));
}

void PikaBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();
    // Can be improved by Spatial Hash, Quad Tree, ...
    // However simply loop through all enemies is enough for this program.
    for (auto& it : scene->DefenseGroup->GetObjects()) {
        Defense* defense = dynamic_cast<Defense*>(it);
        if (defense->id == 7 && defense->cooled == false) continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, defense->Position, defense->CollisionRadius)) {
            for (auto& it2 : scene->DefenseGroup->GetObjects()) {
                Defense* defense_beside = dynamic_cast<Defense*>(it2);
                for (int i = 0; i < 8; i++) {
                    float X = defense->Position.x + dir[i][0] * BlockSize, Y = defense->Position.y + dir[i][1] * BlockSize;
                    if (defense_beside->Position.x == X && defense_beside->Position.y == Y) {
                        getPlayScene()->EffectGroup->AddNewObject(new ThunderEffect(X, Y));
                        defense_beside->Hit(damage/2);
                    }        
                }
                for (int i = 0; i < 4; i++) {
                    float X = defense->Position.x + dir2[i][0] * BlockSize, Y = defense->Position.y + dir2[i][1] * BlockSize;
                    if (defense_beside->Position.x == X && defense_beside->Position.y == Y) {
                        getPlayScene()->EffectGroup->AddNewObject(new ThunderEffect(X, Y));
                        defense_beside->Hit(damage / 2);
                    }
                }
            }
            
            OnExplode(defense);
            defense->Hit(damage);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }
    for (auto& it : scene->WallGroup->GetObjects()) {
        Defense* defense = dynamic_cast<Defense*>(it);
        if (defense->id == 7) continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, defense->Position, defense->CollisionRadius)) {

            OnExplode(defense);
            defense->Hit(damage);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }

    // Check if out of boundary.
    if (Position.x < 0 || Position.x > PlayScene::GetClientSize().x || Position.y < 0 || Position.y > PlayScene::GetClientSize().y)
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}