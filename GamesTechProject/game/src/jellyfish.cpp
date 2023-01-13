#include "jellyfish.h"
#include <string>
#include <GLFW/include/GLFW/glfw3.h>

jellyfish::jellyfish() {}

jellyfish::~jellyfish() {}

//Initiallizes jellyfish object
engine::game_object_properties jellyfish::build() {

	engine::ref <engine::model> jellyfish_model = engine::model::create("assets/models/static/JellyFish.obj");
	engine::game_object_properties jellyfish_props;
	jellyfish_props.meshes = jellyfish_model->meshes();
	jellyfish_props.rotation_amount = 1.f;
	jellyfish_props.rotation_axis = glm::vec3(0, 1.f, 0);
	float jellyfish_scale = 0.2f;
	jellyfish_props.scale = glm::vec3(jellyfish_scale);
	jellyfish_props.bounding_shape = jellyfish_model->size() / 2.f * jellyfish_scale;

	return jellyfish_props;
}

void jellyfish::initialise(engine::game_object_properties jellyfish_props, glm::vec3 spawnPosition) {
	this->jellyfish_props = jellyfish_props;
	this->jellyfish_props.position = spawnPosition;
	this->m_jellyfish = engine::game_object::create(this->jellyfish_props);

	this->setSpeed(0.5f);
	this->m_jellyfish->set_position(spawnPosition);
	this->m_jellyfish->set_forward(glm::vec3(0.f, 0.f, -1.f));

	m_jellyfish_box.set_box(jellyfish_props.bounding_shape.x / (jellyfish_props.scale.z * 2), jellyfish_props.bounding_shape.y / (jellyfish_props.scale.z * 2),
		jellyfish_props.bounding_shape.z / (jellyfish_props.scale.z * 2), m_jellyfish->position() - (jellyfish_props.bounding_shape.y / (jellyfish_props.scale.z * 2))/2);

	glm::vec3 pos = m_jellyfish->position();
	glm::vec3 forward = m_jellyfish->forward();
	glm::vec3 up = m_jellyfish->up();

	this->m_jellyfish->set_angular_factor_lock(true);

}

//Updates jellyfish object animation and position
void jellyfish::on_update(const engine::timestep& time_step, const engine::perspective_camera& camera, player& player1)
{
	double currentTime = glfwGetTime();
	glm::vec3 m_position = m_jellyfish->position();
	m_position.x = glm::clamp(m_position.x, -19.0f, 19.0f);
	m_position.z = glm::clamp(m_position.z, -19.0f, 19.0f);
	glm::vec3 d = glm::vec3(m_position - (this->getTarget()));
	glm::vec3 nd = glm::vec3(m_jellyfish->position() += m_jellyfish->forward() * m_speed * (float)time_step - (this->getTarget()));

	if (m_state == state::passive) {
		if (((this->getChangeTargetTime() == 0.f) || (this->getChangeTargetTime() < currentTime) || length(d) == 0 || d.x * nd.x < 0 || d.y * nd.y < 0 || d.z * nd.z < 0)) {
			float targetx = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float targetz = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float minY = this->getTerrain().ground_height(glm::vec3(targetx, 0, targetz) + 0.5f);
			float targety = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - minY)));
			this->setTarget(glm::vec3(targetx, targety, targetz));
			this->setChangeTargetTime(currentTime + (rand() % 10 + 5));
			this->m_jellyfish->turn_towards(this->getTarget());
		}
	}
	m_jellyfish->set_position(m_jellyfish->position() += m_jellyfish->forward() * (float)this->getSpeed() * (float)time_step);
	m_jellyfish_box.on_update(m_jellyfish->position() - glm::vec3(0.f, (jellyfish_props.bounding_shape.z / (jellyfish_props.scale.z * 2) / 2), 0.f));

}
//Renders jellyfish
void jellyfish::on_render(const engine::ref<engine::shader>& shader, const engine::perspective_camera& camera)
{
	//Transforms jellyfish to rotate towards the camera
	glm::mat4 jellyfish_transform(1.0f);
	glm::vec3 jellyfishp = this->m_jellyfish->position();
	glm::vec3 jellyfishc = this->getTarget();

	glm::vec3 jellyfishv = jellyfishc - jellyfishp;
	float pi = 3.14159265358979323846;
	//rotation angle of jellyfish in horizontal plane
	float jellyfishtheta = atan2(jellyfishv.x, jellyfishv.z) + pi / 2;
	//Transforms the jellyfish
	jellyfish_transform = glm::translate(jellyfish_transform, this->m_jellyfish->position());
	jellyfish_transform = glm::rotate(jellyfish_transform, jellyfishtheta, glm::vec3(0.f, 1.f, 0.f));
	jellyfish_transform = glm::scale(jellyfish_transform, this->m_jellyfish->scale());
	engine::renderer::submit(shader, jellyfish_transform, this->m_jellyfish);
	//Sets the jellyfish forward vector to the vector towards the camera
	this->m_jellyfish->set_forward(glm::normalize(jellyfishv));
}


glm::vec3 jellyfish::get_position() {
	return(m_jellyfish->position());
}

