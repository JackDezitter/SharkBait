#pragma once
#include <engine.h>
#include "pickup.h"
#include <string>
class mesh;
class bubble_emitter {
public:
	bubble_emitter();
	~bubble_emitter();

	engine::game_object_properties bubble_emitter::build();
	void init(engine::game_object_properties emitter_props, glm::vec3 position);
	pickup update();
	void on_render(const engine::ref<engine::shader>& shader);
	int getEmitterTimer() { return emitterTimer; }
	engine::ref<engine::game_object> getObject() const { return m_emitter; }
	void setPosition(glm::vec3 position) { this->m_emitter->set_position(position); }
private:
	int emitterTimer; // Indicates if emitter can produce bubble
	engine::ref<engine::game_object> m_emitter;
};
#pragma once
