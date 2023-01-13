#pragma once
#include <engine.h>
#include"engine/entities/bounding_box.h"
#include"engine/entities/bounding_sphere.h"
#include <GLFW/include/GLFW/glfw3.h>
#include "player.h"
class turtle
{
	enum class state
	{
		passive,
		flee,
		dead
	};

public:
	turtle();
	~turtle();
	glm::vec3 get_position();
	void transition(turtle::state newstate);
	void back(const engine::timestep& time_step);
	engine::game_object_properties build();
	void initialise(engine::game_object_properties turtle_props, glm::vec3 spawnPosition);
	void on_update(const engine::timestep& time_step, const engine::perspective_camera& camera, player& player1);
	void on_render(const engine::ref<engine::shader>& shader, const engine::perspective_camera& camera);
	engine::ref<engine::game_object> object() const { return m_turtle; }

	int getHealth() { return m_health; }
	void setHealth(int health) { m_health = health; }

	float getSpeed() { return m_speed; }
	void setSpeed(float speed) { m_speed = speed; }

	int getMode() { return m_mode; }
	void setMode(int mode) {
		this->m_mode = mode;
		if (m_mode == 0) {
			this->transition(state::passive);
		}
		if (m_mode == 3) {
			this->transition(state::flee);
		}
		if (m_mode == -2) {
			this->transition(state::dead);
		}
	}

	glm::vec3 getTarget() { return m_target; }
	void setTarget(glm::vec3 target) { m_target = target; }

	float getChangeTargetTime() { return changeTargetTime; }
	void setChangeTargetTime(float time) { changeTargetTime = time; }
	engine::heightmap getTerrain() { return *terrain; }
	engine::bounding_sphere& getSphere() { return m_turtle_sphere; }
	std::vector<engine::bounding_sphere>& getSphereSet() { return m_turtle_sphere_set; }
	void setSphereSet(std::vector<engine::bounding_sphere> sphere_set) { m_turtle_sphere_set = sphere_set; }

	turtle::state getState() { return this->m_state; }

private:
	engine::ref<engine::game_object> m_turtle;
	engine::bounding_sphere m_turtle_sphere;
	std::vector<engine::bounding_sphere> m_turtle_sphere_set{};
	float m_speed = 1.5f;
	float m_health = 100.0f;
	int m_mode = 0;
	glm::vec3 m_target = glm::vec3(0, 0, 0);
	float changeTargetTime = 0;
	engine::ref<engine::heightmap> terrain = engine::heightmap::create("assets/textures/RealHeightmap4.bmp", "assets/textures/SandyBottom.bmp", 50.f, 50.f, glm::vec3(0.f, 0.5f, 0.f), 40.f);
	engine::game_object_properties turtle_props;

	//current state of the entities's state machine
	state m_state = state::passive;

};
