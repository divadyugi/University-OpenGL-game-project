#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h"
#include "pickup.h"


//Idle rotate towards player
//In chase range, move towards player, also rotate towards the player
//In attack range, attack player

class Player;


class Melee_enemy	: public engine::game_object
{

public:
	enum class state
	{
		idle,
		chase,
		attack,
		frozen
	};

	//Constructor
	Melee_enemy(const engine::game_object_properties props);

	//Destructor
	~Melee_enemy();

	//initialiser
	void init(float speed, float damage,  float chase_range, float attack_range);

	//Update function
	void on_update(float ts, const glm::vec3& target, engine::ref<Player> player);

	//Renderer
	void on_render(const engine::ref<engine::shader>& shader);
	void pickup_render(const engine::ref<engine::shader>& shader);

	void chase(float ts, const glm::vec3& target);

	void attack(engine::ref<Player> player);

	void reset();

	static engine::ref<Melee_enemy> create(const engine::game_object_properties& props);

	//getters and setters

	bool active() { return m_active; }
	state current_state() { return m_state; }

	engine::bounding_box enemy_collider() { return m_enemy_collider; }

	bool can_attack() { return m_can_attack; }

	void lowerHealth(float damage) { m_health -= damage; }

	int spawn_pickup();

	bool pickup_spawned() { return m_pickup_spawned; }

	void set_chase_range(float chase_range) { m_chase_range = chase_range; }

private:

	bool m_active;

	float m_health = 50.f;

	//enemy's speed
	float m_speed{ 1.f };

	//attack damage
	float m_damage{ 5.f };

	//how far the player has to be for chase
	float m_chase_range{ 20.f };

	//how far the player has to be for attack
	float m_attack_range{ 3.f };


	float m_attack_time = 0.6f;
	float m_attack_timer = 0.6f;

	bool m_can_attack = true;

	//Current state of the state machine
	state m_state = state::idle;

	engine::bounding_box m_enemy_collider;

	glm::vec3 start_pos;

	bool m_pickup_spawned = false;
};
