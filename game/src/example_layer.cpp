#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include <gl\GL.h>
#include "Rocket.h"
#include "Gun.h"
#include "Player.h"
#include "Melee_Enemy.h"
#include "Drone.h"
#include "Shooter_enemy.h"
#include "Bullet.h"
#include "GUI.h"
#include "billboard.h"
#include "Enemy_bullet.h"

example_layer::example_layer() 
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f), 
    m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())


{
	glm::vec3 m_fog_colour = glm::vec3(0.1f, 0.1f, 0.1f);
	int m_fog_factor_type = 1;
    // Hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::event, "bounce"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/DST-impuretechnology.mp3", engine::sound_type::track, "music");  // Royalty free music from http://www.nosoapradio.us/
	m_audio_manager->load_sound("assets/audio/Hit_Hurt.wav", engine::sound_type::event, "hit");
	m_audio_manager->load_sound("assets/audio/Laser_Shoot5.wav", engine::sound_type::event, "shoot");
	m_audio_manager->load_sound("assets/audio/Powerup.wav", engine::sound_type::event, "pickup");
	//m_audio_manager->play("music");


	// Initialise the shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	if (m_level == 2)
	{
		m_directionalLight.AmbientIntensity = 0.05f;
	}
	else
	{

		m_directionalLight.AmbientIntensity = 0.25f;
	}
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	//*********Player point light***********

	m_pointLight.Color = glm::vec3(0.f, 1.0f, 0.f);
	m_pointLight.AmbientIntensity = 0.5f;
	m_pointLight.DiffuseIntensity = 0.6f;
	m_pointLight.Position = glm::vec3(0.0f, 2.0f, 0.0f);

	m_spotLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_spotLight.AmbientIntensity = 1.f;
	m_spotLight.DiffuseIntensity = 0.6f;
	m_spotLight.Position = glm::vec3(-5.0f, 2.0f, 0.0f);
	m_spotLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
	m_spotLight.Cutoff = 0.5f;
	m_spotLight.Attenuation.Constant = 1.0f;
	m_spotLight.Attenuation.Linear = 0.1f;
	m_spotLight.Attenuation.Exp = 0.01f;

	m_spotLight1.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_spotLight1.AmbientIntensity = 1.f;
	m_spotLight1.DiffuseIntensity = 0.6f;
	m_spotLight1.Position = glm::vec3(-5.0f, 2.0f, 0.0f);
	m_spotLight1.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
	m_spotLight1.Cutoff = 0.5f;
	m_spotLight1.Attenuation.Constant = 1.0f;
	m_spotLight1.Attenuation.Linear = 0.1f;
	m_spotLight1.Attenuation.Exp = 0.01f;


	m_spotLight2.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_spotLight2.AmbientIntensity = 1.f;
	m_spotLight2.DiffuseIntensity = 0.6f;
	m_spotLight2.Position = glm::vec3(-5.0f, 2.0f, 0.0f);
	m_spotLight2.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
	m_spotLight2.Cutoff = 0.5f;
	m_spotLight2.Attenuation.Constant = 1.0f;
	m_spotLight2.Attenuation.Linear = 0.1f;
	m_spotLight2.Attenuation.Exp = 0.01f;


	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	//m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity", 0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);

	m_lightsource_material = engine::material::create(1.0f, m_pointLight.Color,
		m_pointLight.Color, m_pointLight.Color, 1.0f);

	m_bullet_material = engine::material::create(1.f, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 1.0f);


	// Skybox texture from https://assetstore.unity.com/packages/2d/textures-materials/sky/skybox-volume-2-nebula-3392

	//Increased skybox size to 100, because of the increased far clipping plane
	
	m_skybox = engine::skybox::create(100.f,
		{ engine::texture_2d::create("assets/textures/newskybox/frontImage.png", true),
		  engine::texture_2d::create("assets/textures/newskybox/leftImage.png", true),
		  engine::texture_2d::create("assets/textures/newskybox/backImage.png", true),
		  engine::texture_2d::create("assets/textures/newskybox/rightImage.png", true),
		  engine::texture_2d::create("assets/textures/newskybox/upImage.png", true),
		  engine::texture_2d::create("assets/textures/newskybox/downImage.png", true)
		});



	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	//glTexParameter(type) (target specifies the texture mode (GL_TEXTURE_2D), pname(which texture parameter is being set), param(value to set))
	//Rock texture from: https://assetstore.unity.com/packages/2d/textures-materials/24-pbr-materials-for-unity-5-51991
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/rock3.png", false) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(500.f, 0.5f, 500.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	//Mesh from rocket is from: https://free3d.com/3d-model/rocket-v1--141304.html
	engine::ref<engine::model> rocket_model = engine::model::create("assets/models/static/rocket_4.obj");

	engine::game_object_properties rocket_props;
	rocket_props.meshes = rocket_model->meshes();
	rocket_props.textures = rocket_model->textures() ;
	float rocket_scale = 0.1f;
	rocket_props.position = { 10.f,40.f,0.f };
	rocket_props.scale = glm::vec3(0.1f, 0.1f, 0.1f);
	rocket_props.bounding_shape = rocket_model->size() / 2.f * rocket_scale;

	m_rocket = Rocket::create(rocket_props);
	m_rocket->init(glm::vec3(0.f, 0.f, 0.f));

	m_fire_effect = billboard::create("assets/textures/fire1_64.png", 10, 6, 30);
	m_explosion_effect = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);
	//m_3d_camera.set_view_matrix((m_rocket->position() + glm::vec3(10.f, 2.f, -40.f)), m_rocket->position());

	//************Level1:************
	std::vector<glm::vec3> ramp_vertices;
	ramp_vertices.push_back(glm::vec3(0.f, 0.f, 0.f));	//0
	ramp_vertices.push_back(glm::vec3(0.f, 0.f, 3.f));	//1
	ramp_vertices.push_back(glm::vec3(0.f, 1.f, 3.f));	//2
	ramp_vertices.push_back(glm::vec3(1.f, 0.f, 3.f));	//3
	ramp_vertices.push_back(glm::vec3(1.f, 1.f, 3.f));	//4
	ramp_vertices.push_back(glm::vec3(1.f, 0.f, 0.f));	//5

	engine::ref<engine::Ramp> ramp_shape = engine::Ramp::create(ramp_vertices);
	std::vector<engine::ref<engine::texture_2d>> ramp_texture =
	{ engine::texture_2d::create("assets/textures/ramp_text.png", true) };

	engine::game_object_properties ramp_props;
	ramp_props.position = { 0.f,0.25f,-12.f };
	ramp_props.meshes = { ramp_shape->mesh() };
	//Texture from http://www.texturise.club/2016/05/tillable-metal-plate-texture-maps.html
	ramp_props.textures = ramp_texture;
	ramp_props.scale = glm::vec3(3.f, 3.f, 3.f);
	m_ramp = engine::game_object::create(ramp_props);

	m_ramp_box.set_box(3.f, 3.f, 9.f, m_ramp->position() + glm::vec3(1.f, 0.f, 5.f));

	//*********Sign*********
		//Sign vertices
	std::vector<glm::vec3> board_vertices;
	//0
	board_vertices.push_back(glm::vec3(1.f, 0.5f, 0.f));
	//1
	board_vertices.push_back(glm::vec3(1.f, 0.5f, 0.1f));
	//2
	board_vertices.push_back(glm::vec3(1.f, 1.f, 0.f));
	//3
	board_vertices.push_back(glm::vec3(1.f, 1.f, 0.1f));
	//4
	board_vertices.push_back(glm::vec3(0.f, 1.f, 0.1f));
	//5
	board_vertices.push_back(glm::vec3(0.f, 1.f, 0.f));
	//6
	board_vertices.push_back(glm::vec3(0.f, 0.5f, 0.1f));
	//7
	board_vertices.push_back(glm::vec3(0.f, 0.5f, 0.f));

	std::vector<glm::vec3> post_vertices;
	//0
	post_vertices.push_back(glm::vec3(0.25f, 0.f, 0.f));
	//1
	post_vertices.push_back(glm::vec3(.5f, 0.f, 0.f));
	//2
	post_vertices.push_back(glm::vec3(.5f, 0.5f, 0.f));
	//3
	post_vertices.push_back(glm::vec3(0.25f, 0.5f, 0.0f));
	//4
	post_vertices.push_back(glm::vec3(0.5f, 0.f, 0.05f));
	//5
	post_vertices.push_back(glm::vec3(0.5f, 0.5f, 0.05f));
	//6
	post_vertices.push_back(glm::vec3(0.25f, 0.5f, 0.05f));
	//7
	post_vertices.push_back(glm::vec3(0.25f, 0.f, 0.05f));




	engine::ref<engine::Sign> sign_shape = engine::Sign::create(board_vertices, post_vertices);
	std::vector<engine::ref<engine::texture_2d>> sign_Texture =
	{ engine::texture_2d::create("assets/textures/ramp_text2.png", true) };

	engine::game_object_properties sign_props;
	sign_props.position = { 0.f, 1.5f, -10.f };
	sign_props.meshes = { sign_shape->mesh() };
	sign_props.textures = sign_Texture;
	m_sign = engine::game_object::create(sign_props);

	//Gun object
	engine::ref<engine::model> gun_model = engine::model::create("assets/models/static/colt_v2.3ds");

	engine::game_object_properties gun_props;
	gun_props.meshes = gun_model->meshes();
	gun_props.textures = gun_model->textures();
	float gun_scale = 0.25f;
	gun_props.position = { 0.f,0.f,0.f };
	gun_props.rotation_amount = engine::PI / 2;
	gun_props.rotation_axis = glm::vec3(1.f, 0.f, 0.f);
	gun_props.scale = glm::vec3(0.01f, 0.01f, 0.01f);
	gun_props.bounding_shape = gun_model->size() / 2.f * gun_scale;

	m_gun = Gun::create(gun_props);
	m_gun->init(0.4f, 1.2f, 64, 16);

	m_guns.push_back(m_gun);


	//Rifle object
	engine::ref<engine::model> rifle_model = engine::model::create("assets/models/static/MP44_V2.obj");

	engine::game_object_properties rifle_props;
	rifle_props.meshes = rifle_model->meshes();
	rifle_props.textures = rifle_model->textures();
	float rifle_scale = 0.25f;
	rifle_props.position = { 0.f,0.f,0.f };
	rifle_props.scale = glm::vec3(0.01f, 0.01f, 0.01f);
	rifle_props.bounding_shape = rifle_model->size() / 2.f * rifle_scale;

	m_rifle = Gun::create(rifle_props);
	m_rifle->init(0.2f, 1.5f, 128, 32);

	m_guns.push_back(m_rifle);

	//************Bullets************
	engine::ref<engine::model> bullet_model = engine::model::create("assets/models/static/bullet_low.obj");

	engine::game_object_properties bullet_props;
	bullet_props.meshes = bullet_model->meshes();
	bullet_props.textures = bullet_model->textures();
	float bullet_scale = 0.5f;
	bullet_props.position = { 30.f,4.f,0.f };
	bullet_props.rotation_amount = engine::PI / 2;
	bullet_props.rotation_axis = glm::vec3(1.f, 0.f, 0.f);
	bullet_props.scale = glm::vec3(10.f, 10.f, 10.f);
	bullet_props.bounding_shape = bullet_model->size();

	m_bullet = Bullet::create(bullet_props);
	m_bullet->init(1.f, 10.f, glm::vec3(30.f, 4.f, 0.f));


	for (int i = 0; i < 40; i++)
	{
		m_bullet = Bullet::create(bullet_props);
		m_bullet->init(0.5f, 10.f, glm::vec3(30.f + i*1.5f, 4.f, 0.f));
		m_bullets.push_back(m_bullet);

	}

	//************Player************

	engine::game_object_properties player_props;

	player_props.position = glm::vec3(150.f, 2.f, -30.f);
	player_props.scale = glm::vec3(1.f, 1.f, 1.f);

	m_player = Player::create(player_props);
	m_player->init(m_guns, 10.f, m_bullets);

	//************HUD************
	m_HUD = GUI::create("assets/textures/HUD.png", 1.6f, 0.9f);


	//************Enemy************
	engine::ref<engine::model> enemy_model = engine::model::create("assets/models/static/enemy_v2.3ds");

	engine::game_object_properties melee_props;
	melee_props.meshes = enemy_model->meshes();
	melee_props.textures = enemy_model->textures();
	float enemy_scale = 1.f;
	melee_props.position = { 10.f,1.f,0.f };
	melee_props.scale = glm::vec3(1.f, 1.f, 1.f);
	melee_props.bounding_shape = enemy_model->size() / 2.f * enemy_scale;



	for (int i = 0; i < 10; i++)
	{
		melee_props.position.x += std::rand()%3*i;
		melee_props.position.z += std::rand() % 3 * i;
		m_melee_enemy = Melee_enemy::create(melee_props);

		m_melee_enemy->init(0.4f, 5.f, 50.f, 0.5f);
		m_enemies.push_back(m_melee_enemy);
		
	}

	//************Drone Enemy************
	engine::ref<engine::model> drone_model = engine::model::create("assets/models/static/Drone_2.obj");

	engine::game_object_properties drone_props;
	drone_props.meshes = drone_model->meshes();
	drone_props.textures = drone_model->textures();
	float drone_scale = 0.01f;
	drone_props.position = { 60.f,4.f,0.f };
	drone_props.scale = glm::vec3(drone_scale, drone_scale, drone_scale);
	drone_props.bounding_shape = drone_model->size() / 2.f * drone_scale;

	m_drone = Drone::create(drone_props);
	m_drone->init(m_spotLight, m_enemies);

	m_drones.push_back(m_drone);

	m_drone = Drone::create(drone_props);
	m_drone->init(m_spotLight1, m_enemies);
	m_drones.push_back(m_drone);

	m_drone = Drone::create(drone_props);
	m_drone->init(m_spotLight2, m_enemies);
	m_drones.push_back(m_drone);


	//************Shooter Enemy************

	engine::ref<engine::model> shooter_model = engine::model::create("assets/models/static/sam_obj/sam_2.obj");

	engine::game_object_properties shooter_props;
	shooter_props.meshes = shooter_model->meshes();
	shooter_props.textures = shooter_model->textures();
	float shooter_scale = 3.f;
	shooter_props.position = { 30.f,10.f,0.f };
	shooter_props.scale = glm::vec3(shooter_scale, shooter_scale, shooter_scale);
	shooter_props.bounding_shape = shooter_model->size() / 2.f * shooter_scale;


	for (int i = 0; i < 10; i++)
	{
		shooter_props.position = { 30 + (std::rand() % 3) * i, 8.f, -10.f + (std::rand() % 4) * i };
		m_shooter_enemy = Shooter_enemy::create(shooter_props);
		m_shooter_enemy->init(1.f, 5.f, 60.f, 40.f);
		m_shooter_enemies.push_back(m_shooter_enemy);
	}

	engine::ref<engine::sphere> bullet_Shape = engine::sphere::create(32, 32, 0.5f);

	engine::game_object_properties shooter_bullet_props;
	shooter_bullet_props.position = glm::vec3(100.f, 100.f, 100.f);
	shooter_bullet_props.meshes = { bullet_Shape->mesh() };

	for (int i = 0; i < 40; i++)
	{

		m_shooter_bullet = Enemy_bullet::create(shooter_bullet_props);
		m_shooter_bullet->init(15.f, 4.f);
		m_shooter_bullets.push_back(m_shooter_bullet);
	}

	//************Enemy Pickups************
	engine::ref<engine::cuboid> pickup_shape = engine::cuboid::create(glm::vec3(0.5f), false);
	engine::ref<engine::texture_2d> pickup_texture_health = engine::texture_2d::create("assets/textures/medkit.jpg", true);
	engine::ref<engine::texture_2d> pickup_texture_ammo = engine::texture_2d::create("assets/textures/ammo.jpg", true);

	engine::game_object_properties pickup_props_health;
	pickup_props_health.position = glm::vec3(100.f,100.f,100.f);
	pickup_props_health.meshes = { pickup_shape->mesh() };
	pickup_props_health.textures = { pickup_texture_health };
	pickup_props_health.scale = glm::vec3(1.f);

	engine::game_object_properties pickup_props_ammo;
	pickup_props_ammo.position = glm::vec3(100.f, 100.f, 100.f);
	pickup_props_ammo.meshes = { pickup_shape->mesh() };
	pickup_props_ammo.textures = { pickup_texture_ammo };
	pickup_props_ammo.scale = glm::vec3(1.f);



	for (int i = 0; i < 20; i++)
	{
		m_health_pickup = pickup::create(pickup_props_health);

		m_health_pickup->init(glm::vec3(100.f), pickup::type::health, 20);
		m_pickups.push_back(m_health_pickup);

		m_ammo_pickup = pickup::create(pickup_props_ammo);

		m_ammo_pickup->init(glm::vec3(100.f), pickup::type::ammo, 16);
		m_pickups.push_back(m_ammo_pickup);
	}

	//************Lighting************
	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(20, 40, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { -2.5f, 0.0f, -2.5f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.000001f;
	m_ball = engine::game_object::create(sphere_props);


	m_game_objects.push_back(m_terrain);
	//m_game_objects.push_back(m_rocket);

	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();
}

example_layer::~example_layer() {}



void example_layer::on_update(const engine::timestep& time_step) 
{

	if (!m_3d_camera.in_cutscene())
	{

		for (int i = 0; i < m_pickups.size(); i++)
		{
			if (m_pickups.at(i)->active())
			{
				m_pickups.at(i)->update((float)time_step, m_player, m_audio_manager);
			}
		}
	}
	//Changing the alpha, this is for the intro scene
	//This is used to make the flashing effect for the text
	alpha += alpha_increment;
	if (alpha >= 1.0f) {
		alpha_increment = -0.01f;
	}
	else if (alpha <= 0.2f) {
		alpha_increment = 0.01f;
	}

    m_3d_camera.on_update(time_step);

	m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));
	m_fire_effect->on_update(time_step);
	//This is to make sure that the rocket stays stationary during the intro screen
	if (!m_intro_scene) {
		m_rocket->update(time_step);
		if (!m_fire_effect->active())
		{

			m_fire_effect->activate(m_rocket->position()+glm::vec3(0.f,3.f,-2.5f), 4.f, 4.f);
		}
	}

		
	//************Player************
	if (!m_intro_scene && !m_3d_camera.in_cutscene() && !m_3d_camera.minimap() && !m_in_shop)
	{
		m_player->update((float)time_step, m_3d_camera, m_audio_manager);
		//Collision
		if (m_player->player_collider().collision(m_ramp_box))
		{
			m_player->set_grounded(true);
			m_player->ramp_response(glm::vec3(0.f, 3.f, 0.f));
		}





			for (int i = 0; i < m_enemies.size(); i++)
			{ 

				if (m_enemies.at(i)->active())
				{
					m_enemies.at(i)->on_update((float)time_step, m_player->position(), m_player);
				}
			}

			//************Shooter enemy************
			if (m_level == 1)
			{

				for (int i = 0; i < m_shooter_enemies.size(); i++)
				{

					if (m_shooter_enemies.at(i)->active())
					{

						m_shooter_enemies.at(i)->on_update((float)time_step, m_player->position(), m_player);
						if (m_shooter_enemies.at(i)->current_state() == Shooter_enemy::state::attack && m_shooter_enemies.at(i)->can_attack())
						{
							for (int j = 0; j < m_shooter_bullets.size(); j++)
							{
								if (!m_shooter_bullets.at(j)->active())
								{

									m_shooter_bullets.at(j)->shoot(m_shooter_enemy->position(), m_player->position());
									m_shooter_bullets.at(j)->set_active(true);
									m_shooter_enemies.at(i)->set_attack(false);
								}
							}
						}
					}
					else
					{
						m_shooter_enemies.at(i)->reset();
					}
					for (int i = 0; i < m_shooter_bullets.size(); i++)
					{

							m_shooter_bullets.at(i)->on_update((float)time_step);

							if (m_shooter_bullets.at(i)->collider().box_collision(m_player->player_collider()))
							{
								m_player->take_damage(1.f, glm::vec3(0.f), 0.f);
								m_shooter_bullets.at(i)->set_active(false);
							}
						
					}

				}
			}
	//m_shooter_enemy->on_update((float)time_step, m_player->position());

	//TODO: Replace this check with a collision test


		for (int i = 0; i < m_enemies.size(); i++)
		{
			if (m_enemies.at(i)->active())
			{

				if (m_enemies.at(i)->current_state() == Melee_enemy::state::attack)
				{

					if (m_enemies.at(i)->can_attack())
					{

						m_melee_enemy->attack(m_player);
					}
			
				}
			}
		}
	

		//**********Bullets*********
		for (int i = 0; i < m_bullets.size(); i++)
		{
			if (m_bullets.at(i)->active())
			{

				m_bullets.at(i)->on_update((float)time_step);

			}
		}

		m_explosion_effect->on_update(time_step);

		for (int i = 0; i < m_bullets.size(); i++)
		{
			if (m_bullets.at(i)->active())
			{
				for (int j = 0; j < m_enemies.size(); j++)
				{
					if ((glm::distance(m_bullets.at(i)->object()->position(), m_enemies.at(j)->position())) <= 5.f && m_enemies.at(j)->active())
					{

						if (m_bullets.at(i)->bullet_collider().box_collision(m_enemies.at(j)->enemy_collider()))
						{
							m_explosion_effect->activate(m_bullets.at(i)->object()->position(), 4, 4);
							m_enemies.at(j)->lowerHealth(m_player->damage());
							m_audio_manager->play("hit");
							m_bullets.at(i)->set_active(false);
						}
					}
				}

					for (int j = 0; j < m_shooter_enemies.size(); j++)
					{

						if (m_bullets.at(i)->bullet_collider().box_collision(m_shooter_enemies.at(j)->enemy_collider())&& m_shooter_enemies.at(j)->active())
						{
							m_explosion_effect->activate(m_bullets.at(i)->object()->position(), 4, 4);
							m_shooter_enemies.at(j)->lowerHealth(m_player->damage());
							m_audio_manager->play("hit");
						}
					}
					for (int j = 0; j < m_drones.size(); j++)
					{

						if (m_bullets.at(i)->bullet_collider().box_collision(m_drones.at(j)->drone_collider() )&& m_drones.at(j)->active())
						{
							m_explosion_effect->activate(m_bullets.at(i)->object()->position(), 4, 4);
							m_drones.at(j)->set_active(false);
						}
					}
			}
		}

	}
	m_pointLight.Position = m_player->position()+glm::vec3(0.f,50.f,0.f);

	if (m_3d_camera.minimap())
	{
		glm::vec3 offset_pos(m_player->position() + glm::vec3(0.f, 80.f, 0.f));
		m_3d_camera.set_view_matrix(offset_pos, m_player->position());
	}

	//A simple timer, to allow the player to move the camera once the intro cutscene is over
	if(!m_intro_scene && m_3d_camera.in_cutscene())
	{
		if(m_intro_timer>0.0f)
		{
			m_intro_timer -= (float)time_step;
			if (m_intro_timer < 0.0f)
			{
				m_3d_camera.set_in_cutscene(false);
				//Spawn point of the player
				m_3d_camera.position(glm::vec3(150.f, 2.f, -30.f));
			}
		}
	}

	//****Level1 ending******
	if (m_level == 0)
	{
		//If all enemies on level 1 are defeated,  move to shop
		for (int i = 0; i < m_enemies.size(); i++)
		{
			m_all_killed = true;
			if (m_enemies.at(i)->active())
			{
				m_all_killed = false;
				break;
			}
		}
		if (m_all_killed)
		{
			m_level = 1;
			m_in_shop = true;
		}
	}
	else if (m_level == 1 && !m_3d_camera.in_cutscene() && !m_in_shop)
	{
		if (level_two_timer > 0.0f)
		{
			level_two_timer -= (float)time_step;

			if (level_two_timer < 0.0f)
			{
				level_two_timer = level_two_time;
				m_level = 2;
				m_in_shop = true;
			}
		}
	}

	if (!m_3d_camera.in_cutscene() && !m_3d_camera.minimap() && !m_in_shop)
	{
		if (m_level == 2)
		{
			for (int i = 0; i < m_drones.size(); i++)
			{
				if (m_drones.at(i)->active())
				{

					m_drone->on_update((float)time_step, m_player->position());
					if (m_drone->drone_detector().box_collision(m_player->player_collider()))
					{
						m_drone->set_player_detected(true);
					}
				}
			}
		}
	}


}





//*************		Level 1 renderer	*************/

void example_layer::level_one_render()
{

	/*GUI RENDERING*/
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	if (m_intro_scene)
	{
		m_text_manager->render_text(text_shader,
			"Press 'Enter' To start the game",
			(float)engine::application::window().width() / 2 - 350.f, (float)engine::application::window().height() / 2, 1.0f,
			glm::vec4(.8f, .4f, .0f, alpha));
	}
	else
	{

		//Health on top left
		m_text_manager->render_text(text_shader,
			//x = +50
			//y -20
			std::to_string(m_player->health()),
			50.f,
			(float)engine::application::window().height() - 35.f,
			.75f,
			glm::vec4(.8f, .4f, .0f, 1.f));

		if (m_level == 1)
		{

			m_text_manager->render_text(text_shader,
				//x = +50
				//y -20
				"Time: 00:"+std::to_string(((int)level_two_timer)),
				(float)engine::application::window().width()/2 - 125.f,
				(float)engine::application::window().height() - 35.f,
				.75f,
				glm::vec4(.8f, .4f, .0f, 1.f));
		}
		//Score in middle(slightly to the right
		m_text_manager->render_text(text_shader,
			//x = +750 total
			"Score: " + std::to_string(m_player->score()),
			(float)engine::application::window().width() / 2 + 125.f,
			(float)engine::application::window().height() - 35.f,
			.75f,
			glm::vec4(.8f, .4f, .0f, 1.f));

		//Coins on the top right
		m_text_manager->render_text(text_shader,
			//x = 1140
			//halfway = 625
			std::to_string(m_player->coins()),
			(float)engine::application::window().width() / 2 + 515.f,
			(float)engine::application::window().height() - 35.f,
			.75f,
			glm::vec4(.8f, .4f, .0f, 1.f));


		//Bullets on bottom right
		m_text_manager->render_text(text_shader,
			//x =1050?-1060
			//y = -690
			m_player->current_weapon()->toString(),
			(float)engine::application::window().width() / 2 + 425.f,
			15.f,
			1.0f,
			glm::vec4(.8f, .4f, .0f, 1.f));



	}

	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	//Rocket rendering
	if (m_rocket->active())
	{
		


		glm::mat4 rocket_transform(1.f);

		if (m_3d_camera.in_cutscene())
		{
			
			glm::vec3 offset_pos(m_rocket->position() + glm::vec3(60.f, 2.f, 0.f));
			m_3d_camera.set_view_matrix(offset_pos, m_rocket->position());

		}
		//m_3d_camera.set_view_matrix(m_rocket->position() + glm::vec3(1.f, 1.f, 1.f), m_rocket->position());
		rocket_transform = glm::translate(rocket_transform, m_rocket->position());
		rocket_transform = glm::rotate(rocket_transform, m_rocket->rotation_amount() + 45.f, glm::vec3(1.f, 0.f, 0.f));
		rocket_transform = glm::scale(rocket_transform, glm::vec3(0.1f, 0.1f, 0.1f));

		engine::renderer::submit(textured_lighting_shader, rocket_transform, m_rocket);

		
	}
	m_fire_effect->on_render(m_3d_camera, textured_lighting_shader);
	//*******Signs********

	if (m_level == 0)
	{

		glm::mat4 sign_transform1 = glm::mat4(1.f);
		sign_transform1 = glm::translate(sign_transform1, glm::vec3(140.f, 0.5f, -30.f));
		sign_transform1 = glm::rotate(sign_transform1, engine::PI, glm::vec3(0.f, 1.f, 0.f));

		engine::renderer::submit(textured_lighting_shader, sign_transform1, m_sign);

		glm::mat4 sign_transform2 = glm::mat4(1.f);
		sign_transform2 = glm::translate(sign_transform2, glm::vec3(100.f, 0.5f, -30.f));
		sign_transform2 = glm::rotate(sign_transform2, engine::PI, glm::vec3(0.f, 1.f, 0.f));

		engine::renderer::submit(textured_lighting_shader, sign_transform2, m_sign);

		glm::mat4 sign_transform3 = glm::mat4(1.f);
		sign_transform3 = glm::translate(sign_transform3, glm::vec3(80.f, 0.5f, -30.f));
		sign_transform3 = glm::rotate(sign_transform3, engine::PI, glm::vec3(0.f, 1.f, 0.f));

		engine::renderer::submit(textured_lighting_shader, sign_transform3, m_sign);

	}

	//Bullets on every level

	for (int i = 0; i < m_bullets.size(); i++)
	{
		//std::cout << "Bullet position outside class " << m_bullets.at(i)->position();
		//engine::renderer::submit(textured_lighting_shader,m_bullets.at(i)->bullet_transform(), m_bullets.at(i));
		if (m_bullets.at(i)->active())
		{
			m_bullets.at(i)->on_render(textured_lighting_shader);
		}
	}

	m_explosion_effect->on_render(m_3d_camera, textured_lighting_shader);

	//The ramp next to the rocket needed on every level
	engine::renderer::submit(textured_lighting_shader, m_ramp);

	//Player's render, used for debugging only
	m_player->on_render(textured_lighting_shader);


	//***********Enemies for level 1***********
	//Render these enemies on level 1, maybe on level 2 as well??
	if (m_level == 0)
	{


		for (int i = 0; i < m_enemies.size(); i++)
		{
			if (m_enemies.at(i)->active())
			{

				//m_enemies.at(i)->on_render(textured_lighting_shader);
				engine::renderer::submit(textured_lighting_shader, m_enemies.at(i));
			}
			else if (!m_enemies.at(i)->pickup_spawned() && !m_enemies.at(i)->active())
			{
				std::cout << "Enemy not active, and pickup was not spawned";
				//int type = m_enemies.at(i)->spawn_pickup();
				int type = std::rand() % 3;
				for (int j = 0; j < m_pickups.size(); j++)
				{
					std::cout << "j: " << j;
					std::cout << "type = " << type << "\n";
					if (type == 0 && m_pickups.at(j)->pickup_type() == pickup::type::health && !m_pickups.at(j)->active())
					{
						std::cout << "Does it go to here, or no?";
						m_pickups.at(j)->set_position(m_enemies.at(i)->position());
						m_pickups.at(j)->set_active(true);
						m_enemies.at(i)->spawn_pickup();
						break;

					}
					else if (type == 1 && m_pickups.at(j)->pickup_type() == pickup::type::ammo && !m_pickups.at(j)->active())
					{
						m_pickups.at(j)->set_position(m_enemies.at(i)->position());
						m_pickups.at(j)->set_active(true);
						m_enemies.at(i)->spawn_pickup();
						break;
					}
				}
			}
		}
	}

	//Pickups needed in every level
	
	for (int i = 0; i < m_pickups.size(); i++)
	{
		if (m_pickups.at(i)->active())
		{
			engine::renderer::submit(textured_lighting_shader, m_pickups.at(i));
		}
	}



	//***********Drone enemies for level 3***********
	//This will only be needed on level 3
	if (m_level == 2)
	{
		for (int i = 0; i < m_drones.size(); i++)
		{

			engine::renderer::submit(textured_lighting_shader, m_drones.at(i));
			m_drones.at(i)->on_render(textured_lighting_shader);
		}
	}

	//***********Shooter Enemies for level 2***********
	if (m_level == 1)
	{
		for (int i = 0; i < m_shooter_enemies.size(); i++)
		{
			if (m_shooter_enemies.at(i)->active())
			{
				m_shooter_enemies.at(i)->on_render(textured_lighting_shader);

				engine::renderer::submit(textured_lighting_shader, m_shooter_enemies.at(i));
			}
		}

		for (int i = 0; i < m_shooter_bullets.size(); i++)
		{
			if (m_shooter_bullets.at(i)->active()) {

				engine::renderer::submit(textured_lighting_shader, m_shooter_bullets.at(i));
			}
		}
	}

	//needed every level for minimap
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	m_spotLight.submit(textured_lighting_shader, 0);
	m_spotLight1.submit(textured_lighting_shader, 1);
	m_spotLight2.submit(textured_lighting_shader, 2);

	//engine::renderer::submit(textured_lighting_shader, m_shooter_enemy);

	//Gun redndering, needed every level

	//Camera's position
	glm::vec3 pos = m_3d_camera.position();
	//The vector the camera's facing
	glm::vec3 forward = m_3d_camera.front_vector();
	//The strafing vector for the camera
	glm::vec3 right = m_3d_camera.right_vector();
	//Check whether this weapon is active or not, and only render it if it is active

	if (m_guns.at(0)->active() && !m_3d_camera.minimap())
	{


		//The angle to rotate the gun sideways
		float theta = (engine::PI / 2.0f - acos(forward.y)) - engine::PI / 2;
		//The angle to rotate the gun up and down
		float phi = atan2(forward.x, forward.z);

		//The position for the gun, with offsets in the y and x axis
		glm::vec3 p = pos + 0.5f * forward + 0.3f * right + glm::vec3(0.f, -.1f, 0.f);

		glm::mat4 gun_transform(1.f);
		gun_transform = glm::translate(gun_transform, p);
		gun_transform = glm::rotate(gun_transform, phi, glm::vec3(0.f, 1.f, 0.f));
		gun_transform = glm::rotate(gun_transform, -theta, glm::vec3(1.f, 0.f, 0.f));
		gun_transform = glm::scale(gun_transform, glm::vec3(0.02f, 0.02f, 0.02f));

		engine::renderer::submit(textured_lighting_shader, gun_transform, m_gun);

	}
	else if (m_guns.at(1)->active() && !m_3d_camera.minimap())
	{

	
		//The angle to rotate the gun sideways
		float theta = (engine::PI / 2.0f - acos(forward.y));
		//The angle to rotate the gun up and down
		float phi = atan2(forward.x, forward.z);
		glm::vec3 p = pos + 0.5f * forward + 0.3f * right + glm::vec3(0.f, -.15f, 0.f);

		glm::mat4 rifle_transform(1.f);
		rifle_transform = glm::translate(rifle_transform, p);
		rifle_transform = glm::rotate(rifle_transform, phi, glm::vec3(0.f, 1.f, 0.f));
		rifle_transform = glm::rotate(rifle_transform, -theta, glm::vec3(1.f, 0.f, 0.f));
		rifle_transform = glm::scale(rifle_transform, glm::vec3(0.5f, 0.5f, 0.5f));

		engine::renderer::submit(textured_lighting_shader, rifle_transform, m_rifle);
	
	}

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	if (m_level == 2)
	{
		std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("fog_on", true);
	}

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
	m_pointLight.submit(textured_lighting_shader, 0);

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	m_spotLight.submit(textured_lighting_shader, 0);
	m_spotLight1.submit(textured_lighting_shader, 1);
	m_spotLight2.submit(textured_lighting_shader, 2);

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("lighting_on", false);

	m_spotLight.Position = m_drones.at(0)->position();
	m_spotLight1.Position = m_drones.at(1)->position();
	m_spotLight2.Position = m_drones.at(2)->position();
	m_lightsource_material->submit(textured_lighting_shader);
	engine::renderer::submit(textured_lighting_shader, m_ball->meshes().at(0),
		glm::translate(glm::mat4(1.f), m_pointLight.Position));

	//engine::renderer::submit(textured_lighting_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_drone->position()));

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->
		set_uniform("lighting_on", true);


		engine::renderer::end_scene();

	//HUD rendering
		engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);
		m_HUD->on_render(textured_lighting_shader, glm::vec3(0.f, 0.f, 0.1f));

		//m_shooter_enemy->on_render(textured_lighting_shader);

		engine::renderer::end_scene();

		const auto material_shader = engine::renderer::shaders_library()->get("mesh_material");
		engine::renderer::begin_scene(m_3d_camera, material_shader);

		m_bullet_material->submit(material_shader);

		m_material->submit(material_shader);
		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
		if (m_level == 2)
		{
			std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("fog_on", true);
		}

		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
		m_pointLight.submit(material_shader, 0);

		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
		m_spotLight.submit(material_shader, 0);
		m_spotLight1.submit(material_shader, 1);
		m_spotLight2.submit(material_shader, 2);

		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->set_uniform("lighting_on", false);

		m_lightsource_material->submit(material_shader);
		engine::renderer::submit(material_shader, m_ball->meshes().at(0),
			glm::translate(glm::mat4(1.f), m_pointLight.Position));

		
		engine::renderer::submit(material_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.f), m_drone->position()));

		std::dynamic_pointer_cast<engine::gl_shader>(material_shader)->
			set_uniform("lighting_on", true);


		engine::renderer::end_scene();


}





//***************Shop render***************


void example_layer::shop_render()
{
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	//Each stat gets a height section of 180, 

	//Health on top left
	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"Current health: " + std::to_string(m_player->health()),
		20.f,
		(float)engine::application::window().height() - 60.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));

	//Coins
	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"$: " + std::to_string(m_player->coins()),
		(float)engine::application::window().width()/2+20.f,
		(float)engine::application::window().height() - 60.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));
	//Health increase
	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"1 - $100 - Health + 10",
		20.f,
		(float)engine::application::window().height() - 110.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));

	//Current speed
	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"Current Speed: " + std::to_string(m_player->speed()),
		20.f,
		(float)engine::application::window().height() - 180.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));

	//Speed increase
	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"2 - $100 - Speed + 1",
		20.f,
		(float)engine::application::window().height() - 230.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));

	//Current damage
	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"Current Strength: " + std::to_string(m_player->damage()),
		20.f,
		(float)engine::application::window().height() - 300.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));

	//Damage increase
	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"3 - $100 - Strength + 5",
		20.f,
		(float)engine::application::window().height() - 350.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));



	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"$100 - Pistol ammo: +10 " + m_gun->toString(),
		20.f,
		(float)engine::application::window().height() - 470.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));

	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"$100 - Rifle ammo: +10 " + m_rifle->toString(),
		20.f,
		(float)engine::application::window().height() - 520.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));

	m_text_manager->render_text(text_shader,
		//x = +50
		//y -20
		"Press backspace to exit shop",
		(float)engine::application::window().width()/2+120.f,
		(float)engine::application::window().height() - 520.f,
		.75f,
		glm::vec4(.8f, .4f, .0f, 1.f));
}

void example_layer::on_render() 
{
    engine::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f}); 
    engine::render_command::clear();


	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_tranform);

	engine::renderer::submit(textured_lighting_shader, m_terrain);

	engine::renderer::submit(textured_lighting_shader, m_ball->meshes().at(0), glm::translate(glm::mat4(1.0f), m_player->position()+glm::vec3(0.f,50.f,0.f)));

    engine::renderer::end_scene();

	if (m_in_shop)
	{
	shop_render();

	}else 
	{
		//level_one_render();
		level_one_render();

	}
	//else if (m_level == 1)
	//{
	//	level_two_render();
	//}
	//else if (m_level == 2)
	//{
	//	level_three_render();
	//}




	
} 

void example_layer::on_event(engine::event& event) 
{ 
    if(event.event_type() == engine::event_type_e::key_pressed) 
    { 
        auto& e = dynamic_cast<engine::key_pressed_event&>(event); 
        if(e.key_code() == engine::key_codes::KEY_TAB) 
        { 
            engine::render_command::toggle_wireframe();
        }
		if(e.key_code() == engine::key_codes::KEY_ENTER && m_intro_scene)
		{
			m_intro_scene = false;
			m_3d_camera.set_in_cutscene(true);
			m_3d_camera.set_pitch(0.f);
		}

		//Debugging
		if (e.key_code() == engine::key_codes::KEY_P)
		{
			std::cout << "Current camera position: " << m_3d_camera.position();
		}

		if (e.key_code() == engine::key_codes::KEY_M)
		{
			m_3d_camera.set_minimap(!m_3d_camera.minimap());
		}

		//Debugging
		if (e.key_code() == engine::key_codes::KEY_L)
		{
			m_in_shop = !m_in_shop;
		}
		//Debugging
		if (e.key_code() == engine::key_codes::KEY_3)
		{
			m_fire_effect->activate(glm::vec3(100.f,2.f,-30.f), 20.f, 20.f);
		}

		if (m_in_shop)
		{
			if (e.key_code() == engine::key_codes::KEY_1)
			{
				if (m_player->coins() >= 100)
				{
					m_player->setCoins(m_player->coins() - 100);
					m_player->setHealth(m_player->health() + 10);
				}
			}
			if (e.key_code() == engine::key_codes::KEY_2)
			{
				if (m_player->coins() >= 100)
				{
					m_player->setCoins(m_player->coins() - 100);
					m_player->setSpeed(m_player->speed() + 1.f);
				}
			}
			if (e.key_code() == engine::key_codes::KEY_3)
			{
				if (m_player->coins() >= 100)
				{
					m_player->setCoins(m_player->coins() - 100);
					m_player->setDamage(m_player->damage() + 5.f);
				}
			}
			if (e.key_code() == engine::key_codes::KEY_4)
			{
				if (m_player->coins() >= 100)
				{
					m_player->setCoins(m_player->coins() - 100);
					//TODO: add set ammo function to weapons
				}
			}
			if (e.key_code() == engine::key_codes::KEY_5)
			{
				if (m_player->coins() >= 100)
				{
					m_player->setCoins(m_player->coins() - 100);
					m_player->setHealth(m_player->health() + 10);
				}
			}

			if (e.key_code() == engine::key_codes::KEY_BACKSPACE && m_in_shop)
			{
				m_in_shop = false;
			}
			//TODO: add exit button for shop, and when you exit shop, reset everything on the level
		}
    } 
}
