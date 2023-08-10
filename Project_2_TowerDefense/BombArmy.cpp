#include <allegro5/base.h>
#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <string>
#include <iostream>

#include "AudioHelper.hpp"
#include "Collider.hpp"
#include "Army.hpp"
#include "Group.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "BombArmy.hpp"
#include "Defense.hpp"


//Army(std::string img, float x, float y, float radius, float coolDown, float speed, float hp, int id, float shootRadius);
BombArmy::BombArmy(float x, float y) :
    Army("play/bombs.png", x, y, 20, 0, 80, 15, 1, 0) {
    // Move center downward, since we the army head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void BombArmy::Update(float deltaTime) {
    // PlayScene
    PlayScene* scene = getPlayScene();

    if (isPreview) return ;
    
    // position
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    const int distThreshold = PlayScene::BlockSize / 32;
    if (!Target) { 
        // Lock closet target
        // Can be improved by Spatial Hash, Quad Tree, ...
        // However simply loop through all enemies is enough for this program.
        int maxDis = INT_MAX;
        Defense* tgt = nullptr;
        for (auto& it : scene->WallGroup->GetObjects()) {
            target_is_wall = true;
            int dis = ManHattanDistance(it->Position);
            if (dis < maxDis) {
                maxDis = dis;
                tgt = dynamic_cast<Defense*>(it);
            }
        }
        if (!target_is_wall) {
            for (auto& it : scene->DefenseGroup->GetObjects()) {
                int dis = ManHattanDistance(it->Position);
                if (dis < maxDis) {
                    maxDis = dis;
                    tgt = dynamic_cast<Defense*>(it);
                }
            }
        }
        if (tgt) {
            Target = tgt;
            Target->lockedArmies.push_back(this);
            lockedArmyIterator = std::prev(Target->lockedArmies.end());
        }
    }
    if (Target) {
        Rotation = UpdateRotation(deltaTime, Target->Position);
        // Shoot reload.
        reload = coolDown;
        if (abs(Target->Position.x - Position.x) <= 1 && abs(Target->Position.y - Position.y) <= 1){
            // Notice that bomb army deals different damage to wall and normal target.
            
            if(target_is_wall)Target->Hit(INFINITY);
            else Target->Hit(10);
            target_is_wall = 0;
            Hit(INFINITY);

        }
        else {
            int dx = Target->Position.x - Position.x;
            int dy = Target->Position.y - Position.y;
            double len = sqrt(pow(dx, 2) + pow(dy, 2));
            Velocity = speed * Engine::Point(dx / len, dy / len);
        }
    }

    Position.x += Velocity.x * deltaTime;
    Position.y += Velocity.y * deltaTime;
}

// Since the bomb army cannot shoot, the function doesn't need to do anything.
void BombArmy::CreateBullet(Engine::Point pt) {}

// TODO 2 (5/8): You can imitate the hit function in Army class. Notice that the bomb army won't have explosion effect.
void BombArmy::Hit(float damage) {
    HP -= damage;
    if (HP <= 0) {     
        // Remove all Defense's reference to target.
        for (auto& it: lockedDefenses)
            it->Target = nullptr;
        
        getPlayScene()->ArmyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");
    }
}
