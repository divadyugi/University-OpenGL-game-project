#include "Enemy_bullet.h"
#include "engine/entities/Bounding_sphere.h"

#include "Enemy_bullet.h"

Enemy_bullet::Enemy_bullet(const engine::game_object_properties& props) : engine::game_object(props) {}

Enemy_bullet::~Enemy_bullet() {}

void Enemy_bullet::init(float speed, float lifetime)
{
	m_speed = speed;
	m_lifetime = lifetime;
	m_life_timer = lifetime;

	start_pos = position();
	m_collider.set_sphere(position(), 0.5f);
}

void Enemy_bullet::on_update(float ts)
{
	if (m_active)
	{

	
		glm::vec3 start(position());

		float dist = glm::distance(start, target_pos);

		if (std::abs(dist) >= .1f)
		{
			glm::vec3 direction(target_pos - start);

			direction = glm::normalize(direction);
			glm::vec3 newPos(start + direction * m_speed * ts);
			set_position(newPos);
		}
		m_collider.on_update(position());
	}
	if (m_life_timer > 0.0f)
	{
		std::cout << "Bullet timer: " << m_life_timer;
		m_life_timer -= ts;

		if (m_life_timer < 0.0f)
		{
			m_active = false;
			
		}
	}
}

void Enemy_bullet::shoot(const glm::vec3& start, const glm::vec3& target)
{
	set_position(start);

	target_pos = target;
}


engine::ref<Enemy_bullet> Enemy_bullet::create(const engine::game_object_properties& props)
{
	return std::make_shared<Enemy_bullet>(props);
}
