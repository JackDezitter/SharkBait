#include "pch.h"
#include "bounding_sphere.h"
#include "platform/opengl/gl_shader.h"

engine::bounding_sphere::bounding_sphere()
{
	m_radius = 0;
}

engine::bounding_sphere::~bounding_sphere()
{}

void engine::bounding_sphere::get(glm::vec3& centre, float& radius)
{
	centre = m_centre;
	radius = m_radius;
}

void engine::bounding_sphere::set_sphere(float radius, glm::vec3 position)
{
	m_radius = radius;
	m_centre = position;
	engine::ref<engine::sphere> pickup_shape = engine::sphere::create(15, 15, radius);
	m_mesh = pickup_shape->mesh();
}

void engine::bounding_sphere::on_update(glm::vec3 position)
{
	m_centre = position;
}

void engine::bounding_sphere::on_render(float r, float g, float b, const engine::ref<engine::shader>& shader)
{
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, m_centre);

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("u_transform", transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_colour", glm::vec3(r, g, b));
	m_mesh->va()->bind();
	engine::renderer_api::draw_indexed_lines(m_mesh->va());
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", false);
}



bool engine::bounding_sphere::collision(bounding_sphere other_sphere)
{

	float radius1, radius2;
	glm::vec3 centre1, centre2;

	get(centre1, radius1); // this sphere
	other_sphere.get(centre2, radius2); // other sphere

	glm::vec3 v = centre2 - centre1;
	if (fabs(v.x) * 2 <= (radius1 + radius2) &&
		fabs(v.y) * 2 <= (radius1 + radius2) &&
		fabs(v.z) * 2 <= (radius1 + radius2))
		return true; // Collision detected
	else
		return false; // No collision detected
}

bool engine::bounding_sphere::collisionBox(engine::bounding_box other_box)
{

	float radius1, width2, height2, depth2;
	glm::vec3 centre1, bottom2, centre2;

	get(centre1, radius1); // this sphere
	other_box.get(bottom2, width2, height2, depth2); // other box
	
	centre2 = bottom2 + glm::vec3(width2 / 2.f, height2 / 2.0f, depth2 / 2.f);

	glm::vec3 v = centre2 - centre1;
	if (fabs(v.x) * 2 <= (radius1 + width2) &&
		fabs(v.y) * 2 <= (radius1 + height2) &&
		fabs(v.z) * 2 <= (radius1 + depth2))
		return true; // Collision detected
	else
		return false; // No collision detected
}

bool engine::bounding_sphere::collisionPoint(glm::vec3 point) {
	float radius1;
	glm::vec3 centre1;
	get(centre1,radius1);
	glm::vec3 v = point - centre1;
	if (fabs(v.x) * 2 <= (radius1) &&
		fabs(v.y) * 2 <= (radius1) &&
		fabs(v.z) * 2 <= (radius1))
		return true; // Collision detected
	else
		return false; // No collision detected
}
