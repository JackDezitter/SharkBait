#include "dolphin.h"
#include <string>
#include <GLFW/include/GLFW/glfw3.h>

dolphin::dolphin() {}

dolphin::~dolphin() {}

//Initiallizes dolphin object
engine::game_object_properties dolphin::build() {
	//Creates animated skinned mesh
	engine::ref<engine::skinned_mesh> dolphin_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/dolphin/DolphinMesh.dae");
	//Loads annimation file for idle swimming
	dolphin_skinned_mesh->LoadAnimationFile("assets/models/animated/dolphin/Dolphin_Swim.dae");
	dolphin_skinned_mesh->switch_root_movement(true);
	this->dolphin_mesh = dolphin_skinned_mesh;
	//Sets dolphin object attributes and creates it
	engine::game_object_properties dolphin_props;
	engine::ref<engine::texture_2d> dolphin_texture = engine::texture_2d::create("assets/models/animated/dolphin/Dolphin_for_Textures_Dolphin_Retopo_Low_BaseColor.png.001.png", true);
	dolphin_props.animated_mesh = dolphin_skinned_mesh;
	dolphin_props.scale = glm::vec3(25.f);
	dolphin_props.textures = { dolphin_texture };
	dolphin_props.type = 0;
	dolphin_props.mass = 0.5f;
	dolphin_props.bounding_shape = glm::vec3(this->getMesh()->size().x, this->getMesh()->size().x, this->getMesh()->size().x) / 2.f;

	return dolphin_props;
}

void dolphin::initialise(engine::game_object_properties dolphin_props, glm::vec3 spawnPosition) {
	this->dolphin_props = dolphin_props;
	this->dolphin_props.position = spawnPosition;
	this->m_dolphin = engine::game_object::create(this->dolphin_props);

	this->m_dolphin->set_position(spawnPosition);
	this->m_dolphin->set_forward(glm::vec3(0.f, 0.f, -1.f));
	this->m_dolphin->animated_mesh()->switch_animation(0);

	glm::vec3 pos = this->m_dolphin->position();
	glm::vec3 forward = m_dolphin->forward();
	glm::vec3 up = m_dolphin->up();
	glm::vec3 p;

	this->m_dolphin_sphere.set_sphere(this->dolphin_props.bounding_shape.z + 0.1, pos);
	this->m_dolphin_sphere_set.push_back(m_dolphin_sphere);
	this->m_dolphin_sphere.set_sphere(this->dolphin_props.bounding_shape.z + 1.f, pos);
	this->m_dolphin_sphere_set.push_back(m_dolphin_sphere);
	this->m_dolphin->set_angular_factor_lock(true);

}

//Updates dolphin object animation and position based on state
void dolphin::on_update(const engine::timestep& time_step, const engine::perspective_camera& camera, player& player1)
{
	double currentTime = glfwGetTime();
	glm::vec3 m_position = m_dolphin->position();
	m_position.x = glm::clamp(m_position.x, -19.0f, 19.0f);
	m_position.z = glm::clamp(m_position.z, -19.0f, 19.0f);
	glm::vec3 d = glm::vec3(m_position - (this->getTarget()));
	glm::vec3 nd = glm::vec3(m_dolphin->position() += m_dolphin->forward() * m_speed * (float)time_step - (this->getTarget()));

	if (m_state == state::roam) {
		if (((this->getChangeTargetTime() == 0.f) || (this->getChangeTargetTime() < currentTime) || length(d) == 0 || d.x * nd.x < 0 || d.y * nd.y < 0 || d.z * nd.z < 0)) {
			float targetx = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float targetz = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float minY = this->getTerrain().ground_height(glm::vec3(targetx, 0, targetz) + 0.5f);
			float targety = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - minY)));
			this->setTarget(glm::vec3(targetx, targety, targetz));
			this->setChangeTargetTime(currentTime + (rand() % 10 + 5));
			this->m_dolphin->turn_towards(this->getTarget());
		}
	}
	else if (m_state == state::pod) {
		if (((this->getChangeTargetTime() == 0.f) || (this->getChangeTargetTime() < currentTime) || length(d) == 0 || d.x * nd.x < 0 || d.y * nd.y < 0 || d.z * nd.z < 0)) {
			float targetx = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float targetz = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float minY = this->getTerrain().ground_height(glm::vec3(targetx, 0, targetz) + 0.5f);
			float targety = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - minY)));
			this->setTarget(glm::vec3(targetx, targety, targetz));
			this->setChangeTargetTime(currentTime + (rand() % 10 + 5));
			this->m_dolphin->turn_towards(this->getTarget());
		}
	}

	else if (m_state == state::flee) {
		if (glfwGetTime() < this->getFleeTimer()) {
			this->transition(state::roam);
		}
	}


	m_dolphin->set_position(m_dolphin->position() += m_dolphin->forward() * (float)this->getSpeed() * (float)time_step);
	//Updates animations
	m_dolphin->animated_mesh()->on_update((time_step / 4));

	//Sets collision spheres of dolphins
	glm::vec3 pos = m_dolphin->position();
	glm::vec3 forward = m_dolphin->forward();
	glm::vec3 up = m_dolphin->up();
	glm::vec3 p;
	getSphereSet()[0].on_update(pos);
	getSphereSet()[1].on_update(pos);
}

//Renders dolphin
void dolphin::on_render(const engine::ref<engine::shader>& shader, const engine::perspective_camera& camera)
{
	//Transforms dolphin to rotate towards the camera
	glm::mat4 dolphin_transform(1.0f);
	glm::vec3 dolphinp = this->m_dolphin->position();
	glm::vec3 dolphinc = this->getTarget();

	glm::vec3 dolphinv = dolphinc - dolphinp;
	float pi = 3.14159265358979323846;
	//rotation angle of dolphin in horizontal plane
	float dolphintheta = atan2(dolphinv.x, dolphinv.z) - pi;
	float deadzone = abs(atan2(dolphinv.z, dolphinv.x));
	float dolphinUpTheta;
	if ((deadzone < 2.3f) && (deadzone > 0.8f)) {
		dolphinUpTheta = atan2(dolphinv.y, abs(dolphinv.z));
	}
	else {
		dolphinUpTheta = atan2(dolphinv.y, abs(dolphinv.x));
	}
	//Transforms the dolphin
	dolphin_transform = glm::translate(dolphin_transform, this->m_dolphin->position());
	dolphin_transform = glm::rotate(dolphin_transform, dolphintheta, glm::vec3(0.f, 1.f, 0.f));
	dolphin_transform = glm::rotate(dolphin_transform, dolphinUpTheta, glm::vec3(1.f, 0.f, 0.f));
	dolphin_transform = glm::scale(dolphin_transform, this->m_dolphin->scale());
	engine::renderer::submit(shader, dolphin_transform, this->m_dolphin);
	//Sets the dolphin forward vector to the vector towards the target
	this->m_dolphin->set_forward(glm::normalize(dolphinv));
}



glm::vec3 dolphin::get_position() {
	return(m_dolphin->position());
}

void dolphin::transition(dolphin::state newstate) {
	if (newstate == state::roam) {
		this->m_mode = 0;
		this->pod = 0;
	}
	else if (newstate == state::pod) {
		this->m_mode = 1;
	}
	else if (newstate == state::flee) {
		this->m_mode = 3;
		this->setPod(0);
		int direction = rand() % 3 + 1;
		if (direction == 1) {
			this->setTarget(glm::vec3(-this->getTarget().x, this->getTarget().y, -this->getTarget().z));
		}
		else if (direction == 2) {
			this->setTarget(glm::vec3(this->getTarget().x, this->getTarget().y, -this->getTarget().z));

		}
		else if (direction == 3) {
			this->setTarget(glm::vec3(-this->getTarget().x, this->getTarget().y, this->getTarget().z));
		}
		this->setChangeTargetTime(glfwGetTime() + (rand() % 8 + 6));
		this->setFleeTimer(glfwGetTime()+2);
	}

	this->m_state = newstate;
}

void dolphin::back(glm::vec3 pos) {
	this->m_dolphin->set_position(pos);
	for (int i = 0; i < 2; i++) {
		this->getSphereSet()[i].on_update(pos);
	}
}
