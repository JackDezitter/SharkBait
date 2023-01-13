#include "bubble_emitter.h"
#include "pickup.h"
#include <string>

bubble_emitter::bubble_emitter() {}
bubble_emitter::~bubble_emitter() {}

//Loads mesh and initial values for bubble emitter
engine::game_object_properties bubble_emitter::build() {

	engine::game_object_properties emitter_props;
	engine::ref <engine::model> emitter_model = engine::model::create("assets/models/static/Coral15G3.obj");
	engine::ref<engine::texture_2d> emitter_texture = engine::texture_2d::create("assets/models/static/Corals_13_14_15_16_diff.png", true);
	emitter_props.meshes = emitter_model->meshes();
	emitter_props.textures = { emitter_texture };
	float emitter_scale = 1.f;
	emitter_props.scale = glm::vec3(emitter_scale);
	return emitter_props;
}


//Initializes emitter object, sets attributes, and creates the object
void bubble_emitter::init(engine::game_object_properties emitter_props, glm::vec3 position)
{
	emitter_props.position = { position };
	m_emitter = engine::game_object::create(emitter_props);
	LOG_CORE_TRACE("{}", m_emitter->position());

}
//Causes the ewmitter to release bubbles periodically
pickup bubble_emitter::update()
{
	pickup m_pickup;
	m_pickup.init("oxygen", m_emitter->position()+glm::vec3(0.f,0.1f,0.f));
	this->emitterTimer = glfwGetTime() + 5;
	return m_pickup;
	
}
//Renders the emitter object
void bubble_emitter::on_render(const engine::ref<engine::shader>& shader)
{
	m_emitter->textures().at(0)->bind();
	glm::mat4 emitter_transform(1.0f);
	emitter_transform = glm::translate(emitter_transform, m_emitter->position());
	engine::renderer::submit(shader, m_emitter->meshes().at(0), emitter_transform);

}
