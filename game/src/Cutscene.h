#pragma once
#include <engine.h>


class Cutscene
{
public:

	Cutscene();

	~Cutscene();

	void init(const std::vector<std::string>& texts, engine::ref<engine::text_manager> text_manager);
	void on_render(const engine::ref<engine::shader>& shader);

	void next_text();

	bool in_cutscene() { return m_in_cutscene; }



private:

	engine::ref<engine::text_manager> m_text_manager;

	std::vector<std::string> m_texts;

	bool m_in_cutscene;

	int m_current_text;
};
