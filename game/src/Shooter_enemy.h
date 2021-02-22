#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h"

class Player;

class Shooter_enemy : public engine::game_object
{
public:
	enum class state
	{
		idle,
		chase,
		attack
	};

	Shooter_enemy(const engine::game_object_properties& props);

	~Shooter_enemy();


	void init(float speed, float damage, float chase_range, float attack_range);

	void on_render(const engine::ref<engine::shader>& shader);

	void on_update(float ts, const glm::vec3& target, engine::ref<Player> player);

	void chase(float ts, const glm::vec3& target);

	void reset();
	void lowerHealth(float damage) { m_health -= damage; }

	//Getters and setters
	bool active() { return m_active; }
	state current_state() { return m_state; }

	void set_attack(bool can_attack) { m_can_attack = can_attack; }

	bool can_attack() { return m_can_attack; }

	engine::bounding_box enemy_collider() { return m_enemy_collider; }

	static engine::ref<Shooter_enemy> create(const engine::game_object_properties& props);

private:

	//Movement speed of Ai
	float m_speed;

	//Damage of the AI
	float m_damage;

	//health of the enemy
	float m_health = 50.f;

	bool m_active = true;

	//range for chasing player
	float m_chase_range;

	//Attack range of enemy
	float m_attack_range;

	bool m_can_attack = true;

	float m_attack_timer = 1.f;
	float m_attack_time = 1.f;

	state m_state{ state::idle };

	engine::bounding_box m_enemy_collider;

	glm::vec3 start_pos;



};
