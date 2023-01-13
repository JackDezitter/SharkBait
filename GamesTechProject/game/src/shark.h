#pragma once
#include <engine.h>
#include"engine/entities/bounding_box.h"
#include"engine/entities/bounding_sphere.h"
#include <GLFW/include/GLFW/glfw3.h>
#include "player.h"
class shark
{
	enum class state
	{
		passive,
		attack,
		strike,
		dying,
		dead,
		flee
	};

public:
	shark();
	~shark();

	void setAnimation(std::string animation);

	glm::vec3 get_position();
	void transition(shark::state newstate);
	void back(const engine::timestep& time_step);
	engine::game_object_properties build();
	void initialise(engine::game_object_properties shark_props,glm::vec3 spawnPosition, int wave);
	void on_update(const engine::timestep& time_step, const engine::perspective_camera& camera, player& player1);
	void on_render(const engine::ref<engine::shader>& shader, const engine::perspective_camera& camera);
	engine::ref<engine::game_object> object() const { return m_shark; }
	int getWave() { return m_wave; }
	void setWave(int wave) { m_wave = wave; }

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
		if (m_mode == 1) {
			this->transition(state::attack);
		}
		if (m_mode == 2) {
			this->transition(state::strike);
		}
		if (m_mode == 3) {
			this->transition(state::flee);
		}
	}
	int getDamage() { return m_damage; }

	int getAttackCooldown() { return attackCooldown; }
	void resetAttackCooldown() { attackCooldown = glfwGetTime() + 15; }

	glm::vec3 getTarget() { return m_target; }
	void setTarget(glm::vec3 target) { m_target = target; }

	float getChangeTargetTime() { return changeTargetTime; }
	void setChangeTargetTime(float time) { changeTargetTime = time; }
	engine::heightmap getTerrain() { return *terrain; }

	engine::ref<engine::skinned_mesh> getMesh() { return shark_mesh; }
	void setMesh(engine::ref<engine::skinned_mesh> mesh) {
		this->shark_mesh = mesh;
	}

	void set_box(float width, float height, float depth, glm::vec3 position) {
		m_shark_box.set_box(width, height, depth, position);
	}
	engine::bounding_box& getBox() { return m_shark_box; }
	engine::bounding_sphere& getSphere() { return m_shark_sphere; }
	std::vector<engine::bounding_sphere>& getSphereSet() { return m_shark_sphere_set; }
	void setSphereSet(std::vector<engine::bounding_sphere> sphere_set) { m_shark_sphere_set = sphere_set; }

	shark::state getState() { return this->m_state; }


private:
	engine::ref<engine::game_object> m_shark;
	engine::bounding_box m_shark_box;
	engine::bounding_sphere m_shark_sphere;
	std::vector<engine::bounding_sphere> m_shark_sphere_set{};
	int m_wave =1;
	float m_speed = 1.0f + ((2*(this->getWave() -1))/10);
	float m_health = 100.0f + (20*(this->getWave() - 1));
	float m_damage = 10 + (2 * (this->getWave()-1));
	int m_mode = 0;
	int attackCooldown = glfwGetTime()+5;
	float deathTimer;
	float biteTimer;
	glm::vec3 m_target = glm::vec3(0,0,0);
	float changeTargetTime = 0;
	engine::ref<engine::heightmap> terrain = engine::heightmap::create("assets/textures/RealHeightmap4.bmp", "assets/textures/SandyBottom.bmp", 50.f, 50.f, glm::vec3(0.f, 0.5f, 0.f), 40.f);
	engine::ref<engine::skinned_mesh> shark_mesh;
	engine::game_object_properties shark_props;

	//current state of the enemy's state machine
	state m_state = state::passive;

};
