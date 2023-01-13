#include "pch.h"
#include "menu.h"
#include "quad.h"

//Creates Menu Quad
menu::menu(const std::string& path, float width, float height)
{
	m_texture = engine::texture_2d::create(path, true);

	m_quad = quad::create(glm::vec2(width, height));
}

menu::~menu()
{}

void menu::on_render(engine::ref<engine::shader> shader)
{
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.f, 0.f, 0.1f));

	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", true);
	m_texture->bind();
	engine::renderer::submit(shader, m_quad->mesh(), transform);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("has_texture", false);
	std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("transparency", 1.0f);
}


engine::ref<menu> menu::create(const std::string& path, float width, float height)
{
	return std::make_shared<menu>(path, width, height);
}
