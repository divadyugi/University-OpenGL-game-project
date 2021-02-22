#include "pch.h"
#include "Ramp.h"
#include <engine.h>

engine::Ramp::Ramp(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	//Calculate normals
	std::vector<glm::vec3>normals;

	//Normals for the front
	normals.push_back(glm::normalize(glm::cross(vertices.at(0) - vertices.at(2),
		vertices.at(0) - vertices.at(4))));

	//Normals for the left
	normals.push_back(glm::normalize(glm::cross(vertices.at(5) - vertices.at(3),
		vertices.at(5) - vertices.at(4))));

	//Normals for the right
	normals.push_back(glm::normalize(glm::cross(vertices.at(0) - vertices.at(1),
		vertices.at(0) - vertices.at(2))));

	//Normals for the bottom
	normals.push_back(glm::normalize(glm::cross(vertices.at(1) - vertices.at(3),
		vertices.at(1) - vertices.at(4))));



	//Create a vector of vertices

	std::vector<mesh::vertex> ramp_vertices
	{
		//Front
		{	{vertices.at(0)},	{normals.at(0)}, {0.f,0.f}	},
		{	{vertices.at(2)},	{normals.at(0)}, {1.f,0.f}	},
		{	{vertices.at(4)},	{normals.at(0)}, {1.f,1.f}	},
		{	{vertices.at(5)},	{normals.at(0)}, {0.f,1.f}	},

		//left
		{	{vertices.at(5)},	{normals.at(1)}, {0.f,0.5f}	},
		{	{vertices.at(4)},	{normals.at(1)}, {1.f,0.f}	},
		{	{vertices.at(3)},	{normals.at(1)}, {1.f,1.f}	},
		//right
		{	{vertices.at(0)},	{normals.at(2)}, {0.f,0.5f}	},
		{	{vertices.at(1)},	{normals.at(2)}, {1.f,0.f}	},
		{	{vertices.at(2)},	{normals.at(2)}, {1.f,1.f}	},
		//bottom
		{	{vertices.at(1)},	{normals.at(3)}, {0.f,0.f}	},
		{	{vertices.at(3)},	{normals.at(3)}, {1.f,0.f}	},
		{	{vertices.at(4)},	{normals.at(3)}, {1.f,1.f}	},
		{	{vertices.at(2)},	{normals.at(3)}, {0.f,1.f}	},
	};

	//The indices
	const std::vector<uint32_t> ramp_indices
	{
		//front
		0,1,2,		0,2,3,
		//left
		4,5,6,
		//right
		7,8,9,
		//bottom
		10,11,12,	10,12,13
	};

	m_mesh = engine::mesh::create(ramp_vertices, ramp_indices);
}

engine::Ramp::~Ramp() {}

engine::ref<engine::Ramp> engine::Ramp::create(std::vector<glm::vec3> vertices)
{
	return std::make_shared<engine::Ramp>(vertices);
}


