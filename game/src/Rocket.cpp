#include "Rocket.h"

Rocket::Rocket(const engine::game_object_properties props) : engine::game_object(props) {}


Rocket::~Rocket() {}

void Rocket::init(glm::vec3 target_pos)
{
	m_is_active = true;
	m_target_pos = target_pos;

}

void Rocket::update(float dt)
{
	move_towards(dt);
	/*glm::vec3 target(10.f, 20.f, 0.f);
	glm::vec3 start(position());
	float dist = glm::distance(position(), target);
	if (std::abs(dist) >= .1f)
	{
		//Find the direction to move towards
		glm::vec3 direction(target - start);
		//Find out the unit vector to move item by
		direction = glm::normalize(direction);
		glm::vec3 newPos(start + direction * 0.1f);
		set_position(newPos);
	}*/
}

void setTarget(glm::vec3 new_target);

void setSpeed(float new_speed);

void Rocket::move_towards(float dt)
{

	glm::vec3 start(position());
	//Find out the distance between the rocket's current position and the target position
	float dist = glm::distance(start, m_target_pos);

	//If the distance between the two is more than .1f, then move the rocket towards the target position
	if(std::abs(dist)>=.1f)
	{
		//Find the direction to move towards:
		glm::vec3 direction(m_target_pos - start);

		//Find out how much to move the rocket by, by using the normalizing the direction vector
		direction = glm::normalize(direction);
		glm::vec3 newPos(start + direction * m_speed);
		set_position(newPos);
	}
}

engine::ref<Rocket> Rocket::create(const engine::game_object_properties& props)
{
	return std::make_shared<Rocket>(props);
}
