#include "pickup.h"
#include "engine/entities/shapes/plus.h"
#include <string>

pickup::pickup() {}
pickup::~pickup() {}

//Initializes pick-up object, sets attributes, and creates the object
void pickup::init(std::string type, glm::vec3 spawnPosition)
{
	this->m_is_active = true;
	this->type = type;

	engine::game_object_properties pickup_props;
	pickup_props.position = { spawnPosition };
	if (this->type != "oxygen") {
		// Healthbox texture created myself
		engine::ref<engine::texture_2d> pickup_texture;

		if (type == "health") {
			engine::ref<engine::plus> pickup_shape = engine::plus::create(0.1);
			pickup_texture = engine::texture_2d::create("assets/textures/RedBackground.jpg", true);
			pickup_props.textures = { pickup_texture };
			pickup_props.meshes = { pickup_shape->mesh() };
		}
		else {
			engine::ref<engine::cuboid> pickup_shape = engine::cuboid::create(glm::vec3(0.1f), false);
			pickup_texture = engine::texture_2d::create("assets/textures/Wood_Crate_001_basecolor.jpg", true);
			pickup_props.textures = { pickup_texture };
			pickup_props.meshes = { pickup_shape->mesh() };
		}
	}
	else {
		engine::ref<engine::sphere> pickup_shape = engine::sphere::create(20,20,0.1f);
		pickup_props.meshes = { pickup_shape->mesh() };
	}
	
	this->m_pickup = engine::game_object::create(pickup_props);
}
//Rotates the object and if the camera is within a set location and the pick-up is of type health it adds 25hp to the player, then setting its active status to false
void pickup::update(glm::vec3 c, player& m_player, float dt)
{
	glm::vec3 d = m_pickup->position() - c;
	if (type != "oxygen") {
		m_pickup->set_rotation_amount(m_pickup->rotation_amount() + dt * 1.f);
		if (m_is_active == true) {
			if (glm::length(d) <=  (1.5 * 0.1) && type == "health") {
				m_player.setHealth(m_player.getHealth() + 25);
				m_is_active = false;
			}
			else if (glm::length(d) <= 0.1 && type == "ammo") {
				m_player.setAmmoCount(m_player.getAmmoCount() + 30);
				m_is_active = false;
			}
		}
	}
	else {
		if (glm::length(d) < 0.15f && m_is_active == true) {
			m_player.setOxygen(m_player.getOxygen() + 10);
			m_player.setOxygenCounter();
			m_is_active = false;
		}
		else if(m_pickup->position().y > 20) {
			m_is_active = false;
		}
		m_pickup->set_position(glm::vec3(m_pickup->position().x, (m_pickup->position().y + 0.4 * dt), m_pickup->position().z));
	}
		
}
//Renders the pickup object
void pickup::on_render(const engine::ref<engine::shader>& shader)
{
	if (this->type != "oxygen"){
		m_pickup->textures().at(0)->bind();
	}
	glm::mat4 pickup_transform(1.0f);
	pickup_transform = glm::translate(pickup_transform, m_pickup->position());
	pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), m_pickup->rotation_axis());
	engine::renderer::submit(shader, m_pickup->meshes().at(0), pickup_transform);

}



