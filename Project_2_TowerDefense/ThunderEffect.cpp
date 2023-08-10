#include <cmath>
#include <string>
#include <allegro5/base.h>
#include <random>
#include <string>
#include <unordered_map>
#include <utility>

#include "DirtyEffect.hpp"
#include "PikaBullet.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Defense.hpp"
#include "ThunderEffect.hpp"
#include "Collider.hpp"
#include "ThunderEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "Resources.hpp"
#include "AudioHelper.hpp"
// TODO 3 (2/2): You can imitate the 2 files: '"ExplosionEffect.hpp', '"ExplosionEffect.cpp' to create a Shoot Effect.
PlayScene* ThunderEffect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
ThunderEffect::ThunderEffect(float x, float y) : Sprite("play/thunder-1.png", x, y, 100, 100), timeTicks(0) {
	AudioHelper::PlayAudio("ElectricitySound.wav");
	for (int i = 1; i <= 5; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/thunder-" + std::to_string(i) + ".png"));
	}
}
void ThunderEffect::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}


PlayScene* fire_explosion_effect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
fire_explosion_effect::fire_explosion_effect(float x, float y) : Sprite("play/fire_explosion_effect1.png", x, y, 100, 100), timeTicks(0) {
	
	for (int i = 1; i <= 4; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/fire_explosion_effect" + std::to_string(i) + ".png"));
	}
	bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/fire_explosion_effect" + std::to_string(4) + ".png"));

}
void fire_explosion_effect::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {

		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}


PlayScene* frozen_effect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
frozen_effect::frozen_effect(float x, float y) : Sprite("play/ice_melt (1).png", x, y, 72, 72), timeTicks(0) {
	this->Tint = al_map_rgba(255, 255, 255, 150);
	PlayScene* scene = getPlayScene();
	for (auto& it : scene->DefenseGroup->GetObjects()) {
		Defense* defense = dynamic_cast<Defense*>(it);	
		if (defense->Position.x == x && defense->Position.y == y) {
			frozed = defense;
			frozed->cooled = true;
			frozed->coolDown *= 1.5;
			frozed->Enabled = false;
			frozed->Tint = al_map_rgba(0, 150, 255, 255);
		}
	}
	for (int i = 1; i <= 12; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/ice_melt (" + std::to_string(i) + ").png"));
	}

}
void frozen_effect::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		if (frozed) {
			frozed->Enabled = true;
			frozed->Tint = al_map_rgba(0, 218, 228, 230);
		}	
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}


PlayScene* ice_explode_effect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
ice_explode_effect::ice_explode_effect(float x, float y) : Sprite("play/ice_explode1.png", x, y, 300, 300), timeTicks(0) {
	for (int i = 1; i <= 6; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/ice_explode" + std::to_string(i) + ".png"));
	}
}
void ice_explode_effect::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}
PlayScene* mines_explosion_effect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
mines_explosion_effect::mines_explosion_effect(float x, float y) : Sprite("play/mines_explosion1.png", x, y, 300, 300), timeTicks(0) {
	for (int i = 1; i <= 12; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/mines_explosion" + std::to_string(i) + ".png"));
	}
}
void mines_explosion_effect::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}


PlayScene* smoke::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
smoke::smoke(float x, float y) : Sprite("play/smoke/smoke (1).png", x, y, 300, 300), timeTicks(0) {
	
	for (int i = 1; i <= 17; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/smoke/smoke (" + std::to_string(i) + ").png"));
	}
}
void smoke::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {

		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}

PlayScene* nothing::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
nothing::nothing(float x, float y, Army* t) : Sprite("play/nothing.png", x, y, 1, 1), timeTicks(0) , target(t){
	bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/nothing.png"));
}
void nothing::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		target->Hit(INFINITY);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}

PlayScene* heal::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
heal::heal(float x, float y) : Sprite("play/heal(1).png", x, y, 100, 100), timeTicks(0) {
	for (int i = 1; i <= 6; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/heal(" + std::to_string(i) + ").png"));
	}
}
void heal::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {

		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}


PlayScene* star::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
star::star(float x, float y) : Sprite("play/star(1).png", x, y, 100, 100), timeTicks(0) {
	for (int i = 1; i <= 4; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/star(" + std::to_string(i) + ").png"));
	}
}
void star::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {

		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}

PlayScene* priest_die::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
priest_die::priest_die(float x, float y) : Sprite("play/priest.png", x, y, 100, 100), timeTicks(0) {
	for (int i = 1; i <= 5; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/priest.png"));
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/priest.png"));
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/nothing.png"));
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/nothing.png"));
	}
	for (int i = 1; i <= 10; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/priest.png"));
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/nothing.png"));
	}
}
void priest_die::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {

		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}