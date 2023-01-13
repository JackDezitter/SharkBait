#pragma once
#include <engine.h>
#include "bounding_box.h"

namespace engine
{
	class bounding_sphere
	{
	public:
		bounding_sphere();
		~bounding_sphere();
		void get(glm::vec3& centre, float& radius);
		void set_sphere(float radius, glm::vec3 bottom);
		void on_update(glm::vec3 position);
		void on_render(float r, float g, float b, const engine::ref<engine::shader>& shader);
		bool collision(bounding_sphere other_sphere);

		bool collisionBox(engine::bounding_box other_box);

		bool collisionPoint(glm::vec3 point);

	private:
		glm::vec3 m_centre;
		float m_radius;
		engine::ref<engine::mesh> m_mesh;

		float m_rotation_amount;
	};
}
#pragma once
