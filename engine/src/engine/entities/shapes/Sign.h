#pragma once

namespace engine
{
	class mesh;

	class Sign
	{
	public:
		Sign(std::vector<glm::vec3> board_vertices, std::vector<glm::vec3> post_vertices);

		~Sign();

		std::vector<glm::vec3> board_vertices() const { return m_board_vertices; }
		std::vector<glm::vec3> post_vertices() const { return m_post_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<Sign> create(std::vector<glm::vec3> board_vertices, std::vector<glm::vec3> post_vertices);

	private:

		std::vector<glm::vec3> m_board_vertices;

		std::vector<glm::vec3> m_post_vertices;


		ref<engine::mesh> m_mesh;
	};
}
