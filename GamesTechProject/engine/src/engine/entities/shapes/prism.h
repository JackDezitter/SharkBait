#pragma once

namespace engine
{
	class mesh;

	/// \brief Class creating a prism object with a mesh of a specified size
	class prism
	{
	public:
		/// \brief Constructor
		prism(std::vector<glm::vec3> vertices);

		/// \brief Destructor
		~prism();

		static engine::ref<engine::prism> create();

		/// \brief Getter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }

	private:
		/// \brief Fields
		// vertices of the prism specified by a vector of positions.
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}

