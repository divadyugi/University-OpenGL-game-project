//first create a player that just walks
#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h"
#include "engine/entities/Bounding_sphere.h"

class Gun;
class Bullet;


class Player : public engine::game_object
{

public:
	//Constructor
	Player(const engine::game_object_properties props);

	void init(std::vector<engine::ref<Gun>> weapons, float speed, std::vector<engine::ref<Bullet>> bullets);

	void update(float ts, engine::perspective_camera& camera, engine::ref<engine::audio_manager> audio_manager);

	void on_render(const engine::ref<engine::shader>& shader);


	//Movement function
	void move(float ts);

	//Weapon related functions
	void shoot(engine::perspective_camera& camera);

	void switchWeapon();

	void reload();

	void attackTimer(float ts);

	void reloadTimer(float ts);

	//Physics
	//Ground Collission detection
	bool ground_collision(float y_plane);
	//Ground collission response
	void ground_response(float y_pos);

	void ramp_response(const glm::vec3& height);

	void take_damage(float damage, const glm::vec3& push_back, float force);

	//Getters and setters 

	engine::ref<Gun> current_weapon() { return m_current_weapon; }


	void set_grounded(bool grounded) { m_is_grounded = grounded; }

	engine::bounding_box player_collider() { return m_player_box; }

	//engine::Bounding_sphere player_sphere() { return m_player_sphere; }


	static engine::ref<Player> create(const engine::game_object_properties& props);

	//Stat getters and setters
	float speed() { return m_speed; }
	int health() { return m_health; }
	int score() { return m_score; }
	int coins() { return m_coins; }
	int damage() { return m_damage; }

	void setSpeed(float speed) { m_speed = speed; }
	void setHealth(int health) { m_health = health; }
	void setScore(int score) { m_score = score; }
	void setCoins(int coins) { m_coins = coins; }
	void setDamage(int damage) { m_damage = damage; }

private:

	//Stats
	float m_speed = 5;

	int m_health = 100;

	int m_score = 0;

	int m_coins = 0;

	int m_damage = 5;

	//A list of all the guns the player has

	std::vector<engine::ref<Gun>> m_weapons;

	//The currently equipped gun
	engine::ref<Gun> m_current_weapon;

	bool m_can_attack = true;

	bool m_reloading = false;

	//Physics
	bool m_is_grounded = true;

	bool m_is_jumping = false;

	bool m_can_jump = false;

	float m_push_back_timer = 0.4f;

	bool m_push_back = false;


	//Collision
	engine::bounding_box m_player_box;

	//engine::Bounding_sphere m_player_sphere;

	std::vector<engine::ref<Bullet>> m_bullets{};
};
