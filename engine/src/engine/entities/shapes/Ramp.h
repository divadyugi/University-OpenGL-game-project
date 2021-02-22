#pragma once

namespace engine
{
	class mesh;

	class Ramp
	{
	public:
		Ramp(std::vector<glm::vec3> vertices);

		~Ramp();

		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh;}

		static ref<Ramp> create(std::vector<glm::vec3> vertices);

	private:

		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}
