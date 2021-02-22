#include "pch.h"
#include "GUI.h"
#include "quad.h"

GUI::GUI(const std::string& path, float width, float height)
{
	//Create the texture
	m_texture = engine::texture_2d::create(path, true);

	//Create the size and shape of the GUI
	m_quad = quad::create(glm::vec2(width, height));


}

GUI::~GUI()
{}


void GUI::on_render(engine::ref<engine::shader> shader, const glm::vec3& position)
{
	//Transform the GUI to the position given
	if (!m_active)
	{
		return;
	}

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, position);

	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);

}

engine::ref<GUI> GUI::create(const std::string& path, float width, float height)
{
	return std::make_shared<GUI>(path, width, height);
}
