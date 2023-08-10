#include <cmath>
#include <string>

#include "ExplosionEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include "AudioHelper.hpp"
#include "Collider.hpp"
#include "Army.hpp"
#include "Point.hpp"
#include "IceArmy.hpp"
#include "Defense.hpp"
#include "ThunderEffect.hpp"
#include "sion.hpp"
#include "doge_tank.hpp"
// TODO 3 (2/2): You can imitate the 2 files: '"ExplosionEffect.hpp', '"ExplosionEffect.cpp' to create a Shoot Effect.
PlayScene* ExplosionEffect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
ExplosionEffect::ExplosionEffect(float x, float y) : Sprite("play/explosion-1.png", x, y), timeTicks(0) {
	for (int i = 1; i <= 5; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/explosion-" + std::to_string(i) + ".png"));
	}
}
void ExplosionEffect::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}

PlayScene* ExplosionEffect2::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
ExplosionEffect2::ExplosionEffect2(float x, float y) : Sprite("play/explosion-6.png", x, y, 100, 100), timeTicks(0) {
	for (int i = 6; i <= 10; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/explosion-" + std::to_string(i) + ".png"));
	}
}
void ExplosionEffect2::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}

PlayScene* ExplosionEffect3::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
ExplosionEffect3::ExplosionEffect3(float x, float y) : Sprite("play/wizard_effect1.png", x, y, 150, 150), timeTicks(0) {

	PlayScene* scene = getPlayScene();
	for (auto& it : scene->ArmyGroup->GetObjects()) {
		Army* army = dynamic_cast<Army*>(it);
		if (army->Position.x == x && army->Position.y == y) {
			
			army->Hit(5);
			if (army->id == 3) {
				sion* tmp = dynamic_cast<sion*>(army);
				if (tmp->death_time == 1) continue;
			}
			if (army->id == 4) {
				doge_tank* tmp = dynamic_cast<doge_tank*>(army);
				if (tmp->death_time == 1) continue;
			}
			frozed = army;
			int random = (rand() % 10) * rand() % 10 ;
			if (random == 0 && frozed->Enabled == true) {
				frozed->Enabled = false;
				frozed->Tint = al_map_rgba(0, 255, 255, 255);
				scene->AddNewObject(rec = new Engine::Image("play/cold.png", x-frozed->Size.x, y- frozed->Size.x, frozed->Size.x*2, frozed->Size.y*2));
				key = 1;
			}	
			break;
		}
	}
	for (int i = 1; i <= 14; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/wizard_effect" + std::to_string(i) + ".png"));
	}
}
void ExplosionEffect3::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		if (frozed && key) {
			getPlayScene()->RemoveObject(rec->GetObjectIterator());
			if (frozed->id == 3) {
				sion* tmp = dynamic_cast<sion*>(frozed);
				if (tmp->death_time == 1) {
					getPlayScene()->EffectGroup->RemoveObject(objectIterator);
					return;
				}
			}
			if (frozed->id == 4) {
				doge_tank* tmp = dynamic_cast<doge_tank*>(frozed);
				if (tmp->death_time == 1) {
					getPlayScene()->EffectGroup->RemoveObject(objectIterator);
					return;
				}
			}
			frozed->Tint = al_map_rgba(0, 182, 255, 255);
			frozed->Enabled = true;
			frozed->speed = 25;
			
		}
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}