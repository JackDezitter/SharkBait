#include "ballistic.h"

ballistic::ballistic() {}

ballistic::~ballistic() {}

//Builds ballistic shape from mesh
engine::game_object_properties ballistic::build() {
	//Mesh 
	proj_model = engine::model::create("assets/models/static/HarpoonBullet2.obj");
	engine::game_object_properties proj_props;
	engine::ref<engine::texture_2d> proj_texture = engine::texture_2d::create("assets/textures/harpoon/Harpoon_Low_Harpoon_BaseColor.png", true);
	proj_props.meshes = proj_model->meshes();
	proj_props.textures = { proj_texture };
	proj_props.rotation_amount = 0.f;
	proj_props.rotation_axis = glm::vec3(1, 1, 1);
	float proj_scale = 1.2f ;
	proj_props.scale = glm::vec3(proj_scale);
	proj_props.type = 0;
	proj_props.bounding_shape = proj_model->size() / 4.f;
	proj_props.mass = 0.0f;
	return proj_props;
}



//Initializes ballistic object with given mesh and sets vcollision box
void ballistic::initialise(engine::game_object_properties proj_props) {
	//Projectile
	m_object = engine::game_object::create(proj_props);
	m_object->set_offset(proj_model->offset());
	float proj_scale = 1.2f;
	m_ballistic_box.set_box((proj_props.bounding_shape.x * 4.f) * proj_scale, (proj_props.bounding_shape.x * 4.f) * proj_scale, (proj_props.bounding_shape.x * 4.f) *
		proj_scale, proj_props.position - glm::vec3(0.f, m_object->offset().y, 0.f));
}

//Emitts projectile from gun's position
void ballistic::fire(const engine::perspective_camera& camera, float speed, glm::vec3 position)
{
	glm::vec3 direction = normalize(camera.front_vector());
	m_object->turn_towards(-direction);

	m_object->set_position(camera.front_vector()+position);
	m_ballistic_box.on_update(object()->position() - glm::vec3(0.f, m_object->offset().y, 0.f));
	
	m_object->set_forward(direction);
	m_speed = speed;
}

//Calculates the new position of the projectile accounting its speed and timestep
void ballistic::on_update(const engine::timestep& time_step)
{
	m_object->set_position(m_object->position() + m_object->forward() * (float)time_step * m_speed);

	glm::vec3 forward = m_object->forward();
	float theta = engine::PI / 2.0f - acos(forward.y);
	float phi = engine::PI - atan2(forward.x, forward.z);

	m_ballistic_box.on_update(m_object->position() - glm::vec3(0,m_object->offset().y*1.2f,0));

}
//Renders projectile
void ballistic::on_render(const engine::ref<engine::shader>& shader)
{
	engine::renderer::submit(shader, m_object);
	//m_ballistic_box.on_render(2.5f, 0.f, 0.f, shader);
}
//Retireves current position of projectile
glm::vec3 ballistic::get_position() {
	return(m_object->position());
}
