#ifndef HURTEFFECT_HPP
#define HURTEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class PlayScene;

class HurtEffect : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.05;
public:
	HurtEffect(float x, float y);
	void Update(float deltaTime) override;
};
#endif // HURTEFFECT_HPP
