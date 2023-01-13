#include "pch.h"
#include "plus.h"
#include <engine.h>		

engine::plus::plus(std::vector<glm::vec3> vertices) : m_vertices(vertices)
{
	float orientation = 1;
	std::vector<mesh::vertex> plus_vertices
	{
		//front
		//  position				normal						tex coord       
		{ vertices.at(0),		{ 0.0f, 0.0f, 1.0f},		{ 0.3f,  1.f } },//0
		{ vertices.at(1),		{ 0.0f, 0.0f, 1.0f},		{ 0.6f,  1.f } },//1
		{ vertices.at(2),		{ 0.0f, 0.0f, 1.0f},		{ 0.6f, 0.6f } },//2
		{ vertices.at(3),		{ 0.0f, 0.0f, 1.0f},		{ 0.1f, 0.6f } },//3
		{ vertices.at(4),		{ 0.0f, 0.0f, 1.0f},		{ 0.1f, 0.3f } },//4
		{ vertices.at(5),		{ 0.0f, 0.0f, 1.0f},		{ 0.6f, 0.3f } },//5
		{ vertices.at(6),		{ 0.0f, 0.0f, 1.0f},		{ 0.1f, 0.f } },//6
		{ vertices.at(7),		{ 0.0f, 0.0f, 1.0f},		{ 0.3f, 0.f } },//7
		{ vertices.at(8),		{ 0.0f, 0.0f, 1.0f},		{ 0.3f, 0.3f } },//8
		{ vertices.at(9),		{ 0.0f, 0.0f, 1.0f},		{ 0.f, 0.3f } },//9
		{ vertices.at(10),		{ 0.0f, 0.0f, 1.0f},		{ 0.3f, 1.f } },//10
		{ vertices.at(11),		{ 0.0f, 0.0f, 1.0f},		{ 0.3f, 0.6f } },//11

		//Back
		//  position				normal						tex coord       
		{ vertices.at(12),		{ 0.0f, 0.0f, -1.0f},		{ 0.3f,  0.f } },//12
		{ vertices.at(13),		{ 0.0f, 0.0f, -1.0f},		{ 0.6f,  0.f } },//13
		{ vertices.at(14),		{ 0.0f, 0.0f, -1.0f},		{ 0.6f, 0.3f } },//14
		{ vertices.at(15),		{ 0.0f, 0.0f, -1.0f},		{ 0.1f, 0.3f } },//15
		{ vertices.at(16),		{ 0.0f, 0.0f, -1.0f},		{ 0.1f, 0.6f } },//16
		{ vertices.at(17),		{ 0.0f, 0.0f, -1.0f},		{ 0.6f, 0.6f } },//17
		{ vertices.at(18),		{ 0.0f, 0.0f, -1.0f},		{ 0.1f, 1.f } },//18
		{ vertices.at(19),		{ 0.0f, 0.0f, -1.0f},		{ 0.3f, 1.f } },//19
		{ vertices.at(20),		{ 0.0f, 0.0f, -1.0f},		{ 0.3f, 0.6f } },//20
		{ vertices.at(21),		{ 0.0f, 0.0f, -1.0f},		{ 0.f, 0.6f } },//21
		{ vertices.at(22),		{ 0.0f, 0.0f, -1.0f},		{ 0.3f, 0.f } },//22
		{ vertices.at(23),		{ 0.0f, 0.0f, -1.0f},		{ 0.3f, 0.3f } },//23

		//Left facing
		//Up
		{ vertices.at(0),		{ 1.0f, 0.0f, 0.0f},		{ 1.f,  0.f } },//24
		{ vertices.at(23),		{ 1.0f, 0.0f, 0.0f},		{ 0.f,  0.f } },//25
		{ vertices.at(22),		{ 1.0f, 0.0f, 0.0f},		{ 0.f,  1.f } },//26
		{ vertices.at(1),		{ 1.0f, 0.0f, 0.0f},		{ 1.f, 1.f } },//27
		//Mid
		{ vertices.at(2),		{ 1.0f, 0.0f, 0.0f},		{ 1.f,  0.f } },//28
		{ vertices.at(21),		{ 1.0f, 0.0f, 0.0f},		{ 0.f,  0.f } },//29
		{ vertices.at(20),		{ 1.0f, 0.0f, 0.0f},		{ 0.f,  1.f } },//30
		{ vertices.at(3),		{ 1.0f, 0.0f, 0.0f},		{ 1.f, 1.f } },//31

		//Bot
		{ vertices.at(4),		{ 1.0f, 0.0f, 0.0f},		{ 1.f,  0.f } },//32
		{ vertices.at(19),		{ 1.0f, 0.0f, 0.0f},		{ 0.f,  0.f } },//33
		{ vertices.at(18),		{ 1.0f, 0.0f, 0.0f},		{ 0.f,  1.f } },//34
		{ vertices.at(5),		{ 1.0f, 0.0f, 0.0f},		{ 1.f, 1.f } },//35


		//Right Facing
		//Top
		{ vertices.at(12),		{ -1.0f, 0.0f, 0.0f},		{ 1.f,  0.f } },//36
		{ vertices.at(11),		{ -1.0f, 0.0f, 0.0f},		{ 0.f,  0.f } },//37
		{ vertices.at(10),		{ -1.0f, 0.0f, 0.0f},		{ 0.f,  1.f } },//38
		{ vertices.at(13),		{ -1.0f, 0.0f, 0.0f},		{ 1.f, 1.f } },//39

		//Mid
		{ vertices.at(14),		{ -1.0f, 0.0f, 0.0f},		{ 1.f,  0.f } },//40
		{ vertices.at(9),		{ -1.0f, 0.0f, 0.0f},		{ 0.f,  0.f } },//41
		{ vertices.at(8),		{ -1.0f, 0.0f, 0.0f},		{ 0.f,  1.f } },//42
		{ vertices.at(15),		{ -1.0f, 0.0f, 0.0f},		{ 1.f, 1.f } },//43

		//Down
		{ vertices.at(16),		{ -1.0f, 0.0f, 0.0f},		{ 1.f,  0.f } },//44
		{ vertices.at(7),		{ -1.0f, 0.0f, 0.0f},		{ 0.f,  0.f } },//45
		{ vertices.at(6),		{ -1.0f, 0.0f, 0.0f},		{ 0.f,  1.f } },//46
		{ vertices.at(17),		{ -1.0f, 0.0f, 0.0f},		{ 1.f, 1.f } },//47

		//Up Facing
		//UpRight
		{ vertices.at(9),		{ 0.0f, 1.0f, 0.0f},		{ 1.f,  0.f } },//48
		{ vertices.at(14),		{ 0.0f, 1.0f, 0.0f},		{ 0.f,  0.f } },//49
		{ vertices.at(13),		{ 0.0f, 1.0f, 0.0f},		{ 0.f,  1.f } },//50
		{ vertices.at(10),		{ 0.0f, 1.0f, 0.0f},		{ 1.f, 1.f } },//51

		//UpMid
		{ vertices.at(11),		{ 0.0f, 1.0f, 0.0f},		{ 1.f,  0.f } },//52
		{ vertices.at(12),		{ 0.0f, 1.0f, 0.0f},		{ 0.f,  0.f } },//53
		{ vertices.at(23),		{ 0.0f, 1.0f, 0.0f},		{ 0.f,  1.f } },//54
		{ vertices.at(0),		{ 0.0f, 1.0f, 0.0f},		{ 1.f, 1.f } },//55

		//UpLeft
		{ vertices.at(1),		{ 0.0f, 1.0f, 0.0f},		{ 1.f,  0.f } },//56
		{ vertices.at(22),		{ 0.0f, 1.0f, 0.0f},		{ 0.f,  0.f } },//57
		{ vertices.at(21),		{ 0.0f, 1.0f, 0.0f},		{ 0.f,  1.f } },//58
		{ vertices.at(2),		{ 0.0f, 1.0f, 0.0f},		{ 1.f, 1.f } },//59


		//Bottom Facing
		//Down Right
		{ vertices.at(15),		{ 0.0f, -1.0f, 0.0f},		{ 1.f,  0.f } },//60
		{ vertices.at(8),		{ 0.0f, -1.0f, 0.0f},		{ 0.f,  0.f } },//61
		{ vertices.at(7),		{ 0.0f, -1.0f, 0.0f},		{ 0.f,  1.f } },//62
		{ vertices.at(16),		{ 0.0f, -1.0f, 0.0f},		{ 1.f, 1.f } },//63

		//DownMid
		{ vertices.at(17),		{ 0.0f, -1.0f, 0.0f},		{ 1.f,  0.f } },//64
		{ vertices.at(6),		{ 0.0f, -1.0f, 0.0f},		{ 0.f,  0.f } },//65
		{ vertices.at(5),		{ 0.0f, -1.0f, 0.0f},		{ 0.f,  1.f } },//66
		{ vertices.at(18),		{ 0.0f, -1.0f, 0.0f},		{ 1.f, 1.f } },//67

		//Down Left
		{ vertices.at(19),		{ 0.0f, -1.0f, 0.0f},		{ 1.f,  0.f } },//68
		{ vertices.at(4),		{ 0.0f, -1.0f, 0.0f},		{ 0.f,  0.f } },//69
		{ vertices.at(3),		{ 0.0f, -1.0f, 0.0f},		{ 0.f,  1.f } },//70
		{ vertices.at(20),		{ 0.0f, -1.0f, 0.0f},		{ 1.f, 1.f } },//71

	};

	const std::vector<uint32_t> plus_indices
	{
		//Front
		0,5,6,11,0,6,
		2,3,8,9,2,8,

		//Back
		12,17,18,23,12,18,
		14,15,20,21,14,20,

		//LeftFacing
		//Top
		24,25,26,24,26,27,
		//Mid
		28,29,30,28,30,31,
		//Bot
		32,33,34,32,34,35,


		//RightFacing
		//Top
		36,37,38,36,38,39,
		//Mid
		40,41,42,40,42,43,
		//Bot
		44,45,46,44,46,47,

		//UpFacing
		//Right
		48,49,50,48,50,51,
		//Mid
		52,53,54,52,54,55,
		//Left
		56,57,58,56,58,59,

		//Down Facing
		//Right
		60,61,62,60,62,63,
		//Mid
		64,65,66,64,66,67,
		//Left
		68,69,70,68,70,71

	};

	m_mesh = engine::mesh::create(plus_vertices, plus_indices);
}

engine::plus::~plus() {}

engine::ref<engine::plus> engine::plus::create(float size)
{
	std::vector<glm::vec3> plus_vertices;
		plus_vertices.push_back(glm::vec3(-0.5f*size, 1.5f*size, 0.0f*size));//0
		plus_vertices.push_back(glm::vec3(-0.5f*size, 0.5f*size, 0.f*size));//1
		plus_vertices.push_back(glm::vec3(-1.5f*size, 0.5f*size, 0.f*size));//2
		plus_vertices.push_back(glm::vec3(-1.5f*size, -0.5f*size, 0.f*size));//3
		plus_vertices.push_back(glm::vec3(-0.5f*size, -0.5f*size, 0.f*size));//4
		plus_vertices.push_back(glm::vec3(-0.5f*size, -1.5f*size, 0.f*size));//5
		plus_vertices.push_back(glm::vec3(0.5f*size, -1.5f*size, 0.f*size));//6
		plus_vertices.push_back(glm::vec3(0.5f*size, -0.5f*size, 0.f*size));//7
		plus_vertices.push_back(glm::vec3(1.5f*size, -0.5f*size, 0.f*size));//8
		plus_vertices.push_back(glm::vec3(1.5f*size, 0.5f*size, 0.f*size));//9
		plus_vertices.push_back(glm::vec3(0.5f*size, 0.5f*size, 0.f*size));//10
		plus_vertices.push_back(glm::vec3(0.5f*size, 1.5f*size, 0.f*size));//11
		
		

		plus_vertices.push_back(glm::vec3(0.5f*size, 1.5f*size, -0.5f*size)); //12
		plus_vertices.push_back(glm::vec3(0.5f*size, 0.5f*size, -0.5f*size)); //13
		plus_vertices.push_back(glm::vec3(1.5f*size, 0.5f*size, -0.5f*size)); //14
		plus_vertices.push_back(glm::vec3(1.5f*size, -0.5f*size, -0.5f*size)); //15
		plus_vertices.push_back(glm::vec3(0.5f*size, -0.5f*size, -0.5f*size)); //16
		plus_vertices.push_back(glm::vec3(0.5f*size, -1.5f*size, -0.5f*size));//17--
		plus_vertices.push_back(glm::vec3(-0.5f*size, -1.5f*size, -0.5f*size));//18--
		plus_vertices.push_back(glm::vec3(-0.5f*size, -0.5f*size, -0.5f*size));//19
		plus_vertices.push_back(glm::vec3(-1.5f*size, -0.5f*size, -0.5f*size));//20
		plus_vertices.push_back(glm::vec3(-1.5f*size, 0.5f*size, -0.5f*size));//21
		plus_vertices.push_back(glm::vec3(-0.5f*size, 0.5f*size, -0.5f*size));//22
		plus_vertices.push_back(glm::vec3(-0.5f*size, 1.5f*size, -0.5f*size));//23
		
	return std::make_shared<engine::plus>(plus_vertices);
}
