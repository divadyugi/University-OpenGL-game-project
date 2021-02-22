#include "Melee_Enemy.h"
#include "Player.h"
#include "engine/entities/bounding_box.h"
#include "pickup.h"

Melee_enemy::Melee_enemy(const engine::game_object_properties props) : engine::game_object(props) {}


Melee_enemy::~Melee_enemy() {}

void Melee_enemy::init(float speed, float damage,float chase_range, float attack_range)
{
	m_speed = speed;
	m_damage = damage;
	m_chase_range = chase_range;
	m_attack_range = attack_range;

	m_enemy_collider.set_box(1.5f, 4.f, 1.5f, position());

	start_pos = position();
}

void Melee_enemy::reset()
{
	set_position(start_pos);
	m_health = 50.f;
	m_state = state::idle;
	m_active = true;
	m_pickup_spawned = false;
}

void Melee_enemy::on_update(float ts, const glm::vec3& target, engine::ref<Player> player)
{
	if (m_active)
	{

		if (m_health <= 0)
		{
			player->setCoins(player->coins() + 20);
			player->setScore(player->score() + 200);
			m_active = false;
			
		}
		float distance_to_target = glm::distance(position(), target);

		if (distance_to_target < m_attack_range)
		{
			m_state = state::attack;
			//perform attack action
			engine::ref<engine::cuboid> pickup_shape = engine::cuboid::create(glm::vec3(0.5f), false);
		}
		else if (distance_to_target < m_chase_range)
		{
			m_state = state::chase;
			//perform chase action
			chase(ts, target);

		}
		else
		{
			m_state = state::idle;
			//perform idle action

		}

		m_enemy_collider.on_update(position());
		if (m_attack_timer> 0.0f)
		{
			//std::cout << "The current weapon's time_between shots is more than 0, it is: " << m_current_weapon->time_between_shots();
			m_attack_timer -= ts;

			if (m_attack_timer < 0.0f)
			{
				m_attack_timer = m_attack_time;
				m_can_attack = true;

			}
		}
	}

	
}


int Melee_enemy::spawn_pickup()
{
	m_pickup_spawned = true;
	return std::rand() % 3;
}


void Melee_enemy::on_render(const engine::ref<engine::shader>& shader)
{
	m_enemy_collider.on_render(2.5f, 0.f, 0.f, shader);
}

void Melee_enemy::chase(float ts, const glm::vec3& target)
{
	set_forward(target - position());
	set_position(position() + forward() * m_speed * ts);
	set_rotation_amount(atan2(forward().x, forward().z));
}

void Melee_enemy::attack(engine::ref<Player> player)
{
	player->take_damage(m_damage, forward(), 1.f);
	m_can_attack = false;
}

engine::ref<Melee_enemy> Melee_enemy::create(const engine::game_object_properties& props)
{
	//Makes a pointer to the player
	return std::make_shared<Melee_enemy>(props);
}
