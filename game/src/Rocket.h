#pragma once
#include <engine.h>


class Rocket : public engine::game_object
{
public:
	//Constructor
	Rocket(const engine::game_object_properties props);

	//Destructor
	~Rocket();

	void init(glm::vec3 target_pos);
	void update(float dt);
	bool active() { return m_is_active; }

	void setTarget(glm::vec3 new_target);

	void setSpeed(float new_speed);


	static engine::ref<Rocket> create(const engine::game_object_properties& props);

private:

	//Boolean for checking if object is active
	bool m_is_active;
	//The position of the target that the rocket will move to in the update function
	glm::vec3 m_target_pos{ 0.f,0.f,0.f };
	//The speed the rocket will move at
	float m_speed{ 0.1f };

	void move_towards(float dt);
};
