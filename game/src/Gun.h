#pragma once
#include <engine.h>



class Gun : public engine::game_object
{

public:

	//Constructor
	Gun(const engine::game_object_properties props);

	//Destructor
	~Gun();


	//Initialise a gun with the time between shots, max bullets, and mag size
	void init(float time_between_shots, float reload_time, int max_bullets, int mag_size);

	void update();

	//Shooting actions
	void reload();

	void shoot();

	static engine::ref<Gun> create(const engine::game_object_properties& props);

	//Getters and setters
	float& time_between_shots() { return m_time_between_shots; }

	void set_time_between_shots(float time_between_shots) { m_time_between_shots = time_between_shots; }

	float reload_time() { return m_reload_time; }

	void set_reload_time(float reload_time) { m_reload_time = reload_time; }

	//The amount of bullets currently in the chamber
	int bullets() { return m_bullets; }

	//The amount of total bullets left for current gun
	int current_mag() { return m_mag_bullets; }

	void set_current_mag(int amount) { m_mag_bullets += amount; }

	bool active() { return m_active; }



	//A to string function used to display the amount of ammo the gun has
	const std::string toString() { return std::to_string(m_bullets) + "/" + std::to_string(m_mag_bullets); }

	void set_active(bool active) { m_active = active; }

	//Timer functions

	//Used for resetting the attack timer
	void lower_time_attack(float difference) { m_time_between_shots -= difference; }

	//Function to reset the attack timer
	void reset_attack_timer() { m_time_between_shots = m_original_time; }

	//Used for resetting the reload timer
	void lower_reload_time(float difference) { m_reload_time -= difference; }

	//Function to reset the reload timer
	void reset_reload_timer() { m_reload_time = m_original_reload_time; }


private:

	//Checks if the gun is active

	bool m_active;

	//The time between each shot, make it smaller if you want the gun to shoot faster
	float m_time_between_shots;
	//Original_time used for resetting the timer
	float m_original_time;

	//The amount of seconds it takes to reload the gun
	float m_reload_time;
	//Original reload time, used for resetting the timer
	float m_original_reload_time;

	//Number of bullets currently in the chamber
	int m_bullets;

	//Maximum bullets a gun can hold
	int m_max_bullets;

	//The size of the mag for the gun
	int m_mag_size;

	//The total number of bullets left for the current gun

	int m_mag_bullets;

};
