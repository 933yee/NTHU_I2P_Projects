#include <cmath>
#include <string>

#include "PikachuDie.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
// TODO 3 (2/2): You can imitate the 2 files: '"ExplosionEffect.hpp', '"ExplosionEffect.cpp' to create a Shoot Effect.
PlayScene* PikachuDie::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
PikachuDie::PikachuDie(float x, float y) : Sprite("play/PikaDizzy-1.png", x, y, 70.0, 70.0), timeTicks(0) {
	for (int i = 1; i <= 5; i++) {	
		if (i == 5) {
			for (int j = 0; j < 15; j++) {
				bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/PikaDizzy-" + std::to_string(i) + ".png"));
				bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/nothing.png"));
			}
		}
		else {
			bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/PikaDizzy-" + std::to_string(i) + ".png"));
		}
	}
}
void PikachuDie::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}


PlayScene* sionDie::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
sionDie::sionDie(float x, float y) : Sprite("play/sion_death_action1.png", x, y, 120.0, 120.0), timeTicks(0) {
	for (int i = 11; i <= 20; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/sion_death_action" + std::to_string(i) + ".png"));
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/sion_death_action" + std::to_string(i) + ".png"));
	}
	for (int i = 1; i <= 15; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/nothing.png"));
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/sion_death_action" + std::to_string(20) + ".png"));
		
	}

}
void sionDie::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {
		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}