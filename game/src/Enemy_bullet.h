#pragma once
#include <engine.h>
#include "engine/entities/Bounding_sphere.h"


class Enemy_bullet : public engine::game_object
{
public:

	Enemy_bullet(const engine::game_object_properties& props);

	~Enemy_bullet();

	void init(float speed, float lifetime);
	void on_update(float ts);

	void shoot(const glm::vec3& start, const glm::vec3& target);

	static engine::ref<Enemy_bullet> create(const engine::game_object_properties& props);

	//Getters and setters
	bool active() { return m_active; }

	void set_active(bool active) { m_active = active; }

	engine::Bounding_sphere collider() { return m_collider; }

private:

	bool m_active = false;

	//Speed at whichj the ball will move at
	float m_speed;

	float m_lifetime;

	float m_life_timer;

	glm::vec3 start_pos;

	glm::vec3 target_pos;
	//Collider
	engine::Bounding_sphere m_collider;

};
