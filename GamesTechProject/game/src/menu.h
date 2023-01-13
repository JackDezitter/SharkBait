#pragma once
#include <engine.h>

class quad;

class menu
{
public:
	menu(const std::string& path, float width, float height);
	~menu();

	void on_render(engine::ref<engine::shader> shader);
	static engine::ref<menu> create(const std::string& path, float width, float height);

private:
	engine::ref<engine::texture_2d> m_texture;
	engine::ref<quad> m_quad;
};
