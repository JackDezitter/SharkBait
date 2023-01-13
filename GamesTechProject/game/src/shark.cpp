#include "shark.h"
#include <string>
#include <GLFW/include/GLFW/glfw3.h>

shark::shark() {}

shark::~shark() {}

//Initiallizes Shark object sourced: https://www.cgtrader.com/free-3d-models/animals/fish/realistic-animated-great-white-shark
engine::game_object_properties shark::build() {
	//Creates animated skinned mesh
	engine::ref<engine::skinned_mesh> shark_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/shark/GreatWhiteShark_Mesh2.dae");
	//Loads annimation file for idle swimming
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_Idle1.dae");
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_Swimming1.dae");
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_Swimming2.dae");
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_Bite1.dae");
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_Bite2.dae");
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_DamageLeft.dae");
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_DamageRight.dae");
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_Death.dae");
	shark_skinned_mesh->LoadAnimationFile("assets/models/animated/shark/GreatWhiteShark_DeathIdle.dae");
	shark_skinned_mesh->switch_root_movement(true);
	this->shark_mesh = shark_skinned_mesh;
	//Sets shark object attributes and creates it
	engine::game_object_properties shark_props;
	engine::ref<engine::texture_2d> shark_texture = engine::texture_2d::create("assets/models/animated/shark/GreatWhiteShark_Grey_Scarred_BaseColor.png", true);
	shark_texture = engine::texture_2d::create("assets/models/animated/shark/GreatWhiteShark_Teeth_BaseColor.png", true);
	shark_texture = engine::texture_2d::create("assets/models/animated/shark/GreatWhiteShark_Eyes_Black_BaseColor.png", true);
	shark_texture = engine::texture_2d::create("assets/models/animated/shark/GreatWhiteShark_Gums_BaseColor.png", true);
	shark_props.animated_mesh = shark_skinned_mesh;
	shark_props.scale = glm::vec3(5.f);
	shark_props.textures = { shark_texture };
	shark_props.type = 0;
	shark_props.mass = 0.5f;
	shark_props.bounding_shape = glm::vec3(this->getMesh()->size().x, this->getMesh()->size().x, this->getMesh()->size().x)/2.f;

	return shark_props;
}

void shark::initialise(engine::game_object_properties shark_props, glm::vec3 spawnPosition, int wave) {
	this->shark_props = shark_props;
	
	this->shark_props.position = spawnPosition;
	this->m_shark = engine::game_object::create(this->shark_props);
	this->m_shark->set_offset(glm::vec3(this->getMesh()->offset().x, (shark_props.bounding_shape.x / (shark_props.scale.z * 4)), this->getMesh()->offset().z));

	this->setWave(wave);
	this->setSpeed(1.0f + (((m_wave - 1)) / 5));
	this->setHealth(100.0f + (20 * (m_wave - 1)));
	this->m_shark->set_position(spawnPosition);
	this->m_shark->set_forward(glm::vec3(0.f, 0.f, -1.f));
	this->m_shark->animated_mesh()->set_default_animation(0);
	this->setAnimation("swimming1");


	m_shark_box.set_box(shark_props.bounding_shape.x / (shark_props.scale.z*2), shark_props.bounding_shape.y / (shark_props.scale.z*2),
		shark_props.bounding_shape.z / (shark_props.scale.z*2), m_shark->position() - glm::vec3(0.f, this->m_shark->offset().y, 0.f));


	glm::vec3 pos = m_shark->position();
	glm::vec3 forward =m_shark->forward();
	glm::vec3 up = m_shark->up();
	glm::vec3 p;

	p = pos + 0.6f * forward;
	m_shark_sphere.set_sphere(shark_props.bounding_shape.y / (shark_props.scale.y * 30), p);
	m_shark_sphere_set.push_back(m_shark_sphere);

	p = pos + 0.45f * forward;
	m_shark_sphere.set_sphere(shark_props.bounding_shape.y / (shark_props.scale.y * 22), p);
	m_shark_sphere_set.push_back(m_shark_sphere);

	m_shark_sphere.set_sphere(shark_props.bounding_shape.y / (shark_props.scale.y * 20), m_shark->position());
	m_shark_sphere_set.push_back(m_shark_sphere);

	p = pos - 0.25f * forward - 0.1f*up;
	m_shark_sphere.set_sphere(shark_props.bounding_shape.y / (shark_props.scale.y * 25), p);
	m_shark_sphere_set.push_back(m_shark_sphere);;

	p = pos - 0.5f * forward - 0.1f * up;
	m_shark_sphere.set_sphere(shark_props.bounding_shape.y / (shark_props.scale.y * 38), p);
	m_shark_sphere_set.push_back(m_shark_sphere);;
	setSphereSet(m_shark_sphere_set);
	this->m_shark->set_angular_factor_lock(true);

}

//Updates Shark object animation and position
void shark::on_update(const engine::timestep& time_step, const engine::perspective_camera& camera, player& player1)
{
	double currentTime = glfwGetTime();
	glm::vec3 m_position = m_shark->position();
	m_position.x = glm::clamp(m_position.x, -19.0f, 19.0f);
	m_position.z = glm::clamp(m_position.z, -19.0f, 19.0f);
	glm::vec3 d = glm::vec3(m_position - (this->getTarget()));
	glm::vec3 nd = glm::vec3(m_shark->position() += m_shark->forward() * m_speed * (float)time_step - (this->getTarget()));

	if (m_state == state::passive) {
		if (((this->getChangeTargetTime() == 0.f) || (this->getChangeTargetTime() < currentTime) || length(d) == 0 || d.x * nd.x < 0 || d.y * nd.y < 0 || d.z * nd.z < 0)) {
			float targetx = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float targetz = -18.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (18.8 - (-18.8))));
			float minY = this->getTerrain().ground_height(glm::vec3(targetx, 0, targetz) + 0.5f);
			float targety = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - minY)));
			this->setTarget(glm::vec3(targetx, targety, targetz));
			this->setChangeTargetTime(currentTime + (rand() % 10 + 5));
			this->m_shark->turn_towards(this->getTarget());
		}
	}
	else if (m_state == state::attack) {
		this->setTarget(camera.position());
		if (length(this->m_shark->position() - camera.position()) < 10) {
			this->transition(state::strike);
		}
	}
	else if (m_state == state::strike) {
		glm::vec3 centre;
		float radius;
		m_shark_sphere_set[0].get(centre, radius);
		this->setTarget(camera.position());
		d = glm::vec3(centre - (this->getTarget()));
		nd = glm::vec3(centre += m_shark->forward() * this->getSpeed() * (float)time_step - (this->getTarget()));
		if (((length(d) - radius <= 2.f || d.x * nd.x < 0 || d.y * nd.y < 0 || d.z * nd.z < 0))) {
			if (this->biteTimer <= 0.0f) {
				this->m_shark->animated_mesh()->switch_root_movement(true);
				this->biteTimer = m_shark->animated_mesh()->animations().at(3)->mDuration;
				this->setAnimation("bite2");
			}
			else {
				this->biteTimer -= (float)time_step;
			}
		}
		if (length(d - radius) <= 0 || d.x * nd.x < 0 || d.y * nd.y < 0 || d.z * nd.z < 0){
			this->resetAttackCooldown();
			this->setAnimation("swimming1");
			player1.setHealth(player1.getHealth() - (this->getDamage()));
			transition(state::flee);
		}
	}
	else if (m_state == state::dying) {
		if (this->deathTimer > 0.0f)
		{
			this->deathTimer -= (float)time_step;
			if (this->deathTimer < 0.0f)
			{
				this->m_shark->animated_mesh()->switch_root_movement(false);
				this->transition(state::dead);
			}
		}
		m_shark->animated_mesh()->on_update((time_step / 2));
	}
	else if (m_state == state::flee) {
		this->transition(state::passive);
	}

	if (m_state != state::dying && m_state != state::dead && this->getHealth() > 0) {
		m_shark->set_position(m_shark->position() += m_shark->forward() * (float)this->getSpeed() * (float)time_step);
		m_shark_box.on_update(m_shark->position() - glm::vec3(0.f, object()->offset().y, 0.f));
		//Updates animations
		m_shark->animated_mesh()->on_update((time_step / 2) * this->getSpeed());

		//Sets collision spheres of sharks
		glm::vec3 pos = m_shark->position();
		glm::vec3 forward = m_shark->forward();
		glm::vec3 up = m_shark->up();
		glm::vec3 p;

		p = pos + 0.5f * forward + 0.05f * up;
		getSphereSet()[0].on_update(p);
		p = pos + 0.27f * forward;
		getSphereSet()[1].on_update(p);
		getSphereSet()[2].on_update(m_shark->position());
		p = pos - 0.25f * forward - 0.05f * up;
		getSphereSet()[3].on_update(p);
		p = pos - 0.5f * forward - 0.06f * up;
		getSphereSet()[4].on_update(p);
	}
	else if (m_state != state::dying && m_state != state::dead && this->getHealth() <= 0) {
		this->transition(state::dying);
	}
	
}
//Renders shark
void shark::on_render(const engine::ref<engine::shader>& shader, const engine::perspective_camera& camera)
{
	//Transforms Shark to rotate towards the camera
	glm::mat4 shark_transform(1.0f);
	glm::vec3 sharkp = this->m_shark->position();
	glm::vec3 sharkc = this->getTarget();
	
	glm::vec3 sharkv = sharkc - sharkp;
	float pi = 3.14159265358979323846;
	//rotation angle of shark in horizontal plane
	float sharktheta = atan2(sharkv.x, sharkv.z) + pi / 2;
	//Calculates rotation angle of shark in y plane. Had issues when the horizontal plane component of the shark calculation was the direction the camera was in so if the player
	//is too close to the x axis it calculates using the z
	float deadzone = abs(atan2(sharkv.z, sharkv.x));
	float sharkUpTheta;
	if ((deadzone < 2.3f) && (deadzone > 0.8f)) {
		sharkUpTheta = -atan2(sharkv.y, abs(sharkv.z));
	}
	else {
		sharkUpTheta = -atan2(sharkv.y, abs(sharkv.x));
	}
	//Transforms the Shark
	shark_transform = glm::translate(shark_transform, this->m_shark->position());
	shark_transform = glm::rotate(shark_transform, sharktheta, glm::vec3(0.f, 1.f, 0.f));
	shark_transform = glm::rotate(shark_transform, sharkUpTheta, glm::vec3(0.f, 0.f, 1.f));
	shark_transform = glm::scale(shark_transform, this->m_shark->scale());
	engine::renderer::submit(shader, shark_transform, this->m_shark);
	//Sets the shark forward vector to the vector towards the camera
	this->m_shark->set_forward(glm::normalize(sharkv));
	//this->m_shark_box.on_render(2.5f, 0.f, 0.f, shader);
	for (int i = 0; i < 5; i++) {
		//this->getSphereSet()[i].on_render(25, 0.f, 0.f, shader);
		LOG_CORE_TRACE(i);
	}
}

void shark::setAnimation(std::string animation) {
	if (animation == "idle1") {
		this->m_shark->animated_mesh()->switch_animation(0);
	}
	else if (animation == "swimming1") {
		this->m_shark->animated_mesh()->switch_animation(1);
	}
	else if (animation == "swimming2") {
		this->m_shark->animated_mesh()->switch_animation(2);
	}
	else if (animation == "bite1") {
		this->m_shark->animated_mesh()->switch_animation(3);
	}
	else if (animation == "bite2") {
		this->m_shark->animated_mesh()->switch_animation(4);
	}
	else if (animation == "damageleft") {
		this->m_shark->animated_mesh()->switch_animation(5);
	}
	else if (animation == "damageright") {
		this->m_shark->animated_mesh()->switch_animation(6);
	}
	else if (animation == "death"){
		this->m_shark->animated_mesh()->switch_animation(7);
	}
	else if (animation == "deathidle") {
		this->m_shark->animated_mesh()->switch_animation(8);
	}
}

glm::vec3 shark::get_position() {
	return(m_shark->position());
}

void shark::transition(shark::state newstate) {
	if (newstate == state::passive) {
		this->m_mode = 0;
		this->setSpeed(1.0f + ((2 * (this->getWave() - 1)) / 10));
		this->setAnimation("swimming2");
	}
	else if (newstate == state::attack) {
		this->m_mode = 1;
		this->setSpeed((1.0f + ((2 * (this->getWave() - 1)) / 10)) + 1.f);
	}
	else if (newstate == state::strike) {
		this->m_mode = 2;
		this->setSpeed((1.0f + ((2 * (this->getWave() - 1)) / 10)) + 0.5f);
		this->setAnimation("idle1");
	}
	else if (newstate == state::flee) {
		this->m_mode = 3;
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
	}
	else if (newstate == state::dying) {
		this->m_mode = -1;
		this->setAnimation("death");
		this->deathTimer = m_shark->animated_mesh()->animations().at(7)->mDuration + 1;
	}
	else if (newstate == state::dead) {
		this->setAnimation("deathIdle");
		this->m_mode = -2;
	}
	this->m_state = newstate;
}
void shark::back(const engine::timestep& time_step) {
	this->m_shark->set_position(this->m_shark->position() -= 4.f*(m_shark->forward() * (float)this->getSpeed() * (float)time_step));
}
