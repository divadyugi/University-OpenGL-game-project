#pragma once
#include <engine.h>

class quad;

class GUI
{
public:
	GUI(const std::string& path, float width, float height);

	~GUI();

	void on_render(engine::ref<engine::shader> shader, const glm::vec3& position);



	static engine::ref<GUI> create(const std::string& path, float width, float height);

	//Getters
	bool active() { return m_active; }

	void setActive(bool active) { m_active = active; }

private:

	//Checks if the GUI is active
	bool m_active = true;

	//Texture for the GUI
	engine::ref<engine::texture_2d> m_texture;

	engine::ref<quad> m_quad;
};
