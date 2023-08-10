#ifndef PIKACHUDIE_HPP
#define PIKACHUDIE_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"

class PlayScene;

class PikachuDie: public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 2.0;
public:
	PikachuDie(float x, float y);
	void Update(float deltaTime) override;
};

class PlayScene;

class sionDie : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 1.8;
public:
	sionDie(float x, float y);
	void Update(float deltaTime) override;
};
#endif // PIKACHUDIE_HPP
