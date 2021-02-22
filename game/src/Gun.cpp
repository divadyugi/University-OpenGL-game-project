#include "Gun.h"


Gun::Gun(const engine::game_object_properties props) : engine::game_object(props) {}

Gun::~Gun() {}

void Gun::update()
{}

void Gun::init(float time_between_shots, float reload_time, int max_bullets, int mag_size)
{
	m_time_between_shots = time_between_shots;
	m_original_time = m_time_between_shots;
	m_reload_time = reload_time;
	m_original_reload_time = reload_time;
	m_max_bullets = max_bullets;
	m_mag_bullets = max_bullets;
	m_mag_size = mag_size;
	m_bullets = mag_size;
}

void Gun::reload()
{
	if ((m_mag_bullets - (m_mag_size - m_bullets)) <= 0)
	{
		m_bullets = m_bullets + m_mag_bullets;
		m_mag_bullets = 0;
	}
	else
	{
		m_mag_bullets -= (m_mag_size - m_bullets);
		m_bullets = m_mag_size;
	}
}

engine::ref<Gun> Gun::create(const engine::game_object_properties& props)
{
	return std::make_shared<Gun>(props);
}

void Gun::shoot()
{
	if (m_bullets > 0)
	{
		m_bullets--;
		//std::cout << "fired" << m_bullets << "/" << m_mag_bullets << "\n";
	}
}

