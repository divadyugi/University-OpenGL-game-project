#include "Pickup.h"
#include "Player.h"
#include "Gun.h"

//The engine::game_object(props) is the constructor of the extended class, taking the same props as pickup as an argument
pickup::pickup(const engine::game_object_properties props) : engine::game_object(props) {}

//Destructor
pickup::~pickup()
{}

void pickup::init(const glm::vec3& spawn_pos, type current_type, float amount)
{
	set_position(spawn_pos);
	//m_active = true;
	m_amount = amount;
	m_current_type = current_type;
}

void pickup::update(float ts, engine::ref<Player> player, engine::ref<engine::audio_manager> audio_manager)
{
	if (m_active)
	{

		set_rotation_amount(rotation_amount() + ts * 1.5f);
		if (m_move_up)
		{
			glm::vec3 newPosition(position().x, position().y + ts * 0.5f, position().z);
			set_position(newPosition);
			if (position().y >= 1.5f)
			{
				m_move_up = false;
			}
		}
		else
		{
			glm::vec3 newPosition(position().x, position().y - ts * 0.5f, position().z);
			set_position(newPosition);
			if (position().y <= .75f)
			{
				m_move_up = true;
			}
		}

		std::cout << "Player: " << player->position() << "\n";
		glm::vec3 d = position() - player->position();
		std::cout << "Distance: " << d << "\n";
		if (glm::length(d) < 4.0f)
		{
			audio_manager->play("pickup");
			if (m_current_type == type::ammo)
			{
				player->current_weapon()->set_current_mag(m_amount);
			}else
			{
				player->setHealth(player->health() + m_amount);
			}
			m_active = false;
			
		}
	}

}
engine::ref<pickup> pickup::create(const engine::game_object_properties& props)
{
	//This is bacially the reason why we can use ->function instead of .function
	//This is more efficient, as it is a shared pointer, rather than directly accessing the functions
	return std::make_shared<pickup>(props);
}
