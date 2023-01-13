#pragma once
#include <engine.h>
#include "engine/entities/bounding_box.h" 
class ballistic
{
public:
	ballistic();
	~ballistic();

	engine::game_object_properties build();

	void initialise();
	void on_render(const engine::ref<engine::shader>& shader);
	glm::vec3 get_position();
	void initialise(engine::game_object_properties proj_props);
	void fire(const engine::perspective_camera& camera, float speed, glm::vec3 position);
	void on_update(const engine::timestep& time_step);
	engine::ref<engine::game_object> object() const { return m_object; }

	void set_box(float width, float height, float depth, glm::vec3 position) {}
	engine::bounding_box& getBox() { return m_ballistic_box; };
private:

	engine::ref<engine::game_object> m_object;
	float m_speed = 0.0f;
	engine::bounding_box m_ballistic_box;
	engine::ref <engine::model> proj_model;
};
