#pragma once
#include <engine.h>
#include "player.h"
#include <string>
class mesh;
class pickup{
public:
	pickup();
	~pickup();

	void init(std::string type, glm::vec3 spawnPosition);
	void update(glm::vec3 c, player& m_player, float dt);
	void on_render(const engine::ref<engine::shader>& shader);
	bool active() { return m_is_active; }
	void setActive() { m_is_active = true; }
	std::string getType() { return type; }
	engine::ref<engine::game_object> getObject() const { return m_pickup; }
private:
	bool m_is_active; // Indicates if pickup is active
	std::string type;
	engine::ref<engine::game_object> m_pickup;
};
