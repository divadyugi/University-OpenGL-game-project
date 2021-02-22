#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h"


namespace engine
{

	class Bounding_sphere
	{

	public:

		//Constructor
		Bounding_sphere();

		//Destructor
		~Bounding_sphere();

		//getter for values used in collision
		void get(glm::vec3& center, float& raidus);

		//Used to create the sphere
		void set_sphere(glm::vec3 center, float radius);

		//Update function to place the circle to target position every frame
		void on_update(glm::vec3 position);

		//Render function used for debugging
		void on_render(float r, float g, float b, const engine::ref<engine::shader>& shader);

		//Check for collision between two spheres
		bool collision(Bounding_sphere other_sphere);

		bool box_collision(bounding_box other_box);


	private:

		glm::vec3 m_center;

		float m_radius;

		engine::ref<engine::mesh> m_mesh;

	};

}
