#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a tetrahedron object with a mesh of a specified size
	class plus
	{
	public:
		/// \brief Constructor
		plus(std::vector<glm::vec3> vertices);

		/// \brief Destructor
		~plus();

		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

		static ref<plus> create(float size);
	private:
		/// \brief Fields
		// vertices of the tetrahedron specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}

#pragma once
