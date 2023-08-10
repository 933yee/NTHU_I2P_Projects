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
#include "sion.hpp"
#include "Defense.hpp"
#include "HurtEffect.hpp"
#include "sionBullet.hpp"
#include "sion_death_effect.hpp"
#include "PikachuDie.hpp"
#include "doge_tank.hpp"
#include "ThunderEffect.hpp"
#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_LEFT 2
#define BOTTOM_RIGHT 3

#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3
//Army(std::string img, float x, float y, float radius, float coolDown, float speed, float hp, int id, float shootRadius);
doge_tank::doge_tank(int x, int y) : Army("play/doge_tank.png", x, y, 30, 0, 100, 20, 4, 64) {
    // Use bounding circle to detect collision is for simplicity, pixel-perfect collision can be implemented quite easily,
    // and efficiently if we use AABB collision detection first, and then pixel-perfect collision.
}
doge_tank::doge_tank(float x, float y, int revive) :
    Army("play/doge_tank-4.png", x, y, 60, 0, 25, 20, 4, 64, 200, 200), death_time(revive) {
    // Move center downward, since we the army head is slightly biased upward.
}

void doge_tank::CreateBullet(Engine::Point pt) {   
}

void doge_tank::Hit(float damage) {
    HP -= damage;
    if (HP <= 0) {
        if (death_time == 0) {
            getPlayScene()->EffectGroup->AddNewObject(new smoke(Position.x, Position.y));       
            death_time++;
            Army* second_live = new doge_tank(0, 0, 2);
            second_live->Tint = al_map_rgba(255, 255, 255, 0);
            bmp = second_live->bmp;
            second_live->Visible = false;
            
            HP = 9999;
            speed = 0;
            Size.x = 200;
            Size.y = 200;
            Tint = al_map_rgba(255, 255, 255, 255);
            getPlayScene()->ArmyGroup->AddNewObject(second_live);
            // To keep responding when paused.
            second_live->Update(0);
            second_live->Hit(INT_MAX);
            return;
        }
        for (auto& it : lockedDefenses)
            it->Target = nullptr;
        
        getPlayScene()->ArmyGroup->RemoveObject(objectIterator);

    }
}

void doge_tank::Update(float deltaTime) {
    if (death_time == 1) {
        timeTicks += deltaTime;
        if (timeTicks >= timeSpan) {
            getPlayScene()->EffectGroup->AddNewObject(new smoke(Position.x, Position.y));
            Hit(INFINITY);
            
            return;
        }
    }
    // PlayScene
    PlayScene* scene = getPlayScene();

    const int distThreshold = PlayScene::BlockSize / 32;

    if (isPreview) return;

    // position
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));

    if (region == 5) {
        if (!Target) {
            // Lock closet target
            // Can be improved by Spatial Hash, Quad Tree, ...
            // However simply loop through all enemies is enough for this program.
            int maxDis = INT_MAX;
            Defense* tgt = nullptr;
            for (auto& it : scene->DefenseGroup->GetObjects()) {
                Defense* defense = dynamic_cast<Defense*>(it);
                if (defense->id == 7 && defense->cooled == false) continue;
                int dis = ManHattanDistance(it->Position);
                if (dis < maxDis) {
                    maxDis = dis;
                    tgt = dynamic_cast<Defense*>(it);
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
            reload -= deltaTime;
            if (reload <= 0) {
                // shoot.
                reload = coolDown;
                if (InShootingRange(Target->Position))
                    CreateBullet(Target->Position);
            }
            if (InShootingRange(Target->Position)) {
                Velocity = Engine::Point(0, 0);
            }
            else {
                float tx = Target->Position.x;
                float ty = Target->Position.y;

                if (dir == 1) {
                    if (abs(tx - Position.x) > distThreshold) {
                        Velocity = (Position.x < tx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                    }
                    else {
                        Position.x = tx;
                        Velocity = (Position.y < ty) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                    }
                }
                else {
                    if (abs(ty - Position.y) > distThreshold) {
                        Velocity = (Position.y < ty) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                    }
                    else {
                        Position.y = ty;
                        Velocity = (Position.x < tx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                    }
                }
            }
        }
    }
    else { // region != 5
        CalcRegion(x, y);
        if (!movingToWall) {
            // top
            if (region == 1 || region == 2 || region == 3) {
                if (!scene->brokenWall[UP].empty()) {
                    movingToWall = true;
                    int minDis = INT_MAX;
                    for (auto wall : scene->brokenWall[UP]) {
                        int dis = ManHattanDistance(wall * PlayScene::BlockSize);
                        if (minDis > dis) {
                            minDis = dis;
                            wallPos = wall * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
                        }
                    }
                    dir = 0;
                }
            }
            // down
            if (region == 7 || region == 8 || region == 9) {
                if (!scene->brokenWall[DOWN].empty()) {
                    movingToWall = true;
                    int minDis = INT_MAX;
                    for (auto wall : scene->brokenWall[DOWN]) {
                        int dis = ManHattanDistance(wall * PlayScene::BlockSize);
                        if (minDis > dis) {
                            minDis = dis;
                            wallPos = wall * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
                        }
                    }
                    dir = 0;
                }
            }
            // left
            if (region == 1 || region == 4 || region == 7) {
                if (!scene->brokenWall[LEFT].empty()) {
                    movingToWall = true;
                    int minDis = INT_MAX;
                    for (auto wall : scene->brokenWall[LEFT]) {
                        int dis = ManHattanDistance(wall * PlayScene::BlockSize);
                        if (minDis > dis) {
                            minDis = dis;
                            wallPos = wall * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
                        }
                    }
                    dir = 1;
                }
            }
            // right
            if (region == 3 || region == 6 || region == 9) {
                if (!scene->brokenWall[RIGHT].empty()) {
                    movingToWall = true;
                    int minDis = INT_MAX;
                    for (auto wall : scene->brokenWall[RIGHT]) {
                        int dis = ManHattanDistance(wall * PlayScene::BlockSize);
                        if (minDis > dis) {
                            minDis = dis;
                            wallPos = wall * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
                        }
                    }
                    dir = 1;
                }
            }
        }

        // there's no broken wall
        if (!movingToWall) {
            if (!Target) {
                // Lock closet wall
                // Can be improved by Spatial Hash, Quad Tree, ...
                // However simply loop is enough for this program.
                int maxDis = INT_MAX;
                Defense* tgt = nullptr;
                for (auto& it : scene->WallGroup->GetObjects()) {
                    int dis = ManHattanDistance(it->Position);
                    if (dis < maxDis) {
                        maxDis = dis;
                        tgt = dynamic_cast<Defense*>(it);
                    }
                }
                if (tgt) {
                    Target = tgt;
                    Target->lockedArmies.push_back(this);
                    lockedArmyIterator = std::prev(Target->lockedArmies.end());
                }
            }
            else {
                Rotation = UpdateRotation(deltaTime, Target->Position);

                // Shoot reload.
                reload -= deltaTime;
                if (reload <= 0) {
                    // shoot.
                    reload = coolDown;
                    if (InShootingRange(Target->Position))
                        CreateBullet(Target->Position);
                }
                if (InShootingRange(Target->Position)) {
                    Velocity = Engine::Point(0, 0);
                }
                else {
                    float tx = Target->Position.x;
                    float ty = Target->Position.y;

                    if (dir == 0) {
                        if (abs(tx - Position.x) > distThreshold) {
                            Velocity = (Position.x < tx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                        }
                        else {
                            Position.x = tx;
                            Velocity = (Position.y < ty) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                        }
                    }
                    else {
                        if (abs(ty - Position.y) > distThreshold) {
                            Velocity = (Position.y < ty) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                        }
                        else {
                            Position.y = ty;
                            Velocity = (Position.x < tx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                        }
                    }
                }
            }
        }
        else {
            if (Target) {
                Target = nullptr;
            }
            Rotation = UpdateRotation(deltaTime, wallPos);
            // Shoot reload.
            reload = coolDown;
            // position
            float wx = wallPos.x;
            float wy = wallPos.y;

            if (abs(wx - Position.x) < distThreshold && abs(wy - Position.y) < distThreshold) {
                Position = wallPos;
                movingToWall = false;
                region = 5;
                Velocity = Engine::Point(0, 0);
            }
            else {
                if (dir == 0) {
                    if (abs(wx - Position.x) > distThreshold) {
                        Velocity = (Position.x < wx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                    }
                    else {
                        Position.x = wx;
                        Velocity = (Position.y < wy) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                    }
                }
                else {
                    if (abs(wy - Position.y) > distThreshold) {
                        Velocity = (Position.y < wy) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                    }
                    else {
                        Position.y = wy;
                        Velocity = (Position.x < wx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                    }
                }
            }
        }
    }
    Velocity = Velocity * speed;
    Position.x += Velocity.x * deltaTime;
    Position.y += Velocity.y * deltaTime;
}


priest::priest(int x, int y) : Army("play/priest.png", x, y, 30, 0, 40, 30, 5, 200, 100, 100) {

}

void priest::CreateBullet(Engine::Point pt) {
}

void priest::Hit(float damage) {
    HP -= damage;
    if (HP <= 0) {
        getPlayScene()->EffectGroup->AddNewObject(new priest_die(Position.x,Position.y));
        for (auto& it : lockedDefenses)
            it->Target = nullptr;

        getPlayScene()->ArmyGroup->RemoveObject(objectIterator);

    }
}

void priest::Update(float deltaTime) {
    if (isPreview) return;
    timeTicks += deltaTime;
    
    if (timeTicks >= CD) {
        getPlayScene()->EffectGroup->AddNewObject(new star(Position.x, Position.y));
        CD += 7;
        PlayScene* scene = getPlayScene();
        for (auto& it : scene->ArmyGroup->GetObjects()) {
            Army* army = dynamic_cast<Army*>(it);
            getPlayScene()->EffectGroup->AddNewObject(new heal(army->Position.x, army->Position.y));
            
            army->Hit(-5);
            army->reload = 0;
        }
    }
    // PlayScene
    PlayScene* scene = getPlayScene();

    const int distThreshold = PlayScene::BlockSize / 32;

    

    // position
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));

    if (region == 5) {
        if (!Target) {
            // Lock closet target
            // Can be improved by Spatial Hash, Quad Tree, ...
            // However simply loop through all enemies is enough for this program.
            int maxDis = INT_MAX;
            Defense* tgt = nullptr;
            for (auto& it : scene->DefenseGroup->GetObjects()) {
                Defense* defense = dynamic_cast<Defense*>(it);
                if (defense->id == 7 && defense->cooled == false) continue;
                int dis = ManHattanDistance(it->Position);
                if (dis < maxDis) {
                    maxDis = dis;
                    tgt = dynamic_cast<Defense*>(it);
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
            reload -= deltaTime;
            if (reload <= 0) {
                // shoot.
                reload = coolDown;
                if (InShootingRange(Target->Position))
                    CreateBullet(Target->Position);
            }
            if (InShootingRange(Target->Position)) {
                Velocity = Engine::Point(0, 0);
            }
            else {
                float tx = Target->Position.x;
                float ty = Target->Position.y;

                if (dir == 1) {
                    if (abs(tx - Position.x) > distThreshold) {
                        Velocity = (Position.x < tx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                    }
                    else {
                        Position.x = tx;
                        Velocity = (Position.y < ty) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                    }
                }
                else {
                    if (abs(ty - Position.y) > distThreshold) {
                        Velocity = (Position.y < ty) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                    }
                    else {
                        Position.y = ty;
                        Velocity = (Position.x < tx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                    }
                }
            }
        }
    }
    else { // region != 5
        CalcRegion(x, y);
        if (!movingToWall) {
            // top
            if (region == 1 || region == 2 || region == 3) {
                if (!scene->brokenWall[UP].empty()) {
                    movingToWall = true;
                    int minDis = INT_MAX;
                    for (auto wall : scene->brokenWall[UP]) {
                        int dis = ManHattanDistance(wall * PlayScene::BlockSize);
                        if (minDis > dis) {
                            minDis = dis;
                            wallPos = wall * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
                        }
                    }
                    dir = 0;
                }
            }
            // down
            if (region == 7 || region == 8 || region == 9) {
                if (!scene->brokenWall[DOWN].empty()) {
                    movingToWall = true;
                    int minDis = INT_MAX;
                    for (auto wall : scene->brokenWall[DOWN]) {
                        int dis = ManHattanDistance(wall * PlayScene::BlockSize);
                        if (minDis > dis) {
                            minDis = dis;
                            wallPos = wall * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
                        }
                    }
                    dir = 0;
                }
            }
            // left
            if (region == 1 || region == 4 || region == 7) {
                if (!scene->brokenWall[LEFT].empty()) {
                    movingToWall = true;
                    int minDis = INT_MAX;
                    for (auto wall : scene->brokenWall[LEFT]) {
                        int dis = ManHattanDistance(wall * PlayScene::BlockSize);
                        if (minDis > dis) {
                            minDis = dis;
                            wallPos = wall * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
                        }
                    }
                    dir = 1;
                }
            }
            // right
            if (region == 3 || region == 6 || region == 9) {
                if (!scene->brokenWall[RIGHT].empty()) {
                    movingToWall = true;
                    int minDis = INT_MAX;
                    for (auto wall : scene->brokenWall[RIGHT]) {
                        int dis = ManHattanDistance(wall * PlayScene::BlockSize);
                        if (minDis > dis) {
                            minDis = dis;
                            wallPos = wall * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
                        }
                    }
                    dir = 1;
                }
            }
        }

        // there's no broken wall
        if (!movingToWall) {
            if (!Target) {
                // Lock closet wall
                // Can be improved by Spatial Hash, Quad Tree, ...
                // However simply loop is enough for this program.
                int maxDis = INT_MAX;
                Defense* tgt = nullptr;
                for (auto& it : scene->WallGroup->GetObjects()) {
                    int dis = ManHattanDistance(it->Position);
                    if (dis < maxDis) {
                        maxDis = dis;
                        tgt = dynamic_cast<Defense*>(it);
                    }
                }
                if (tgt) {
                    Target = tgt;
                    Target->lockedArmies.push_back(this);
                    lockedArmyIterator = std::prev(Target->lockedArmies.end());
                }
            }
            else {
                Rotation = UpdateRotation(deltaTime, Target->Position);

                // Shoot reload.
                reload -= deltaTime;
                if (reload <= 0) {
                    // shoot.
                    reload = coolDown;
                    if (InShootingRange(Target->Position))
                        CreateBullet(Target->Position);
                }
                if (InShootingRange(Target->Position)) {
                    Velocity = Engine::Point(0, 0);
                }
                else {
                    float tx = Target->Position.x;
                    float ty = Target->Position.y;

                    if (dir == 0) {
                        if (abs(tx - Position.x) > distThreshold) {
                            Velocity = (Position.x < tx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                        }
                        else {
                            Position.x = tx;
                            Velocity = (Position.y < ty) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                        }
                    }
                    else {
                        if (abs(ty - Position.y) > distThreshold) {
                            Velocity = (Position.y < ty) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                        }
                        else {
                            Position.y = ty;
                            Velocity = (Position.x < tx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                        }
                    }
                }
            }
        }
        else {
            if (Target) {
                Target = nullptr;
            }
            Rotation = UpdateRotation(deltaTime, wallPos);
            // Shoot reload.
            reload = coolDown;
            // position
            float wx = wallPos.x;
            float wy = wallPos.y;

            if (abs(wx - Position.x) < distThreshold && abs(wy - Position.y) < distThreshold) {
                Position = wallPos;
                movingToWall = false;
                region = 5;
                Velocity = Engine::Point(0, 0);
            }
            else {
                if (dir == 0) {
                    if (abs(wx - Position.x) > distThreshold) {
                        Velocity = (Position.x < wx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                    }
                    else {
                        Position.x = wx;
                        Velocity = (Position.y < wy) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                    }
                }
                else {
                    if (abs(wy - Position.y) > distThreshold) {
                        Velocity = (Position.y < wy) ? Engine::Point(0, 1) : Engine::Point(0, -1);
                    }
                    else {
                        Position.y = wy;
                        Velocity = (Position.x < wx) ? Engine::Point(1, 0) : Engine::Point(-1, 0);
                    }
                }
            }
        }
    }
    Velocity = Velocity * speed;
    Position.x += Velocity.x * deltaTime;
    Position.y += Velocity.y * deltaTime;
}