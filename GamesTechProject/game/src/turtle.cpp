#include "turtle.h"
#include <string>
#include <GLFW/include/GLFW/glfw3.h>

turtle::turtle() {}

turtle::~turtle() {}

//Initiallizes turtle object;
engine::game_object_properties turtle::build() {

	engine::ref <engine::model> turtle_model = engine::model::create("assets/models/static/turtle.obj");
	engine::game_object_properties turtle_props;
	engine::ref<engine::texture_2d> turtle_texture = engine::texture_2d::create("assets/models/static/10042_Sea_Turtle_V1_Diffuse.jpg", true);
	turtle_props.meshes = turtle_model->meshes();
	turtle_props.textures = { turtle_texture };
	turtle_props.rotation_amount = 1.f;
	turtle_props.rotation_axis = glm::vec3(0, 1.f, 0);
	float turtle_scale = 0.3f;
	turtle_props.scale = glm::vec3(turtle_scale);
	turtle_props.bounding_shape = turtle_model->size() / 2.f * turtle_scale;

	return turtle_props;
}

void turtle::initialise(engine::game_object_properties turtle_props, glm::vec3 spawnPosition) {
	this->turtle_props = turtle_props;
	this->turtle_props.position = spawnPosition;
	this->m_turtle = engine::game_object::create(this->turtle_props);

	this->setSpeed(1.0f);
	this->setHealth(100.0f);
	this->m_turtle->set_position(spawnPosition);
	this->m_turtle->set_forward(glm::vec3(0.f, 0.f, -1.f));

	m_turtle_sphere.set_sphere(0.25, m_turtle->position());
	glm::vec3 pos = m_turtle->position();
	glm::vec3 forward = m_turtle->forward();
	glm::vec3 up = m_turtle->up();

	this->m_turtle->set_angular_factor_lock(true);

}

//Updates turtle position and target
void turtle::on_update(const engine::timestep& time_step, const engine::perspective_camera& camera, player& player1)
{
	double currentTime = glfwGetTime();
	glm::vec3 m_position = m_turtle->position();
	m_position.x = glm::clamp(m_position.x, -19.0f, 19.0f);
	m_position.z = glm::clamp(m_position.z, -19.0f, 19.0f);
	glm::vec3 d = glm::vec3(m_position - (this->getTarget()));
	glm::vec3 nd = glm::vec3(m_turtle->position() += m_turtle->forward() * m_speed * (float)time_step - (this->getTarget()));

	if (m_state == state::passive) {
		if (((this->getChangeTargetTime() == 0.f) || (this->getChangeTargetTime() < currentTime) || length(d) == 0 || d.x * nd.x < 0 || d.y * nd.y < 0 || d.z * nd.z < 0)) {
			float targetx = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float targetz = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float minY = this->getTerrain().ground_height(glm::vec3(targetx, 0, targetz) + 0.5f);
			float targety = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - minY)));
			this->setTarget(glm::vec3(targetx, targety, targetz));
			this->setChangeTargetTime(currentTime + (rand() % 10 + 5));
			this->m_turtle->turn_towards(this->getTarget());
		}
	}
	
	else if (m_state == state::flee) {
		this->transition(state::passive);
	}

	if (m_state != state::dead && this->getHealth() > 0) {
		m_turtle->set_position(m_turtle->position() += m_turtle->forward() * (float)this->getSpeed() * (float)time_step);
		m_turtle_sphere.on_update(m_turtle->position());
		//Sets collision spheres of turtles
		glm::vec3 pos = m_turtle->position();
		glm::vec3 forward = m_turtle->forward();
		glm::vec3 up = m_turtle->up();
		glm::vec3 p = pos + 0.5f * forward + 0.05f * up;
	}
	else if (m_state != state::dead && this->getHealth() <= 0) {
		this->transition(state::dead);
	}
}
//Renders turtle
void turtle::on_render(const engine::ref<engine::shader>& shader, const engine::perspective_camera& camera)
{
	//Transforms turtle to rotate towards the camera
	glm::mat4 turtle_transform(1.0f);
	glm::vec3 turtlep = this->m_turtle->position();
	glm::vec3 turtlec = this->getTarget();

	glm::vec3 turtlev = turtlec - turtlep;
	float pi = 3.14159265358979323846;
	//rotation angle of turtle in horizontal plane
	float turtletheta = atan2(turtlev.x, turtlev.z) + pi / 2;
	//Calculates rotation angle of turtle in y plane. 
	float deadzone = abs(atan2(turtlev.z, turtlev.x));
	float turtleUpTheta;
	if ((deadzone < 2.3f) && (deadzone > 0.8f)) {
		turtleUpTheta = -atan2(turtlev.y, abs(turtlev.z));
	}
	else {
		turtleUpTheta = -atan2(turtlev.y, abs(turtlev.x));
	}
	//Transforms the turtle
	turtle_transform = glm::translate(turtle_transform, this->m_turtle->position());
	turtle_transform = glm::rotate(turtle_transform, turtletheta, glm::vec3(0.f, 1.f, 0.f));
	turtle_transform = glm::rotate(turtle_transform, turtleUpTheta, glm::vec3(0.f, 0.f, 1.f));
	turtle_transform = glm::scale(turtle_transform, this->m_turtle->scale());
	engine::renderer::submit(shader, turtle_transform, this->m_turtle);
	//Sets the turtle forward vector to the vector towards the target
	this->m_turtle->set_forward(glm::normalize(turtlev));
}


glm::vec3 turtle::get_position() {
	return(m_turtle->position());
}

void turtle::transition(turtle::state newstate) {
	if (newstate == state::passive) {
		this->setSpeed(1.0f);
		this->m_mode = 0;
	}
	else if (newstate == state::dead) {
		this->m_mode = -2;
	}
	else if (newstate == state::flee) {
		int direction = rand() % 2 + 1;
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
		this->m_mode = 3;
	}
	this->m_state = newstate;
}
void turtle::back(const engine::timestep& time_step) {
	this->m_turtle->set_position(this->m_turtle->position() -= 2.f * (m_turtle->forward() * (float)this->getSpeed() * (float)time_step));
}
