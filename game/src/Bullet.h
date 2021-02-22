#pragma once
#include <engine.h>
#include "engine/entities/Bounding_sphere.h"


class Bullet
{
public:

	//Constructor
	Bullet(const engine::game_object_properties props);

	//Destructor
	~Bullet();


	void init(float speed, float lifetime, glm::vec3 start_pos);
	void on_update(float ts);
	void on_render(const engine::ref<engine::shader>& shader);

	void move(float ts);

	void shoot(const engine::perspective_camera& camera);

	//Getters and setters
	bool active() { return m_active; }

	void set_active(bool active) { m_active = active; }

	engine::Bounding_sphere bullet_collider() { return m_collider; }

	glm::mat4 bullet_transform() { return m_transform; }

	engine::ref<engine::game_object> object() { return m_object; }

	static engine::ref<Bullet> create(const engine::game_object_properties& props);


private:

	bool m_active{ false };

	//Speed the bullet moves at
	float m_speed{ 2.f };

	//Rotation angles
	float m_theta, m_phi;

	//Variables for the timer, to make the bullet inactive after x seconds
	float m_lifetime, m_timer;

	glm::vec3 m_start_pos;



	//Collider
	engine::Bounding_sphere  m_collider;

	//Bullet transform used for rendering
	glm::mat4 m_transform{glm::mat4(1.0f)};
	glm::vec3 m_instantaneous_acceleration;

	engine::ref<engine::game_object> m_object;

	glm::vec3 render_position;
};
