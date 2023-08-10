#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "PikaArmy.hpp"
#include "AudioHelper.hpp"
//#include "ArmyBullet.hpp"
#include "Group.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Army.hpp"
#include "PikaBullet.hpp"
#include "PikachuDie.hpp"
#include "Defense.hpp"

//Army(std::string img, float x, float y, float radius, float coolDown, float speed, float hp, int id, float shootRadius);
PikaArmy::PikaArmy(int x, int y) : Army("play/Pika-2.png", x, y, 20, 2, 55, 100, 2, PlayScene::BlockSize) {
    // Use bounding circle to detect collision is for simplicity, pixel-perfect collision can be implemented quite easily,
    // and efficiently if we use AABB collision detection first, and then pixel-perfect collision.
    

}

void PikaArmy::CreateBullet(Engine::Point pt) {
    int dx = pt.x - Position.x;
    int dy = pt.y - Position.y;
    double len = sqrt(pow(dx, 2) + pow(dy, 2));
    Engine::Point diff = Engine::Point(dx / len, dy / len);
    Engine::Point rotateDiff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(rotateDiff.y, rotateDiff.x) + ALLEGRO_PI / 2;
    getPlayScene()->BulletGroup->AddNewObject(new PikaBullet(Position, diff, rotation, this));
    AudioHelper::PlayAudio("missile.wav");
}
void PikaArmy::Hit(float damage) {
    HP -= damage;
    if (HP <= 0) {
        Die();
        // Remove all Defense's reference to target.
        for (auto& it : lockedDefenses)
            it->Target = nullptr;

        getPlayScene()->ArmyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("PikaDieSound.wav");

    }
}
void PikaArmy::Die() {
    getPlayScene()->EffectGroup->AddNewObject(new PikachuDie(Position.x, Position.y));
}

