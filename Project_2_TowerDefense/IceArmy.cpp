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
#include "IceArmy.hpp"
#include "Defense.hpp"
#include "ThunderEffect.hpp"

const int blockSize = 64;
//Army(std::string img, float x, float y, float radius, float coolDown, float speed, float hp, int id, float shootRadius);
IceArmy::IceArmy(float x, float y) :
    Army("play/ice.png", x, y, 1, 0, 80, 1, 6, 0) {
    // Move center downward, since we the army head is slightly biased upward.
}
void IceArmy::Update(float deltaTime) {
    // PlayScene
   
}

// Since the Ice army cannot shoot, the function doesn't need to do anything.
void IceArmy::CreateBullet(Engine::Point pt) {}

// TODO 2 (5/8): You can imitate the hit function in Army class. Notice that the Ice army won't have explosion effect.
void IceArmy::Hit(float damage) {
    HP -= damage;
    if (HP <= 0) { 
        AudioHelper::PlayAudio("ice_explosion.wav");
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x, Position.y));
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x + blockSize, Position.y));
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x, Position.y + blockSize));
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x + blockSize, Position.y + blockSize));
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x - blockSize, Position.y));
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x, Position.y - blockSize));
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x - blockSize, Position.y - blockSize));
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x - blockSize, Position.y + blockSize));
        getPlayScene()->EffectGroup->AddNewObject(new frozen_effect(Position.x + blockSize, Position.y - blockSize));
        getPlayScene()->EffectGroup->AddNewObject(new ice_explode_effect(Position.x, Position.y));
        for (auto& it : lockedDefenses)
            it->Target = nullptr;

        getPlayScene()->ArmyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");
    }
}
