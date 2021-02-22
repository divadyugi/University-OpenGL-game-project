#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h"
#include "engine/entities/Bounding_sphere.h"

class Melee_enemy;

class Drone : public engine::game_object
{
public:

	Drone(const engine::game_object_properties props);

	~Drone();

	void init(engine::SpotLight spotlight, std::vector < engine::ref<Melee_enemy>> enemies);
	void on_update(float ts, glm::vec3& playerPos);
	void on_render(const engine::ref<engine::shader>& shader);


	//getters and setters

	bool active() { return m_active; }
	void set_active(bool active) { m_active = active; }
	static engine::ref<Drone> create(const engine::game_object_properties& props);

	void set_player_detected(bool player_detected) { m_player_detected = player_detected; }


	engine::bounding_box drone_collider() { return m_collider; }

	engine::Bounding_sphere drone_detector() { return m_spot_collider; }

private:

	bool m_active;

	//movement speed
	float m_speed;

	//Spotlight for spotting the player
	engine::SpotLight m_spotlight;

	//Collider for the spotlight
	engine::Bounding_sphere m_spot_collider;

	//target position for the patrol
	glm::vec3 m_target;

	//Timer for how long to stay at the patrol for
	float m_patrol_wait_time = 2.f;
	float m_patrol_timer = 2.f;

	bool patrol_reached = true;
	bool m_player_detected = false;

	//List of enemies to report back to
	std::vector < engine::ref<Melee_enemy>> m_enemies;

	//Collider for the drone
	engine::bounding_box m_collider;
};
