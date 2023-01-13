#include "pch.h"
#include "hexcone.h"
#include <engine.h>		

//Creates hexagon based cone primative 
engine::hexcone::hexcone(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	std::vector<glm::vec3> normals;
	// front normal
	normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(5)));
	// Top left normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(3), vertices.at(6) - vertices.at(2)));
	// TopMid normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(2), vertices.at(6) - vertices.at(1)));
	// TopRight normal
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(1), vertices.at(6) - vertices.at(0)));
	//BottomRight
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(0), vertices.at(6) - vertices.at(5)));
	//Bottom
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(5), vertices.at(6) - vertices.at(4)));
	//BottomLeft
	normals.push_back(glm::cross(vertices.at(6) - vertices.at(4), vertices.at(6) - vertices.at(3)));


	std::vector<mesh::vertex> hexcone_vertices
	{
		//front
		//  position			normal			      tex coord
		{ vertices.at(0),		normals.at(0),		{ 0.66f,  1.0f } },//0
		{ vertices.at(1),		normals.at(0),		{ 0.33f,  1.0f } },//1
		{ vertices.at(2),		normals.at(0),		{ 0.f,    0.5f } },//2
		{ vertices.at(3),		normals.at(0),		{ 0.33f,  0.0f } },//3
		{ vertices.at(4),		normals.at(0),		{ 0.66f,  0.0f } },//4
		{ vertices.at(5),		normals.at(0),		{ 0.99f,  0.5f } },//5

		//Back

		//Top left
		{ vertices.at(2),		normals.at(1),		{ 1.f,  1.f } },//6
		{ vertices.at(6),		normals.at(1),		{ 0.f,  0.5f } },//7
		{ vertices.at(3),		normals.at(1),		{ 0.f, 1.f } },//8

		//Top
		{ vertices.at(1),		normals.at(2),		{ 1.f,  1.f } },//9
		{ vertices.at(6),		normals.at(2),		{ 0.f,  0.5f } },//10
		{ vertices.at(2),		normals.at(2),		{ 0.f, 1.f } },//11

		//TopRight
		{ vertices.at(0),		normals.at(3),		{ 1.f,  1.f } },//12
		{ vertices.at(6),		normals.at(3),		{ 0.f,  0.5f } },//13
		{ vertices.at(1),		normals.at(3),		{ 0.f, 1.f } },//14

		//BottomRight
		{ vertices.at(5),		normals.at(4),		{ 1.f,  1.f } },//15
		{ vertices.at(6),		normals.at(4),		{ 0.f,  0.5f } },//16
		{ vertices.at(0),		normals.at(4),		{ 0.f, 1.f } },//17

		//Bottom
		{ vertices.at(4),		normals.at(5),		{ 1.f,  1.f}  },//18
		{ vertices.at(6),		normals.at(5),		{ 0.f,  0.5f } },//19
		{ vertices.at(5),		normals.at(5),		{ 0.f, 1.f } },//20

		//Bottom Left
		{ vertices.at(3),		normals.at(6),		{ 1.f,  1.f  } },//21
		{ vertices.at(6),		normals.at(6),		{ 0.f,  0.5f } },//22
		{ vertices.at(4),		normals.at(6),		{ 0.f, 1.f } },//23
		
	};

	const std::vector<uint32_t> hexcone_indices
	{
		//Front
		0,1,2,
		2,3,0,
		3,4,5,
		5,0,3,
		//TL
		6,7,8,
		//T
		9,10,11,
		//TR
		12,13,14,
		//BR
		15,16,17,
		//B
		18,19,20,
		//BL
		21,22,23 

	};
	m_mesh = engine::mesh::create(hexcone_vertices, hexcone_indices);
}

engine::hexcone::~hexcone() {}

engine::ref<engine::hexcone> engine::hexcone::create()
{
	std::vector<glm::vec3> vertices;
	for (int i = 0; i < 6; i++) {
		float theta = (i / 6.0f) * 2 * engine::PI;
		vertices.push_back(glm::vec3(cos(theta)/8,sin(theta)/8, 0.2f));
	}
	vertices.push_back(glm::vec3(0,0,-0.2f));

	return std::make_shared<engine::hexcone>(vertices);
}
