#pragma once
#include <engine.h>
#include"engine/entities/bounding_box.h"
#include"engine/entities/bounding_sphere.h"
#include <GLFW/include/GLFW/glfw3.h>
#include "player.h"
class dolphin
{
	enum class state
	{
		roam,
		pod,
		flee
	};

public:
	dolphin();
	~dolphin();

	glm::vec3 get_position();
	void transition(dolphin::state newstate);
	void back(glm::vec3 pos);
	engine::game_object_properties build();
	void initialise(engine::game_object_properties dolphin_props, glm::vec3 spawnPosition);
	void on_update(const engine::timestep& time_step, const engine::perspective_camera& camera, player& player1);
	void on_render(const engine::ref<engine::shader>& shader, const engine::perspective_camera& camera);
	engine::ref<engine::game_object> object() const { return m_dolphin; }

	float getSpeed() { return m_speed; }
	void setSpeed(float speed) { m_speed = speed; }

	int getMode() { return m_mode; }
	void setMode(int mode) {
		this->m_mode = mode;
		if (m_mode == 0) {
			this->transition(state::roam);
		}
		if (m_mode == 1) {
			this->transition(state::pod);
		}
		if (m_mode == 3) {
			this->transition(state::flee);
		}
	}

	int getFleeTimer() { return fleeTimer; }
	void setFleeTimer(int time) { this->fleeTimer = time; }

	glm::vec3 getTarget() { return m_target; }
	void setTarget(glm::vec3 target) { m_target = target; }

	bool getChangeDirection() { return changeDirection; }
	void setChangeDirection(bool flag) { changeDirection = flag; }

	int getPod() { return pod; }
	void setPod(int newPod) { pod = newPod; }
	float getChangeTargetTime() { return changeTargetTime; }
	void setChangeTargetTime(float time) { changeTargetTime = time; }
	engine::heightmap getTerrain() { return *terrain; }

	engine::ref<engine::skinned_mesh> getMesh() { return dolphin_mesh; }
	void setMesh(engine::ref<engine::skinned_mesh> mesh) {
		this->dolphin_mesh = mesh;
	}

	void set_box(float width, float height, float depth, glm::vec3 position) {
		m_dolphin_box.set_box(width, height, depth, position);
	}
	engine::bounding_box& getBox() { return m_dolphin_box; }
	engine::bounding_sphere& getSphere() { return m_dolphin_sphere; }
	std::vector<engine::bounding_sphere>& getSphereSet() { return m_dolphin_sphere_set; }
	void setSphereSet(std::vector<engine::bounding_sphere> sphere_set) { m_dolphin_sphere_set = sphere_set; }

	dolphin::state getState() { return this->m_state; }


private:
	engine::ref<engine::game_object> m_dolphin;
	engine::bounding_box m_dolphin_box;
	engine::bounding_sphere m_dolphin_sphere;
	std::vector<engine::bounding_sphere> m_dolphin_sphere_set{};
	float m_speed = 2.0f;
	int m_mode = 0;
	int fleeTimer = 0;
	bool changeDirection;
	int pod = 0;
	glm::vec3 m_target = glm::vec3(0, 0, 0);
	float changeTargetTime = 0;
	engine::ref<engine::heightmap> terrain = engine::heightmap::create("assets/textures/RealHeightmap4.bmp", "assets/textures/SandyBottom.bmp", 50.f, 50.f, glm::vec3(0.f, 0.5f, 0.f), 40.f);
	engine::ref<engine::skinned_mesh> dolphin_mesh;
	engine::game_object_properties dolphin_props;

	//current state of the enemy's state machine
	state m_state = state::roam;

};
