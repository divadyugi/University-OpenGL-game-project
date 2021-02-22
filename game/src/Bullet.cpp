#include "Bullet.h"
#include "engine/entities/Bounding_sphere.h"


Bullet::Bullet(const engine::game_object_properties props)
{
	m_object = engine::game_object::create(props);
}

Bullet::~Bullet() {}


//Initialisation
void Bullet::init(float speed, float lifetime, glm::vec3 start_pos)
{
	m_speed = speed;
	m_lifetime = lifetime;
	m_timer = lifetime;

	//Since we are not instantiating a new bullet everytime we shoot, we just move it to and from the start position when needed
	m_start_pos = start_pos;

	m_object->set_position(start_pos);

	//We will have all the bullets instantiated from the start, so only set them active when needed (object pooling)
	m_active = false;

	m_collider.set_sphere(m_object->position(), .3f);
	m_object->set_mass(0.1f);
}

void Bullet::on_update(float ts)
{

	if (m_active)
	{

		//set_rotation_amount(m_phi);
		//set_rotation_axis(glm::vec3(0.f, 1.f, 0.f));

		//std::cout << "Velocity: " << velocity() << " Acceleration: " << acceleration();
		m_object->set_velocity(m_object->velocity() + 0.5f * m_object->acceleration());
		m_object->set_position(m_object->position() + m_object->velocity() *ts);
		m_collider.on_update(m_object->position());

		/*set_velocity(velocity() + (m_instantaneous_acceleration)*ts);
		set_position(position() + velocity() * ts);*/

		//m_transform = glm::translate(m_transform, position());

		if (m_timer > 0.0f)
		{
			m_timer -= ts;

			if (m_timer < 0.0f)
			{
				m_timer = m_lifetime;
				m_active = false;
			}
		}
		
	}

	
}

void Bullet::on_render(const engine::ref<engine::shader>& shader)
{
	//m_object->set_position(glm::vec3(30.f, 2.f, 0.f));
	m_transform = glm::mat4(1.0f);
	m_transform = glm::translate(m_transform, m_object->position());
	//transform = glm::rotate(transform, m_phi, glm::vec3(0.f, 1.f, 0.f));
	m_transform = glm::rotate(m_transform, -m_theta, glm::vec3(1.f, 0.f, 0.f));
	m_transform = glm::rotate(m_transform, m_phi, glm::vec3(0.f, 1.f, 0.f));
	m_transform = glm::rotate(m_transform, m_object->rotation_amount(), m_object->rotation_axis());
	m_transform = glm::scale(m_transform, glm::vec3(10.f, 10.f, 10.f));

	engine::renderer::submit(shader, m_transform, m_object);

	m_collider.on_render(2.5f, 0.f, 0.f, shader);
	
}

void Bullet::shoot(const engine::perspective_camera& camera)
{
	m_object->set_velocity(glm::vec3(0.f));
	m_object->set_acceleration(glm::vec3(0.f));

	m_object->set_acceleration(camera.front_vector() * m_speed);


	m_object->set_position(camera.position()- glm::vec3(0.f,3.f,0.f) + camera.front_vector() * 7.f);

	glm::vec3 force = camera.front_vector() * m_speed;

	m_instantaneous_acceleration = force / m_object->mass();

	m_theta = engine::PI / 2.f - acos(camera.front_vector().y);
	m_phi = atan2(camera.front_vector().x, camera.front_vector().z);

	/*m_transform = glm::mat4(1.0f);
	m_transform = glm::translate(m_transform, m_object->position());
	//transform = glm::rotate(transform, m_phi, glm::vec3(0.f, 1.f, 0.f));
	m_transform = glm::rotate(m_transform, -m_theta, glm::vec3(1.f, 0.f, 0.f));
	m_transform = glm::rotate(m_transform, m_phi, glm::vec3(0.f, 1.f, 0.f));
	m_transform = glm::rotate(m_transform, m_object->rotation_amount(), m_object->rotation_axis());
	m_transform = glm::scale(m_transform, glm::vec3(10.f, 10.f, 10.f));*/
}

engine::ref<Bullet> Bullet::create(const engine::game_object_properties& props)
{
	//Makes a pointer to the player
	return std::make_shared<Bullet>(props);
}
