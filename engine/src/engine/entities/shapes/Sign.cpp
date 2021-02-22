#include "pch.h"
#include "Sign.h"
#include <engine.h>

engine::Sign::Sign(std::vector<glm::vec3> board_vertices, std::vector<glm::vec3> post_vertices) : m_board_vertices(board_vertices), m_post_vertices(post_vertices)
{
	//Calculate the normals

	std::vector<glm::vec3>board_normals;
	board_normals.push_back(glm::normalize(glm::cross(board_vertices.at(7) - board_vertices.at(0),
		board_vertices.at(7) - board_vertices.at(2))));

	board_normals.push_back(glm::normalize(glm::cross(board_vertices.at(7) - board_vertices.at(5),
		board_vertices.at(7) - board_vertices.at(4))));

	board_normals.push_back(glm::normalize(glm::cross(board_vertices.at(0) - board_vertices.at(1),
		board_vertices.at(0) - board_vertices.at(3))));

	board_normals.push_back(glm::normalize(glm::cross(board_vertices.at(6) - board_vertices.at(4),
		board_vertices.at(6) - board_vertices.at(3))));

	board_normals.push_back(glm::normalize(glm::cross(board_vertices.at(5) - board_vertices.at(2),
		board_vertices.at(5) - board_vertices.at(3))));

	board_normals.push_back(glm::normalize(glm::cross(board_vertices.at(7) - board_vertices.at(6),
		board_vertices.at(7) - board_vertices.at(1))));

	std::vector<mesh::vertex> sign_vertices
	{
		//Front board
		//	Position				Normal				Texture
		{	{board_vertices.at(7)},	{board_normals.at(0)}, {0.f,0.f}	},
		{	{board_vertices.at(5)},	{board_normals.at(0)}, {0.f,0.2f}	},
		{	{board_vertices.at(2)},	{board_normals.at(0)}, {0.2f,0.f}	},
		{	{board_vertices.at(0)},	{board_normals.at(0)}, {0.2f,0.2f}	},

		//Front post
		{	{post_vertices.at(0)},	{board_normals.at(0)}, {0.f,0.f}	},
		{	{post_vertices.at(3)},	{board_normals.at(0)}, {0.f,0.2f}	},
		{	{post_vertices.at(2)},	{board_normals.at(0)}, {0.2f,0.f}	},
		{	{post_vertices.at(1)},	{board_normals.at(0)}, {0.2f,0.2f}	},

		//Left board
		{	{board_vertices.at(7)},	{board_normals.at(1)}, {0.f,0.f}	},
		{	{board_vertices.at(6)},	{board_normals.at(1)}, {0.f,0.2f}	},
		{	{board_vertices.at(4)},	{board_normals.at(1)}, {0.2f,0.f}	},
		{	{board_vertices.at(5)},	{board_normals.at(1)}, {0.2f,0.2f}	},

		//Left post
		{	{post_vertices.at(0)},	{board_normals.at(0)}, {0.f,0.f}	},
		{	{post_vertices.at(7)},	{board_normals.at(0)}, {0.f,0.2f}	},
		{	{post_vertices.at(6)},	{board_normals.at(0)}, {0.2f,0.f}	},
		{	{post_vertices.at(3)},	{board_normals.at(0)}, {0.2f,0.2f}	},
		//Right board
		{	{board_vertices.at(0)},	{board_normals.at(2)}, {0.f,0.f}	},
		{	{board_vertices.at(2)},	{board_normals.at(2)}, {0.2f,0.f}	},
		{	{board_vertices.at(3)},	{board_normals.at(2)}, {0.f,0.2f}	},
		{	{board_vertices.at(1)},	{board_normals.at(2)}, {0.2f,0.2f}	},

		
		/*//Right post
		{	{post_vertices.at(1)},	{board_normals.at(0)}, {0.f,0.f}	},
		{	{post_vertices.at(4)},	{board_normals.at(0)}, {0.f,0.2f}	},
		{	{post_vertices.at(5)},	{board_normals.at(0)}, {0.2f,0.f}	},
		{	{post_vertices.at(2)},	{board_normals.at(0)}, {0.2f,0.2f}	},*/
		//Back board
		{	{board_vertices.at(6)},	{board_normals.at(3)}, {0.f,1.f}	},
		{	{board_vertices.at(1)},	{board_normals.at(3)}, {1.f,1.f}	},
		{	{board_vertices.at(3)},	{board_normals.at(3)}, {1.f,0.f}	},
		{	{board_vertices.at(4)},	{board_normals.at(3)}, {0.f,0.f}	},

		//Back post
		{	{post_vertices.at(7)},	{board_normals.at(0)}, {0.f,0.f}	},
		{	{post_vertices.at(6)},	{board_normals.at(0)}, {0.f,0.2f}	},
		{	{post_vertices.at(5)},	{board_normals.at(0)}, {0.2f,0.f}	},
		{	{post_vertices.at(4)},	{board_normals.at(0)}, {0.2f,0.2f}	},
		//Top board
		{	{board_vertices.at(5)},	{board_normals.at(4)}, {0.f,0.f}	},
		{	{board_vertices.at(4)},	{board_normals.at(4)}, {0.2f,0.f}	},
		{	{board_vertices.at(3)},	{board_normals.at(4)}, {0.f,0.2f}	},
		{	{board_vertices.at(2)},	{board_normals.at(4)}, {0.2f,0.2f}	},

		//Bottom board
		{	{board_vertices.at(7)},	{board_normals.at(5)}, {0.f,0.f}	},
		{	{board_vertices.at(0)},	{board_normals.at(5)}, {0.2f,0.f}	},
		{	{board_vertices.at(1)},	{board_normals.at(5)}, {0.f,0.2f}	},
		{	{board_vertices.at(6)},	{board_normals.at(5)}, {0.2f,0.2f}	},
	};

	const std::vector<uint32_t> sign_indices
	{
		//front board
		0,1,2, 0,2,3,

		//left board
		4,5,6,	4,6,7,

		//right board
		8,9,10, 8,10,11,

		//back board
		12,13,14, 12,14,15,

		//top board
		16,17,18, 16,18,19,

		//bottom board
		20,21,22, 20,22,23
	};

	m_mesh = engine::mesh::create(sign_vertices, sign_indices);
}

engine::Sign::~Sign() {}

engine::ref<engine::Sign> engine::Sign::create(std::vector<glm::vec3> board_vertices, std::vector<glm::vec3> post_vertices)
{
	return std::make_shared<engine::Sign>(board_vertices, post_vertices);
}

