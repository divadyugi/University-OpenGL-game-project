#pragma once
#include <engine.h>
#include "engine/entities/Bounding_sphere.h"
#include "engine/entities/bounding_box.h"
#include "GUI.h"


class Rocket;
class Gun;
class Player;
class Melee_enemy;
class Shooter_enemy;
class Bullet;
class Drone;
class billboard;
class pickup;
class Enemy_bullet;

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:

	float m_level = 0;

	//************Player************
	engine::ref<Player> m_player{};

	engine::ref<Gun> m_gun{};
	engine::ref<Gun> m_rifle{};
	std::vector<engine::ref<Gun>> m_guns{};

	//Bullets used for shooting
	engine::ref<Bullet> m_bullet {};
	std::vector<engine::ref<Bullet>> m_bullets{};
	engine::ref<billboard> m_explosion_effect{};



	//Intro scene
	engine::ref<Rocket>	m_rocket{};

	float alpha = 1.f;
	float alpha_increment = -0.1f;

	bool m_intro_scene = true;

	float m_intro_timer = 7.5f;

	//*********Lighting***********
	engine::PointLight m_pointLight;
	uint32_t	num_point_lights = 1;
	engine::ref<engine::material>		m_lightsource_material{};
	engine::ref<engine::game_object>	m_ball{};


	//************Level 1************
	engine::ref<billboard> m_fire_effect{};
	void level_one_render();
	engine::ref<engine::game_object> m_ramp{};
	engine::ref<engine::game_object> m_sign{};
	//You need to add 1/2 bounding box to center the bounding box

	//************Level 2************
	void level_two_render();
	float level_two_time = 30.f;
	float level_two_timer = 30.f;



	//************Level 3************
	void level_three_render();
	bool m_all_killed =  false;
	engine::bounding_box m_ramp_box;

	//************Melee Enemy************
	engine::ref<Melee_enemy> m_melee_enemy {};

	std::vector<engine::ref<Melee_enemy>> m_enemies {};

	std::vector<engine::ref<Melee_enemy>> level_3_enemies{};

	//************Shooter Enemy************
	engine::ref<Shooter_enemy> m_shooter_enemy{};
	std::vector < engine::ref<Shooter_enemy>> m_shooter_enemies{};

	engine::ref<Enemy_bullet> m_shooter_bullet{};
	std::vector<engine::ref<Enemy_bullet>> m_shooter_bullets{};

	engine::ref<engine::material> m_bullet_material{};

	//************Drone Enemy************
	engine::ref<Drone> m_drone{};
	engine::SpotLight m_spotLight;
	engine::SpotLight m_spotLight1;
	engine::SpotLight m_spotLight2;
	uint32_t	num_spot_lights = 3;
	std::vector < engine::ref<Drone>> m_drones;
	std::vector<engine::SpotLight> m_spotlights;

	//************Enemy Drops************
	engine::ref<pickup> m_health_pickup;
	engine::ref<pickup> m_ammo_pickup;
	engine::ref<pickup> m_grenade_pickup;
	std::vector<engine::ref<pickup>> m_pickups;

	//************HUD************
	engine::ref<GUI> m_HUD{};

	void GUI_render();

	void shop_render();

	bool m_in_shop = false;

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};


	engine::ref<engine::material>		m_material{};

	engine::DirectionalLight            m_directionalLight;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	engine::ref<engine::text_manager>	m_text_manager{};



    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;

};
