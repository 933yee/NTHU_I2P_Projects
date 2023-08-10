#ifndef THUNDEREFFECT_HPP
#define THUNDEREFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Sprite.hpp"
#include "Army.hpp"

class PlayScene;

class ThunderEffect : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.5;
public:
	ThunderEffect(float x, float y);
	void Update(float deltaTime) override;
};

class PlayScene;

class fire_explosion_effect : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.5;
public:
	fire_explosion_effect(float x, float y);
	void Update(float deltaTime) override;
};

class frozen_effect : public Engine::Sprite{
protected:
	Defense* frozed = nullptr;
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 2;
public:
	frozen_effect(float x, float y);
	void Update(float deltaTime) override;
};

class ice_explode_effect : public Engine::Sprite {
protected:
	float rec_cooldown;
	ALLEGRO_COLOR rec_tint;
	Defense* frozed = nullptr;
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.5;
public:
	ice_explode_effect(float x, float y);
	void Update(float deltaTime) override;
};

class mines_explosion_effect : public Engine::Sprite {
protected:
	float rec_cooldown;
	ALLEGRO_COLOR rec_tint;
	Defense* frozed = nullptr;
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.8;
public:
	mines_explosion_effect(float x, float y);
	void Update(float deltaTime) override;
};

class smoke : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.5;
public:
	smoke(float x, float y);
	void Update(float deltaTime) override;
};


class nothing : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 8;
public:
	Army* target;
	nothing(float x, float y, Army*);
	void Update(float deltaTime) override;
};

class heal : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.5;
public:
	Army* target;
	heal(float x, float y);
	void Update(float deltaTime) override;
};
class star : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 0.3;
public:
	Army* target;
	star(float x, float y);
	void Update(float deltaTime) override;
};
class priest_die : public Engine::Sprite {
protected:
	PlayScene* getPlayScene();
	float timeTicks;
	std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
	float timeSpan = 1.5;
public:
	Army* target;
	priest_die(float x, float y);
	void Update(float deltaTime) override;
};
#endif // THUNDERFFECT_HPP
