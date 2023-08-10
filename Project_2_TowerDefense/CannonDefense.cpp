#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "CannonDefense.hpp"
#include "AudioHelper.hpp"
#include "CannonBullet.hpp"
#include "Group.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Army.hpp"
#include "ThunderEffect.hpp"

//Defense(std::string imgDefense, float x, float y, float radius, float coolDown, int hp, int id, float shootRadius);
CannonDefense::CannonDefense(float x, float y) :
    Defense("play/cannon.png", x, y, 20, 1, 50, 2, 425) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}

void CannonDefense::CreateBullet(Engine::Point pt) {
    int dx = pt.x - Position.x;
    int dy = pt.y - Position.y;
    double len = sqrt(pow(dx, 2) + pow(dy, 2));
    Engine::Point diff = Engine::Point(dx / len, dy / len);
    Engine::Point rotateDiff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(rotateDiff.y, rotateDiff.x) + ALLEGRO_PI / 2;
    getPlayScene()->BulletGroup->AddNewObject(new CannonBullet(Position , diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}

fat_cannon_Defense::fat_cannon_Defense(float x, float y) :
    Defense("play/fat_cannon.png", x, y, 20, 0.3, 30, 3, 500) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}

void fat_cannon_Defense::CreateBullet(Engine::Point pt) {
    int dx = pt.x - Position.x;
    int dy = pt.y - Position.y;
    double len = sqrt(pow(dx, 2) + pow(dy, 2));
    Engine::Point diff = Engine::Point(dx / len, dy / len);
    Engine::Point rotateDiff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(rotateDiff.y, rotateDiff.x) + ALLEGRO_PI / 2;
    int type_of_bullet = (rand()%3+ rand())%3+1;
    if (type_of_bullet == 1) {
        getPlayScene()->BulletGroup->AddNewObject(new fat_cannon_Bullet(Position, diff, rotation, this));
    }
    else if (type_of_bullet == 2) {
        getPlayScene()->BulletGroup->AddNewObject(new fat_cannon_Bullet(Position, diff, rotation, this, true));
    }
    else if(type_of_bullet == 3) {
        getPlayScene()->BulletGroup->AddNewObject(new fat_cannon_Bullet(Position, diff, rotation, this, true, true));
    }
   
    AudioHelper::PlayAudio("gun.wav");
}

multi_turrent_Defense::multi_turrent_Defense(float x, float y) :
    Defense("play/multi_turrent.png", x, y, 20, 4, 80, 4, 350) {
    // Move center downward, since we the turret head is slightly biased upward.
    this->Size.x = this->Size.y = 75;
}

void multi_turrent_Defense::CreateBullet(Engine::Point pt) {
    if (first == 1) {
        first = 0;
        return;
    }
    int dx = pt.x - Position.x;
    int dy = pt.y - Position.y;
    double len = sqrt(pow(dx, 2) + pow(dy, 2));
    Engine::Point diff = Engine::Point(dx / len, dy / len);
    Engine::Point diff2 = Engine::Point(-dx / len, -dy / len);
    Engine::Point diff3 = Engine::Point(-dy / len, dx / len);
    Engine::Point diff4 = Engine::Point(dy / len, -dx / len);
    Engine::Point rotateDiff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    Engine::Point rotateDiff2 = Engine::Point(cos(Rotation + ALLEGRO_PI / 2), sin(Rotation + ALLEGRO_PI / 2));
    Engine::Point rotateDiff3 = Engine::Point(cos(Rotation ), sin(Rotation ));
    Engine::Point rotateDiff4 = Engine::Point(cos(Rotation - ALLEGRO_PI), sin(Rotation - ALLEGRO_PI));
    float rotation = atan2(rotateDiff.y, rotateDiff.x) + ALLEGRO_PI / 2;
    float rotation2 = atan2(rotateDiff2.y, rotateDiff2.x) + ALLEGRO_PI / 2;
    float rotation3 = atan2(rotateDiff3.y, rotateDiff3.x) + ALLEGRO_PI / 2;
    float rotation4 = atan2(rotateDiff4.y, rotateDiff4.x) + ALLEGRO_PI / 2;
    getPlayScene()->BulletGroup->AddNewObject(new multi_turrent_Bullet(Position, diff, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new multi_turrent_Bullet(Position, diff2, rotation2, this));
    getPlayScene()->BulletGroup->AddNewObject(new multi_turrent_Bullet(Position, diff3, rotation3, this));
    getPlayScene()->BulletGroup->AddNewObject(new multi_turrent_Bullet(Position, diff4, rotation4, this));

    AudioHelper::PlayAudio("gun.wav");
}

mines_Defense::mines_Defense(float x, float y) :
    Defense("play/mines.png", x, y, 0, 0, 10, 7, 200) {
    // Move center downward, since we the turret head is slightly biased upward.
    Tint = al_map_rgba(255, 255, 255, 0);
}

void mines_Defense::CreateBullet(Engine::Point pt) {
}

void mines_Defense::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene* scene = getPlayScene();
    if (!Enabled)
        return;
    int flag = 0;
    for (auto& it : scene->ArmyGroup->GetObjects()) {
        int ey;
        
        ey = static_cast<int>(floor(it->Position.y / PlayScene::BlockSize));
        if (pow(abs(Position.x - it->Position.x), 2) + pow(abs(Position.y - it->Position.y), 2) <= pow(1, 2)) {
            getPlayScene()->EffectGroup->AddNewObject(new mines_explosion_effect(Position.x, Position.y));
            flag = 1;
            break;
        }
    }
    if (flag) {
        for (auto& it : scene->ArmyGroup->GetObjects()) {
           int ey;
            ey = static_cast<int>(floor(it->Position.y / PlayScene::BlockSize));
            if (InShootingRange(it->Position)) {
                Target = dynamic_cast<Army*>(it);
                
                Target->Hit(INFINITY);
                Target = nullptr;
            }
        }
        Hit(INFINITY);
    }
        
}


wizard::wizard(float x, float y) :
    Defense("play/wizard.png", x, y, 20, 3, 50, 5, 425) {
}

void wizard::CreateBullet(Engine::Point pt) {
    int dx = pt.x - Position.x;
    int dy = pt.y - Position.y;
    double len = sqrt(pow(dx, 2) + pow(dy, 2));
    Engine::Point diff = Engine::Point(dx / len, dy / len);
    Engine::Point rotateDiff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(rotateDiff.y, rotateDiff.x) + ALLEGRO_PI / 2;
    getPlayScene()->BulletGroup->AddNewObject(new wizard_bullet(Position, diff, rotation, this));
    AudioHelper::PlayAudio("wizard_shoot.wav");
}
