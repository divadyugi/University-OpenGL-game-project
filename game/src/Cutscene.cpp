#include "Cutscene.h"

Cutscene::Cutscene() {}

Cutscene::~Cutscene() {}


void Cutscene::init(const std::vector<std::string>& texts, engine::ref<engine::text_manager> text_manager)
{
	m_text_manager = text_manager;
	m_texts = texts;
}

void Cutscene::on_render(const engine::ref<engine::shader>& shader)
{

	/*m_text_manager->render_text(shader,
		"Press 'Enter' To continue",
		(float)engine::application::window().width() / 2 - 550.f, (float)engine::application::window().height() + 35.f,
		glm::vec4(.8f, .4f, .0f, 0.8f));*/
	m_text_manager->render_text(shader,
		"Press 'Enter' To continue",
		engine::application::window().width() / 2 + 550.f, (float)engine::application::window().height() + 35.f, 0.5f,
		glm::vec4(.8f, .4f, .0f, 0.8f));

	m_text_manager->render_text(shader,
		m_texts.at(m_current_text),
		engine::application::window().width()  + 30.f, (float)engine::application::window().height() + 35.f, 0.5f,
		glm::vec4(.8f, .4f, .0f, 0.8f));
}

void Cutscene::next_text()
{
	m_in_cutscene = true;
	m_current_text++;
	if (m_current_text < m_texts.size())
	{
		m_in_cutscene = true;
	}
	else {
		m_in_cutscene = false;
	}
}
