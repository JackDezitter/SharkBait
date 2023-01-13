#include "pch.h"
#include "prism.h"
#include <engine.h>		

//Creates triangular based prism primative
engine::prism::prism(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(2)));
	//Back normal
	normals.push_back(glm::cross(vertices.at(3) - vertices.at(5), vertices.at(3) - vertices.at(4)));
	// Left normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
	// Right normal
	normals.push_back(glm::cross(vertices.at(2) - vertices.at(1), vertices.at(2) - vertices.at(4)));
	// Bottom normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(1)));



	std::vector<mesh::vertex> prism_vertices
	{
		//front
		//  position			normal			      tex coord
		{ vertices.at(0),		normals.at(0),		{ 0,  0 } },//0
		{ vertices.at(1),		normals.at(0),		{ 0,  1.0f } },//1
		{ vertices.at(2),		normals.at(0),		{ 0.5f, 0.3f } },//2

		//Back
		{ vertices.at(3),		normals.at(1),		{ 0,  1.0f } },//3
		{ vertices.at(4),		normals.at(1),		{ 0.5f,  0.66f } },//4
		{ vertices.at(5),		normals.at(1),		{ 1.f,    1.f } },//5

		//left
		{ vertices.at(4),		normals.at(2),		{ 0.5f,  0.66f } },//6
		{ vertices.at(3),		normals.at(2),		{ 0,  1.0f } },//7
		{ vertices.at(0),		normals.at(2),		{ 0,  0 } },//8
		{ vertices.at(2),		normals.at(2),		{ 0.5f, 0.3f } },//9

		//right
		{ vertices.at(2),		normals.at(3),		{ 0.5f, 0.3f } },//10
		{ vertices.at(1),		normals.at(3),		{ 0,  1.0f } },//11
		{ vertices.at(5),		normals.at(3),		{ 1.f,    1.f } },//12
		{ vertices.at(4),		normals.at(3),		{ 0.5f,  0.66f } },//13

		//Bottom
		{ vertices.at(3),		normals.at(4),		{ 0,  1.0f } },//14
		{ vertices.at(5),		normals.at(4),		{ 1.f,    1.f } },//15
		{ vertices.at(1),		normals.at(4),		{ 0,  1.0f } },//16
		{ vertices.at(0),		normals.at(4),		{ 0,  0 }},//17

	};

	const std::vector<uint32_t> prism_indices
	{
		//Front
		0,1,2,
		//back
		3,4,5,
		//L
		4,3,0,
		4,0,2,
		//R
		2,1,5,
		2,5,4,
		//B
		3,5,1,
		3,1,0

	};
	m_mesh = engine::mesh::create(prism_vertices, prism_indices);
}

engine::prism::~prism() {}

engine::ref<engine::prism> engine::prism::create()
{
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(-0.2, 0, 0));
	vertices.push_back(glm::vec3(0.2, 0, 0));
	vertices.push_back(glm::vec3(0, 0.2, 0));

	vertices.push_back(glm::vec3(-0.3, 0, -0.4));
	vertices.push_back(glm::vec3(0, 0.3, -0.4));
	vertices.push_back(glm::vec3(0.3, 0, -0.5));

	return std::make_shared<engine::prism>(vertices);
}
