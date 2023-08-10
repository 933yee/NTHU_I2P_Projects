#ifndef sion_death_effect_HPP
#define sion_death_effect_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class PlayScene;

class sion_death_effect : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.1;
public:
	sion_death_effect(float x, float y);
	void Update(float deltaTime) override;
};
#endif // sion_death_effect_HPP
