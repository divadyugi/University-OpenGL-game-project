#pragma once
#include <engine.h>

class Player;

class pickup : public engine::game_object
{

public:

	enum class type
	{
		ammo,
		health,
		grenade
	};

	pickup(const engine::game_object_properties props);
	~pickup();
	void init(const glm::vec3& spawn_pos,type current_type, float amount  );

	void update(float ts, engine::ref<Player> player, engine::ref<engine::audio_manager> audio_manager);

	bool active() { return m_active; }

	static engine::ref<pickup> create(const engine::game_object_properties& props);

	type pickup_type() { return m_current_type; }

	float amount() { return m_amount; }

	void set_active(bool active) { m_active = active; }

private:

	bool m_active = false;

	bool m_move_up = true;

	float m_amount ;

	type m_current_type;
};
