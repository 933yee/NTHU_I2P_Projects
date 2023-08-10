#ifndef EXPLOSIONEFFECT_HPP
#define EXPLOSIONEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"
#include "Army.hpp"

class PlayScene;

class ExplosionEffect : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.5;
public:
	ExplosionEffect(float x, float y);
	void Update(float deltaTime) override;
};

class ExplosionEffect2 : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.8;
public:
	ExplosionEffect2(float x, float y);
	void Update(float deltaTime) override;
};

class ExplosionEffect3 : public Engine::Sprite {
protected:
	
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 2;
public:
	Engine::Image* rec;
	Army* frozed = nullptr;
	int key = 0;
	ExplosionEffect3(float x, float y);
	void Update(float deltaTime) override;
};
#endif // EXPLOSIONEFFECT_HPP
