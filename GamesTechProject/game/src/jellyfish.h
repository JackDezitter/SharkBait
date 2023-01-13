#pragma once
#include <engine.h>
#include"engine/entities/bounding_box.h"
#include"engine/entities/bounding_sphere.h"
#include <GLFW/include/GLFW/glfw3.h>
#include "player.h"
class jellyfish
{
	enum class state
	{
		passive,
	};

public:
	jellyfish();
	~jellyfish();
	glm::vec3 get_position();
	engine::game_object_properties build();
	void initialise(engine::game_object_properties jellyfish_props, glm::vec3 spawnPosition);
	void on_update(const engine::timestep& time_step, const engine::perspective_camera& camera, player& player1);
	void on_render(const engine::ref<engine::shader>& shader, const engine::perspective_camera& camera);
	engine::ref<engine::game_object> object() const { return m_jellyfish; }

	float getSpeed() { return m_speed; }
	void setSpeed(float speed) { m_speed = speed; }

	glm::vec3 getTarget() { return m_target; }
	void setTarget(glm::vec3 target) { m_target = target; }

	float getChangeTargetTime() { return changeTargetTime; }
	void setChangeTargetTime(float time) { changeTargetTime = time; }
	engine::heightmap getTerrain() { return *terrain; }

	void set_box(float width, float height, float depth, glm::vec3 position) {
		m_jellyfish_box.set_box(width, height, depth, position);
	}

	engine::bounding_box& getBox() { return m_jellyfish_box; }

	float getStingTimer() { return stingTimer; }
	void setStingTimer(float newStingTimer) {
		this->stingTimer = newStingTimer;
	}
	jellyfish::state getState() { return this->m_state; }


private:
	engine::ref<engine::game_object> m_jellyfish;
	engine::bounding_box m_jellyfish_box;
	float m_speed = 1.5f;
	int m_mode = 0;
	float stingTimer;
	glm::vec3 m_target = glm::vec3(0, 0, 0);
	float changeTargetTime = 0;
	engine::ref<engine::heightmap> terrain = engine::heightmap::create("assets/textures/RealHeightmap4.bmp", "assets/textures/SandyBottom.bmp", 50.f, 50.f, glm::vec3(0.f, 0.5f, 0.f), 40.f);
	engine::game_object_properties jellyfish_props;

	//current state of the enemy's state machine
	state m_state = state::passive;

};
