#include "pch.h"
#include "Bounding_sphere.h"
#include "platform/opengl/gl_shader.h"
#include "engine/entities/bounding_box.h"


engine::Bounding_sphere::Bounding_sphere()
{
	m_radius = 0;
}

engine::Bounding_sphere::~Bounding_sphere() {}

void engine::Bounding_sphere::get(glm::vec3& center, float& radius)
{
	center = m_center;
	radius = m_radius;
}

void engine::Bounding_sphere::set_sphere(glm::vec3 center, float radius)
{

	m_radius = radius;

	m_center = center;

	//Create the sphere
	engine::ref<engine::sphere> m_sphere = engine::sphere::create(32, 32, radius);

	m_mesh = m_sphere->mesh();
}


//Update function, used to place the sphere collider at target position every frame

void engine::Bounding_sphere::on_update(glm::vec3 position)
{
	m_center = position - glm::vec3(0.f, 0.f, 0.f);
}


void engine::Bounding_sphere::on_render(float r, float g, float b, const engine::ref<engine::shader>& shader)
{
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, m_center);

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("u_transform", transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_colour", glm::vec3(r, g, b));
	m_mesh->va()->bind();
	engine::renderer_api::draw_indexed_lines(m_mesh->va());
	engine::renderer_api::draw_indexed_lines(m_mesh->va());
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", false);
}

bool engine::Bounding_sphere::collision(engine::Bounding_sphere other_sphere)
{
	float radius1, radius2;
	glm::vec3 center1, center2;

	//This sphere
	get(center1, radius1);

	//Other sphere
	other_sphere.get(center2, radius2);

	glm::vec3 v = center2 - center1;

	float distance_between_spheres = std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z);

	if (distance_between_spheres <= (radius1 + radius2))
	{
		//Spheres collided
		/*std::cout << "Player sphere pos: " << center1 << " radius " << radius1 << "\n";
		std::cout << "Ramp sphere pos: " << center2 << " radius " << radius2 << "\n";
		std::cout << "Length between them: " << distance_between_spheres;*/
		//2.7 -0.75 - 16.7
		//Length = 16
		return true;
	}
	else
	{
		return false;
	}
}

bool engine::Bounding_sphere::box_collision(bounding_box other_box)
{
	//Get the coordinate of the box and the sphere
	glm::vec3 b_bottom, s_center;

	//Get the width, height and depth of the box, as well as get the radius of the sphere
	float b_width, b_height, b_depth, s_radius;
	other_box.get(b_bottom, b_width, b_height, b_depth);


	get(s_center, s_radius);

	glm::vec3 v = b_bottom - s_center;

	float dist = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	if (dist <= s_radius + (b_width / 2.f) |
		dist <= s_radius + (b_depth / 2.f) |
		dist <= s_radius + (b_height / 2.f))
	{
		//std::cout << "Sphere and box collided ";
		return true;
	}
	else
	{
		return false;
	}


}
