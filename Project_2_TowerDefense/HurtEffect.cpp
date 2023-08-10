#include <cmath>
#include <string>

#include "HurtEffect.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include "AudioHelper.hpp"
// TODO 3 (2/2): You can imitate the 2 files: '"ExplosionEffect.hpp', '"ExplosionEffect.cpp' to create a Shoot Effect.
PlayScene* HurtEffect::getPlayScene() {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
HurtEffect::HurtEffect(float x, float y) : Sprite("play/SionHurt.png", x, y, 200, 200), timeTicks(0) {
	for (int i = 1; i <= 1; i++) {
		bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/SionHurt.png"));
	}
}
void HurtEffect::Update(float deltaTime) {
	timeTicks += deltaTime;
	if (timeTicks >= timeSpan) {

		getPlayScene()->EffectGroup->RemoveObject(objectIterator);
		return;
	}
	int phase = floor(timeTicks / timeSpan * bmps.size());
	bmp = bmps[phase];
	Sprite::Update(deltaTime);
}
