#include "Player.h"
#include "Gun.h"
#include "engine/entities/bounding_box.h"
#include "Bullet.h"

Player::Player(const engine::game_object_properties props) : engine::game_object(props) {}


void Player::init(std::vector<engine::ref<Gun>> weapons, float speed, std::vector<engine::ref<Bullet>> bullets)
{
	m_speed = speed;

	//weapon 0 = pistol
	//weapon 1 = rifle
	m_weapons = weapons;

	//Set the current weapon to be the pistol
	m_current_weapon = weapons.at(0);

	m_player_box.set_box(0.75f, 2.f, 0.75f, position());

	//m_player_sphere.set_sphere(position(), 2.5f);
	m_bullets = bullets;

}

void Player::update(float ts, engine::perspective_camera& camera, engine::ref<engine::audio_manager> audio_manager)
{

	

	//Set the forward and right vectors of the player to be the same as the camera's
	//as we are rotating the player based on the camera
	set_forward(camera.front_vector());
	set_right(camera.right_vector());

	//Move function
	move(ts);

	//Make the camera follow the player
	camera.position(position());

	switchWeapon();

	reload();

	//Shoot function
	if (engine::input::mouse_button_pressed(0) && m_can_attack && m_current_weapon->bullets() > 0)
	{
		m_current_weapon->shoot();
		m_can_attack = false;
		audio_manager->play("shoot");
		shoot(camera);
	}

	if (!m_can_attack && !m_reloading)
	{
		attackTimer(ts);
	}
	else if (!m_can_attack && m_reloading)
	{
		reloadTimer(ts);
	}

	//Jump function
	if (m_is_jumping)
	{
		set_velocity(velocity() + (0.5f * acceleration()));
		set_position(position() + velocity() * ts);
		ground_collision(2.f);
	}
	else
	{
		ground_response(2.f);
	}

	if (!ground_collision(2.f) && !m_is_grounded)
	{
		m_is_grounded = false;
	}



	if (m_push_back)
	{
		if (m_push_back_timer > 0)
		{
			m_push_back_timer -= ts;
			//setup a timer
			set_velocity(velocity() + (0.5f * acceleration()));
			set_position(position() + velocity() * ts);
			if (m_push_back_timer <= 0)
			{
				
				
				m_push_back = false;
				std::cout << "Acceleration: " << acceleration();
				m_push_back_timer = 0.4f;
			}
		}
	}


	//Collision
	m_player_box.on_update(position() - glm::vec3(0.f, 0.f, 0.f));
	//m_player_sphere.on_update(position());


}

void Player::on_render(const engine::ref<engine::shader>& shader)
{
	m_player_box.on_render(2.5f, 0.f, 0.f, shader);
	//m_player_sphere.on_render(2.5f, 0.f, 0.f, shader);

}

//Movement function
void Player::move(float ts)
{
	float current_y = position().y;
	//Left
	if (engine::input::key_pressed(engine::key_codes::KEY_A) && !m_push_back)
	{
		set_position(position() - m_speed * ts * right());
		set_position(glm::vec3(position().x, current_y, position().z));

	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_D) && !m_push_back)
	{
		//Right
		set_position(position() + m_speed * ts * right());
		set_position(glm::vec3(position().x, current_y, position().z));
	}

	if (engine::input::key_pressed(engine::key_codes::KEY_W) && !m_push_back)
	{
		set_position(position() + m_speed * ts * forward());
		set_position(glm::vec3(position().x, current_y, position().z));
	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_S) && !m_push_back)
	{
		set_position(position() - m_speed * ts * forward());
		set_position(glm::vec3(position().x, current_y, position().z));
	}

	if (engine::input::key_pressed(engine::key_codes::KEY_SPACE) && !m_is_jumping && !m_push_back)
	{
		set_velocity(glm::vec3(0.f, 0.f, 0.f));
		set_acceleration(glm::vec3(0.f, 0.f, 0.f));
		std::cout << "Jumped";
		set_velocity(glm::vec3(0.f, 15.02f, 0.f));
		set_acceleration(glm::vec3(0.f, -2.4f, 0.f));
		m_is_jumping = true;

	}
}

//Physics/collision
bool Player::ground_collision(float y_plane)
{
	if (position().y - bounding_shape().y < y_plane)
	{
		m_is_jumping = false;
		m_is_grounded = true;
		return true;
	}
}

void Player::ground_response(float y_pos)
{
	set_position(glm::vec3(position().x, y_pos, position().z));
}

void Player::ramp_response(const glm::vec3& height)
{

	set_position(position() + height);

}


//Weapon related functions
void Player::shoot(engine::perspective_camera& camera)
{
	for (int i = 0; i < m_bullets.size(); i++)
	{
		if (!(m_bullets.at(i)->active()))
		{
			m_bullets.at(i)->shoot(camera);
			m_bullets.at(i)->set_active(true);
			break;
		}
	}
}

void Player::switchWeapon()
{
	if (engine::input::key_pressed(engine::key_codes::KEY_1))
	{
		m_current_weapon = m_weapons.at(0);
		m_weapons.at(0)->set_active(true);
		m_weapons.at(1)->set_active(false);
	}
	else 	if (engine::input::key_pressed(engine::key_codes::KEY_2))
	{
		m_current_weapon = m_weapons.at(1);
		m_weapons.at(0)->set_active(false);
		m_weapons.at(1)->set_active(true);
	}
}

void Player::reload()
{
	if (engine::input::key_pressed(engine::key_codes::KEY_R))
	{
		m_current_weapon->reload();
		m_reloading = true;
		m_can_attack = false;
	}
}

void Player::attackTimer(float ts)
{
	if (m_current_weapon->time_between_shots() > 0.0f)
	{
		//std::cout << "The current weapon's time_between shots is more than 0, it is: " << m_current_weapon->time_between_shots();
		m_current_weapon->lower_time_attack(ts);

		if (m_current_weapon->time_between_shots() < 0.0f)
		{
			m_current_weapon->reset_attack_timer();
			m_can_attack = true;

		}
	}
}

void Player::reloadTimer(float ts)
{
	if (m_current_weapon->reload_time() > 0.0f)
	{
		m_current_weapon->lower_reload_time(ts);

		if (m_current_weapon->reload_time() < 0.0f)
		{
			m_current_weapon->reset_reload_timer();
			m_can_attack = true;
			m_reloading = false;

		}
	}
}


void Player::take_damage(float damage, const glm::vec3& push_back, float force)
{
	set_velocity(glm::vec3(0.f, 0.f, 0.f));
	set_acceleration(glm::vec3(0.f, 0.f, 0.f));
	set_acceleration(push_back*force);
	m_health -= damage;
	if (force != 0)
	{
		m_push_back = true;
	}
}

engine::ref<Player> Player::create(const engine::game_object_properties& props)
{
	//Makes a pointer to the player
	return std::make_shared<Player>(props);
}
