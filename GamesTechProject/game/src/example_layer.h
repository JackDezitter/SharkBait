#pragma once
#include <engine.h>
#include "ballistic.h"
#include "shark.h"
#include "dolphin.h"
#include "turtle.h"
#include "jellyfish.h"
#include "player.h"
#include "pickup.h"
#include <engine/entities/shapes/hexcone.h>
#include <engine/entities/shapes/prism.h>
#include "bubble_emitter.h"
#include <engine/entities/shapes/heightmap.h>
#include "cross_fade.h"
#include "menu.h"

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:

	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::skybox>			m_waterbox{};

	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::heightmap>		m_heightmap;

	engine::ref<engine::game_object>	m_surface;

	engine::ref<engine::game_object>	m_coral;

	bool m_fog = false;
	glm::vec3 m_fog_colour;
	int32_t m_fog_factor_type;

	engine::game_object_properties		build_shark;
	std::vector<engine::game_object_properties>		build_shark_list;
	shark								m_shark{};
	std::vector<shark>					shark_list;
	player								m_player{};
	engine::ref<engine::game_object>	m_gun{};
	int									current_wave = 0;

	engine::game_object_properties		build_dolphin;
	dolphin								m_dolphin{};
	dolphin								m_dolphin2{};
	dolphin								m_dolphin3{};
	dolphin								m_dolphin4{};
	dolphin								m_dolphin5{};
	dolphin								m_dolphin6{};
	dolphin								m_dolphin7{};
	std::vector<dolphin>				dolphin_list;

	engine::game_object_properties		build_turtle;
	turtle								m_turtle{};
	std::vector<turtle>					turtle_list;

	engine::ref<engine::material>		m_jelly_material{};
	engine::game_object_properties		build_jellyfish;
	jellyfish							m_jellyfish{};
	std::vector<jellyfish>				jellyfish_list;

	engine::ref<engine::game_object>	m_boat;

	engine::ref<engine::material>		m_material{};
	engine::ref<engine::material>		m_surface_material{};
	engine::ref<engine::material>		m_bubble_material{};
	engine::ref<engine::material>		m_shark_material{};
	engine::ref<engine::material>		m_ballistic_material{};

	engine::DirectionalLight            m_directionalLight;
	engine::PointLight					m_pointLight;
	std::vector<engine::PointLight>		pointLight_list;
	uint32_t							num_point_lights = 1;

	engine::SpotLight					m_spotLight;
	engine::SpotLight					m_torch;
	uint32_t							num_spot_lights = 2;
	engine::ref<engine::game_object>	m_lamp;
	engine::ref<engine::game_object>	m_prism;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};
	pickup								m_pickup;
	std::vector<pickup>					pickup_list;

	bubble_emitter						m_bubble_emitter;
	std::vector<bubble_emitter>			emiiter_list;

	engine::ref<engine::bullet_manager> m_physics_manager{};
	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera;

	engine::game_object_properties	build_proj{};
	ballistic						m_ballistic;
	std::vector<ballistic>			ballistic_list;

	engine::ref<cross_fade>			m_cross_fade{};
	engine::ref<menu>				m_menu{};
	bool							menuOn;
	engine::ref<menu>				death_menu{};
	bool							deathMenuOn;

	float m_freq_high;
	float m_freq_low;
};
