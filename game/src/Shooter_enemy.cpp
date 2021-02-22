#include "Shooter_enemy.h"
#include "engine/entities/bounding_box.h"
#include "Player.h"

Shooter_enemy::Shooter_enemy(const engine::game_object_properties& props) : engine::game_object(props) {}


Shooter_enemy::~Shooter_enemy() {}

void Shooter_enemy::init(float speed, float damage, float chase_range, float attack_range)
{
	m_speed = speed;
	m_damage = damage;
	m_chase_range = chase_range;

	m_attack_range = attack_range;

	start_pos = position();

	m_enemy_collider.set_box(4.f, 8.f, 4.f, position());
}

void Shooter_enemy::reset()
{
	set_position(start_pos);
	m_health = 50.f;
	m_state = state::idle;
	m_active = true;
}

void Shooter_enemy::on_update(float ts, const glm::vec3& target, engine::ref<Player> player)
{
	if (m_active)
	{
		if (m_health <= 0)
		{
			player->setCoins(player->coins() + 50);
			player->setScore(player->score() + 500);
			m_active = false;

		}

		glm::vec3 target_pos = glm::vec3(target.x, position().y, target.z);


		float dist = glm::distance(position(), target_pos);

		if (dist < m_attack_range)
		{
			m_state = state::attack;
			std::cout << "State: attack\n";
			//TODO: add shooting action here
		}
		else if (dist < m_chase_range)
		{
			m_state = state::chase;
			std::cout << "State: chase \n";
			chase(ts, target_pos);
			//TODO: add chase function here
		}
		else
		{
			m_state = state::idle;
			std::cout << "State: idle \n";
		}

		m_enemy_collider.on_update(position());

		if (!m_can_attack)
		{
			if (m_attack_timer > 0.f)
			{
				m_attack_timer -= ts;

				if (m_attack_timer <= 0.f)
				{
					m_can_attack = true;
				}
			}
		}
	}
}

void Shooter_enemy::on_render(const engine::ref<engine::shader>& shader)
{
	m_enemy_collider.on_render(2.5f, 0.f, 0.f, shader);
}

void Shooter_enemy::chase(float ts, const glm::vec3& target)
{
	set_forward(target - position());
	set_position(position() + forward() * m_speed * ts);
	set_rotation_amount(atan2(forward().x, forward().z));
}

engine::ref<Shooter_enemy> Shooter_enemy::create(const engine::game_object_properties& props)
{
	return std::make_shared<Shooter_enemy>(props);
}

