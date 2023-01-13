#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a hexcone object with a mesh of a specified size
	class hexcone
	{
	public:
		/// \brief Constructor
		hexcone(std::vector<glm::vec3> vertices);

		/// \brief Destructor
		~hexcone();

		static engine::ref<engine::hexcone> create();

		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

	private:
		/// \brief Fields
		// vertices of the hexcone specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}

