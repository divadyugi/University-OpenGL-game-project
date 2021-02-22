#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h"
#include "engine/entities/Bounding_sphere.h"
#include "Drone.h"
#include "Melee_Enemy.h"

Drone::Drone(const engine::game_object_properties props) : engine::game_object(props) {}


Drone::~Drone() {}

void Drone::init(engine::SpotLight spotlight, std::vector < engine::ref<Melee_enemy>> enemies)
{
	m_enemies = enemies;
	m_spotlight = spotlight;
	m_target = position();
	m_spot_collider.set_sphere(position() - glm::vec3(0.f, 2.f, 0.f), 2.f);
	m_collider.set_box(1.5, 2.f, 1.5f, position());
}

void Drone::on_render(const engine::ref<engine::shader>& shader)
{
	m_spot_collider.on_render(2.5f, 0.f, 0.f, shader);
	m_collider.on_render(2.5f, 0.f, 0.f, shader);
}

void Drone::on_update(float ts, glm::vec3& playerPos)
{
	if (m_active)
	{
		m_spotlight.Position = position();
		m_spotlight.Direction = forward();
		m_spot_collider.on_update(position() + glm::vec3(0.f, -2.f, 0.f));

		//std::cout << "Patrol reached: " << patrol_reached;
		//Check if you  reached the patrol point
		float dist = glm::distance(m_target, position());
		if (dist <= 1.f)
		{
			patrol_reached = true;
		}
		else
		{
			patrol_reached = false;
		}
		//if you have reached patrol point, work out new patrol point
		if (patrol_reached)
		{
			set_forward(forward() - 0.1f);
			set_rotation_amount(rotation_amount() + engine::PI / 60);

		}

		if (m_player_detected)
		{
			m_spotlight.Color = glm::vec3(1.f, 0.f, 0.f);
			m_target = glm::vec3(100.f, 4.f, 100.f);
			set_forward(m_target - position());
			set_position(position() + forward() * 0.4f * ts);
			set_rotation_amount(atan2(forward().x, forward().z));
			for (int i = 0; i < m_enemies.size(); i++)
			{
				m_enemies.at(i)->set_chase_range(1000.f);
			}
			m_active = false;
		}

		if (!patrol_reached)
		{

			set_forward(m_target - position());
			set_position(position() + forward() * 0.8f * ts);
			set_rotation_amount(atan2(forward().x, forward().z));
		}
		if (m_patrol_timer > 0.f && patrol_reached)
		{
			m_patrol_timer -= ts;

			if (m_patrol_timer <= 0.f)
			{
				m_target = glm::vec3(playerPos.x + (std::rand() % 2), position().y, playerPos.z + (std::rand() % 2));
				patrol_reached = false;
				m_patrol_timer = m_patrol_wait_time;
			}
		}
	}
}

engine::ref<Drone> Drone::create(const engine::game_object_properties& props)
{
	//Makes a pointer to the player
	return std::make_shared<Drone>(props);
}

