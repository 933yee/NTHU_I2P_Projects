#include <cmath>
#include <string>

#include "sion_death_effect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include "AudioHelper.hpp"
// TODO 3 (2/2): You can imitate the 2 files: '"ExplosionEffect.hpp', '"ExplosionEffect.cpp' to create a Shoot Effect.
PlayScene* sion_death_effect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
sion_death_effect::sion_death_effect(float x, float y) : Sprite("play/sion_death_effect1.png", x, y, 100, 100), timeTicks(0) {
	for (int i = 1; i <= 2; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/sion_death_effect" + std::to_string(i) +".png"));
	}
}
void sion_death_effect::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {

		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}
