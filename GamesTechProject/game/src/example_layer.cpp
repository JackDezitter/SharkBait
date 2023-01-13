#include "example_layer.h"
#include "platform/opengl/gl_shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/events/mouse_event.h"
#include "engine/utils/track.h"
#include <GLFW/include/GLFW/glfw3.h>




example_layer::example_layer()
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f), 
    m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())

{
	//Sets up blue thin fog
	m_fog = true;
	glm::vec3 m_fog_colour = glm::vec3(0.1f, 0.2f, 0.3f);
	int m_fog_factor_type = 1;

	// Hide the mouse and lock it inside the window
    engine::application::window().hide_mouse_cursor();

	m_freq_high = 0.f;
	m_freq_low = 20000.f;

	// Initialise audio and background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	//Menu Music
	m_audio_manager->load_sound("assets/audio/dark-cinematic-suspenseful-ambient-111682.mp3", engine::sound_type::track, "menuMusic");
	//Game music
	m_audio_manager->load_sound("assets/audio/talaso-extasis-119659.mp3", engine::sound_type::track, "music");
	//Bubble sound effect for the emitters
	m_audio_manager->load_sound("assets/audio/bubbling-6184.mp3", engine::sound_type::spatialised, "bubble");
	//Bite sound effect when shark attacks
	m_audio_manager->load_sound("assets/audio/monster-bite-44538trim.mp3", engine::sound_type::spatialised, "bite");
	//Gunshot sound effect when player shoots
	m_audio_manager->load_sound("assets/audio/gunshot-37055.mp3", engine::sound_type::spatialised, "shot");
	m_audio_manager->loop("music", true);
	m_audio_manager->loop("menuMusic", true);
	m_audio_manager->play("music");
	m_audio_manager->pause("music");
	//Play the menu music while the game loads
	m_audio_manager->play("menuMusic");
	m_audio_manager->create_high_pass_filter();
	m_audio_manager->set_high_pass_filter(m_freq_high);
	m_audio_manager->create_low_pass_filter();
	m_audio_manager->set_low_pass_filter(m_freq_low);


	// Initialise the shaders, materials and lights
	auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");

	//Directional Light properties initialized
	m_directionalLight.Color = glm::vec3(0.5f, 0.7f, 0.9f);
	m_directionalLight.AmbientIntensity = 0.5f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(0.0f, -1.0f, -10.0f));

	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("lighting_on", true);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gMatSpecularIntensity", 1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gSpecularPower", 10.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("transparency", 1.f);
	m_directionalLight.submit(mesh_shader);


	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));

	//Creates bubble material
	m_bubble_material = engine::material::create(0.5f, glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.f, 2.f, 2.f), 0.3f);

	//Creates jellyfish material
	m_jelly_material = engine::material::create(0.4f, glm::vec3(1.0f, 0.71f, 0.75f),
		glm::vec3(1.0f, 0.71f, 0.75f), glm::vec3(1.f, 1.f, 1.f), 0.8f);

	//Creates base material
	m_material = engine::material::create(0.3f, glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.f, 2.f, 2.f), 1.f);

	//Creates water surface material
	m_surface_material = engine::material::create(0.5f, glm::vec3(0.5f, 0.7f, 0.9f),
		glm::vec3(0.5f, 0.7f, 0.9f), glm::vec3(0.5f, 0.7f, 0.9f), 0.9f);

	//Shark Prep
	//Creates a list of sharks in order to spawn multiple and keep track of their information
	shark_list = std::vector<shark>();
	//Initialise Shark material
	m_shark_material = engine::material::create(1.0f, glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);
	//Builds 20 shark models and adds them to a list
	//I had to do this because the animation is taken in by reference and if I used 1 model to initialize all the sharks, if one changed animation they all change
	for (int i = 0; i < 20; i++) {
		build_shark = m_shark.build();
		build_shark_list.push_back(build_shark);
	}

	//Builds objects
	//Builds dolphin model
	build_dolphin = m_dolphin.build();
	//Buuilds turtle model
	build_turtle = m_turtle.build();
	//Builds jellyfish model
	build_jellyfish = m_jellyfish.build();
	//Builds bubble emitter
	engine::game_object_properties emiitter_build = m_bubble_emitter.build();
	//Builds projectile
	build_proj = m_ballistic.build();
	//Ballistic Prep
	//Creates a list of ballistics in order to spawn multiple and keep track of their information
	ballistic_list = std::vector<ballistic>();
	//Initialise ballistic material
	m_ballistic_material = engine::material::create(1.f, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 1.0f);

	//Initializes list of pick-up items
	pickup_list = std::vector<pickup>();

	//Skybox Creation
	// Skybox texture from https://res.cloudinary.com/practicaldev/image/fetch/s--jS7rZiAv--/c_limit%2Cf_auto%2Cfl_progressive%2Cq_auto%2Cw_880/https://git-tutor-assets.s3.eu-west-2.amazonaws.com/skybox.png
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/right.bmp", true),
		engine::texture_2d::create("assets/textures/skybox/front.bmp", true),
		engine::texture_2d::create("assets/textures/skybox/left.bmp", true),
		engine::texture_2d::create("assets/textures/skybox/back.bmp", true),
		engine::texture_2d::create("assets/textures/skybox/up.bmp", true),
		engine::texture_2d::create("assets/textures/skybox/down.bmp", true)
		});

	//Waterbox Creation
	m_waterbox = engine::skybox::create(20.f,
		{ engine::texture_2d::create("assets/textures/waterbox/pz.bmp", true),
		engine::texture_2d::create("assets/textures/waterbox/px.bmp", true),
		engine::texture_2d::create("assets/textures/waterbox/nz.bmp", true),
		engine::texture_2d::create("assets/textures/waterbox/nx.bmp", true),
		engine::texture_2d::create("assets/textures/waterbox/py.bmp", true),
		engine::texture_2d::create("assets/textures/waterbox/ny.bmp", false)
		});

	//Creates heighmapped terrain
	m_heightmap = engine::heightmap::create("assets/textures/RealHeightmap4.bmp", "assets/textures/SandyBottom.bmp", 50.f, 50.f, glm::vec3(0.f, 0.5f, 0.f), 40.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { m_heightmap->mesh() };
	terrain_props.textures = { m_heightmap->texture() };
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.1f, 100.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	//Creates water surface plane
	engine::ref<engine::terrain> surface_shape = engine::terrain::create(100.f, 0.1f, 100.f);
	engine::game_object_properties surface_props;
	surface_props.meshes = { surface_shape->mesh() };
	surface_props.is_static = true;
	surface_props.type = 0;
	surface_props.position = glm::vec3(0, 20, 0);
	surface_props.bounding_shape = glm::vec3(100.f, 0.5f, 100.f);
	surface_props.restitution = 0.92f;
	m_surface = engine::game_object::create(surface_props);

	//Randomizes position and initializes 5 bubble emitters on the surface of the terrain
	//Creates an orange point light at each location of bubble emitter to draw the players attention to them
	float xpos = 4;
	float zpos = 10.5;
	float ypos = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));


	m_bubble_emitter.init(emiitter_build, glm::vec3(xpos, ypos, zpos));
	m_pointLight.Color = glm::vec3(1.f, 0.64f, 0.0f);
	m_pointLight.AmbientIntensity = 0.1f;
	m_pointLight.DiffuseIntensity = 0.8f;
	m_pointLight.Position = glm::vec3(xpos, ypos + 0.1f, zpos);
	m_pointLight.Attenuation.Exp = 0.1f;

	emiiter_list.push_back(m_bubble_emitter);
	pointLight_list.push_back(m_pointLight);

	xpos = -5.75;
	zpos = -12.5;
	ypos = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));

	m_bubble_emitter.init(emiitter_build, glm::vec3(xpos, ypos, zpos));
	m_pointLight.Position = glm::vec3(xpos, ypos + 0.2f, zpos);

	emiiter_list.push_back(m_bubble_emitter);
	pointLight_list.push_back(m_pointLight);

	xpos = 9;
	zpos = -10.25;
	ypos = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));

	m_bubble_emitter.init(emiitter_build, glm::vec3(xpos, ypos, zpos));
	m_pointLight.Position = glm::vec3(xpos, ypos + 0.2f, zpos);

	emiiter_list.push_back(m_bubble_emitter);
	pointLight_list.push_back(m_pointLight);

	xpos = -12;
	zpos = 11;
	ypos = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));

	m_bubble_emitter.init(emiitter_build, glm::vec3(xpos, ypos, zpos));
	m_pointLight.Position = glm::vec3(xpos, ypos + 0.2f, zpos);

	emiiter_list.push_back(m_bubble_emitter);
	pointLight_list.push_back(m_pointLight);

	xpos = -12;
	zpos = 0;
	ypos = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));

	m_bubble_emitter.init(emiitter_build, glm::vec3(xpos, ypos, zpos));
	m_pointLight.Position = glm::vec3(xpos, ypos + 0.2f, zpos);

	emiiter_list.push_back(m_bubble_emitter);
	pointLight_list.push_back(m_pointLight);

	//Builds and initializes the sucken ship model at a set position
	xpos = 12;
	zpos = -3;;
	ypos = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos)-1.f);

	engine::game_object_properties boat_props;
	engine::ref <engine::model> boat_model = engine::model::create("assets/models/static/Boat2.obj");
	engine::ref<engine::texture_2d> boat_texture = engine::texture_2d::create("assets/models/static/boat_diffuse.bmp", true);
	boat_props.meshes = boat_model->meshes();
	boat_props.textures = { boat_texture };
	float boat_scale = 5.f;
	boat_props.scale = glm::vec3(boat_scale);
	boat_props.rotation_axis = glm::vec3(0.1f, 1, 0.1f);
	boat_props.rotation_amount = engine::PI;
	boat_props.position = {glm::vec3(xpos,ypos,zpos)};
	m_boat = engine::game_object::create(boat_props);

	//Places a directional spotlight at the front of the ship pointing towards the centre of the map
	xpos = 9.25;
	zpos = -2.7f;

	m_spotLight.Color = glm::vec3(1.f, 1.f, 1.f);
	m_spotLight.AmbientIntensity = 2.f;
	m_spotLight.DiffuseIntensity = 2.f;
	m_spotLight.Position = glm::vec3(xpos, ypos + 1.65f, zpos);
	m_spotLight.Attenuation.Linear = 0.5f;
	m_spotLight.Cutoff = 0.8f;
	m_spotLight.Direction = glm::normalize(glm::vec3(-1, -0.05f , 0));

	//Creates a lamp using the hexcone primative to act as the spotlight on the ship where the light is coming from
	engine::game_object_properties hexcone_props;
	engine::ref<engine::hexcone> hexcone_shape = engine::hexcone::create();
	hexcone_props.meshes = { hexcone_shape->mesh() };
	hexcone_props.position = glm::vec3(xpos, ypos + 1.62f, zpos);
	hexcone_props.rotation_amount = -engine::PI/2;
	hexcone_props.rotation_axis = glm::vec3(0, 1, 0);
	m_lamp = engine::game_object::create(hexcone_props);

	//Creates a rock out of the prism primative shape and places it at the origin of the terrain
	xpos = 0.f;
	zpos = 0.f;
	ypos = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));
	engine::game_object_properties prism_props;
	engine::ref<engine::prism> prism_shape = engine::prism::create();
	engine::ref<engine::texture_2d> prism_texture = engine::texture_2d::create("assets/textures/rock.jpg", true);
	prism_props.meshes = { prism_shape->mesh()};
	prism_props.textures = { prism_texture };
	prism_props.position = glm::vec3(xpos, ypos, zpos);
	m_prism = engine::game_object::create(prism_props);

	//Creates a directional light source that will be attached the the player's gun and act as a flashlight/torch
	m_torch.Color = glm::vec3(1.f, 1.f, 1.f);
	m_torch.AmbientIntensity = 0.5f;
	m_torch.DiffuseIntensity = 0.f;
	m_torch.Attenuation.Linear = 0.2f;
	m_torch.Cutoff = 0.9f;

	//Initializes player and attributes
	m_player.init();

	// Load the gun model and texture. Create a gun object. Set its initial properties
	engine::ref <engine::model> gun_model = engine::model::create("assets/models/static/Harpoon2.obj");
	engine::game_object_properties gun_props;
	engine::ref<engine::texture_2d> gun_texture = engine::texture_2d::create("assets/textures/harpoon/Harpoon_Low_Harpoon_Gun_BaseColor.png", true);
	gun_props.meshes = gun_model->meshes();
	gun_props.textures = { gun_texture };
	gun_props.rotation_amount = 1.f;
	gun_props.rotation_axis = glm::vec3(0, 1.f, 0);
	float gun_scale = 0.5f;
	gun_props.position = glm::vec3(1,1,1);
	gun_props.scale = glm::vec3(gun_scale);
	gun_props.bounding_shape = gun_model->size() / 2.f * gun_scale;
	m_gun = engine::game_object::create(gun_props);

	m_game_objects.push_back(m_terrain);

	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	m_text_manager = engine::text_manager::create();
	//Creates crossfade element that goes to a red screen, used for when the player loses health as an indicator to the player
	m_cross_fade = cross_fade::create("assets/textures/RedBackground.jpg", 2.0f, 1.6f, 0.9f);

	//Creates Main menu with the corresponding image to be rendered in the ortho camera
	m_menu = menu::create("assets/textures/MenuResize.png", 1.6f, 0.9f);
	//Sets menu flag to be on so the player loads into the main menu before the game begins
	menuOn = true;

	//Creates death menu with the corresponding image to be rendered in the ortho camera
	death_menu = menu::create("assets/textures/Death.png", 1.6f, 0.9f);
	//Sets flag of death menu to be off
	deathMenuOn = false;


}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	//If there is no menu flag update the game elements
	if (!menuOn && !deathMenuOn) {

		m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));

		m_audio_manager->update_with_camera(m_3d_camera);

		m_cross_fade->on_update(time_step);

		//Retrieves players health before the update, compared top the players health after the update to know whether to show the damage crossfade
		float playerHealth = m_player.getHealth();
		//Updates each ballistic in the bullet list
		for (int i = 0; i < ballistic_list.size(); i++) {
			ballistic_list[i].on_update(time_step);
			//If out of bounds the ballistics object is removed from the list and the game
			if (ballistic_list[i].get_position().x > 20 || ballistic_list[i].get_position().y > 30 || ballistic_list[i].get_position().z > 20 || ballistic_list[i].get_position().x < -20 || ballistic_list[i].get_position().y < -20 || ballistic_list[i].get_position().z < -20) {
				ballistic_list.erase(ballistic_list.begin() + i);
				i -= 1;
			}
		}

		//Shark Updaters
		//Counts number of shark currently attacking
		int attack_count = 0;
		for (int i = 0; i < shark_list.size(); i++) {
			if (shark_list[i].getMode() >= 1) {
				attack_count++;
			}
		}
		//Collision management for shark
		//Updates each shark in the shark list causing them to move and rotate
		for (int i = 0; i < shark_list.size(); i++) {
			//If the shark is in the dead state, remove it frommt he list and check the location in the list again
			if (shark_list[i].getMode() == -2) {
				shark_list.erase(shark_list.begin() + i);
				i -= 1;
			}
			//If the shark is not dead
			else {
				//Sets a shark to attack mode if it is not on cooldown and if there are less than 2 sharks attacking the player
				if (attack_count < 2 && glfwGetTime() >= shark_list[i].getAttackCooldown()) {
					if ((0 + (std::rand() % (200 - 1)) == 3)) {
						shark_list[i].setMode(1);
						attack_count++;
					}
				}
				//Update the shark
				shark_list[i].on_update(time_step, m_3d_camera, m_player);
				//If the player has lost health since the shark update, i.e the shark has bitten them, play bite sound at the shark's position
				if (m_player.getHealth() < playerHealth) {
					m_audio_manager->play_spatialised_sound("bite", m_3d_camera.position(),shark_list[i].get_position());
				}
				//If there a bullets currently in the game, check for collision against the shark
				if (ballistic_list.size() != 0) {
					//Checks if any bullets have collided with the sharks wider box
					for (int j = 0; j < ballistic_list.size(); j++) {
						if ((shark_list[i].getBox().collision(ballistic_list[j].getBox()) && (shark_list[i].getMode() >= 0))) {
							//If a box collision was detetcted, check with the sharks set of smaller spheres, spheres move dynamically with the shark
							for (int k = 0; k < 5; k++) {
								if (shark_list[i].getSphereSet()[k].collisionBox(ballistic_list[j].getBox())) {
									//If there is a bullet collision with a shark sphere
									//Reduce the sharks health
									shark_list[i].setHealth(shark_list[i].getHealth() - 40);
									//Remove that bullet from the ballistic list
									ballistic_list.erase(ballistic_list.begin() + j);
									//Reduce j
									j -= 1;
									//Break as there is no need to check that bullets collisions against spheres anymore
									break;
								}
							}
						}
					}
				}
				//Checks collision with other sharks so the sharks don't phase into each-other
				for (int o = 0; o < shark_list.size(); o++) {
					//Don't check against itself
					if (o != i) {
						//If 2 sharks boxes are colliding and they are not dead
						if (shark_list[i].getBox().collision(shark_list[o].getBox()) &&
							((shark_list[i].getMode() >= 0) && (shark_list[i].getMode() <= 2) &&
								(shark_list[o].getMode() >= 0) && (shark_list[o].getMode() <= 2))) {
							//Check each sharks sphere and see if they collide
							for (int k = 0; k < 3; k++) {
								for (int l = 0; l < 5; l++) {
									if (shark_list[i].getSphereSet()[k].collision(shark_list[o].getSphereSet()[l])) {
										//If there is a sphere collision, set 1 shark to move in the direction away from the shark and return to passive mode
										if (i < o) {
											shark_list[i].setTarget(-(shark_list[o].get_position() - shark_list[i].get_position()));
											shark_list[i].setChangeTargetTime(glfwGetTime() + 2);
											shark_list[i].setMode(0);
											break;
										}
									}
								}
							}
						}
					}
				}

			}

		}
		//If pick-up list is empty due to the world iniitalizing
		if (pickup_list.empty()) {
			//Creates 3 pick-up objects of type health and 2 of type ammo and randomly places them with-in the waterbox then adds them to the pickup_list
			for (int i = 0; i < 3; i++) {
				float xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
				float zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
				float minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 0.5f);
				float ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
				m_pickup.init("health", glm::vec3(xpos, ypos, zpos));
				pickup_list.push_back(m_pickup);
			}

			for (int i = 0; i < 3; i++) {
				float xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
				float zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
				float minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 0.5f);
				float ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
				m_pickup.init("ammo", glm::vec3(xpos, ypos, zpos));
				pickup_list.push_back(m_pickup);
			}
		}
		// Updates each pick-up 
		for (int i = 0; i < pickup_list.size(); i++) {
			pickup_list[i].update(m_3d_camera.position(), m_player, time_step);
			//If the oxygen bubbles pick-up is not active remopve it from the pick-up list
			if (pickup_list[i].active() == false && pickup_list[i].getType() == "oxygen") {
				pickup_list.erase(pickup_list.begin() + i);
				i -= 1;
			}
		}
		//Player oxygen management
		//Removes 1 oxygen point per second if the player is underwater
		if (m_player.getOxygenCounter() <= glfwGetTime() && m_3d_camera.position().y <= 20) {
			m_player.setOxygen(m_player.getOxygen() - 1);
			m_player.setOxygenCounter();
		}
		//If the player has no oxygen begin taking health and 1 point every 2 seconds
		else if (m_player.getOxygen() <= 0 && m_player.getDrowningTimer() <= glfwGetTime()) {
			m_player.setHealth(m_player.getHealth() - 1);
			m_player.setDrowningTimer(glfwGetTime() + 2);
		}
		//If the player is above the water's surface increase the players oxygen gradually to 100 points
		else if (m_3d_camera.position().y > 20) {
			m_player.setOxygen(m_player.getOxygen() + 1);
			m_player.setOxygenCounter();
		}

		//Updates each bubble emitter to periodically release bubbles
		for (int i = 0; i < emiiter_list.size(); i++) {
			if (emiiter_list[i].getEmitterTimer() <= glfwGetTime()) {
				m_pickup = emiiter_list[i].update();
				//Adds oxygen bubble created from the emitter to the list of pick-up items
				pickup_list.push_back(m_pickup);
				//When a bubble is created play the bubble sound effect from the emitters location
				m_audio_manager->play_spatialised_sound("bubble", m_3d_camera.position(), emiiter_list[i].getObject()->position());
			}
		}

		//Initialize wave level sharks and turtles and pick-ups
		if (shark_list.empty()) {
			//Check how many turtles are currently in the scene compared to the 4 maximum
			int turtles_added = 4 - turtle_list.size();
			//Create however many turtles are missing at a random position in the playable area and add them to the turtle list
			for (int i = 0; i < turtles_added; i++) {
				float xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
				float zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
				float minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));
				float ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
				m_turtle.initialise(build_turtle, glm::vec3(xpos, ypos, zpos));
				turtle_list.push_back(m_turtle);
			}
			//Sets all pick-up items in the list to active
			for (int i = 0; i < pickup_list.size(); i++) {
				pickup_list[i].setActive();
			}
			//Creates Sharks corresponding wave amount of sharks in random positions with-in the waterbox and adds them to the shark_list
			current_wave++;
			int maxSharks = 3 + current_wave;
			//Set the maximum number of sharks to being 20 as that is howmany models we built
			if (maxSharks > 20) {
				maxSharks = 20;
			}
			for (int i = 0; i < maxSharks; i++) {
				float xpos = -20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - (-20))));
				float zpos = -20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - (-20))));
				float minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));
				float ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (20 - minY)));
				m_shark.initialise(build_shark_list[i], glm::vec3(xpos, ypos, zpos), current_wave);
				shark_list.push_back(m_shark);
			}
		}

		//Turtle Collisions and updates
		for (int i = 0; i < turtle_list.size(); i++) {
			//If the turtle isn't dead
			if (turtle_list[i].getMode() != -2) {
				//Update the turtle
				turtle_list[i].on_update(time_step, m_3d_camera, m_player);
				//Turtle collisions with bullets
				if (ballistic_list.size() != 0) {
					//For each bullet in sncene
					for (int j = 0; j < ballistic_list.size(); j++) {
						//Check if the turtle's sphere collides with the pbullets box
						if ((turtle_list[i].getSphere().collisionBox(ballistic_list[j].getBox()) && (turtle_list[i].getMode() >= 0))) {
							//If successful collision, remove 25 from the turtles health
							turtle_list[i].setHealth(turtle_list[i].getHealth() - 20);
							//Remove that bullet from the scene
							ballistic_list.erase(ballistic_list.begin() + j);
							j -= 1;
							break;
						}
					}
				}
				//Turtle collision with other turtles
				for (int o = 0; o < turtle_list.size(); o++) {
					if (o != i) {
						//Checks if turtle i  sphere collisdes with turtle o's sphere
						if (turtle_list[i].getSphere().collision(turtle_list[o].getSphere()) &&
							(turtle_list[i].getMode() == 0) && (turtle_list[o].getMode() == 0)) {
							if (i < o) {
								//If collision, set this turtle to move away from turtle o's position
								turtle_list[i].setTarget(turtle_list[i].get_position() + (turtle_list[i].get_position() - turtle_list[o].get_position()));
								turtle_list[i].setChangeTargetTime(glfwGetTime() + 2);
								turtle_list[i].setMode(0);
								break;
							}
						}
					}
				}
				//If collides with a shark's collision box
				for (int o = 0; o < shark_list.size(); o++) {
					if (turtle_list[i].getSphere().collisionBox(shark_list[o].getBox())) {
						//Run away from shark's position
						turtle_list[i].setTarget(turtle_list[i].get_position() + (turtle_list[i].get_position() - shark_list[o].get_position()));
						turtle_list[i].object()->turn_towards(turtle_list[i].getTarget());
						turtle_list[i].setChangeTargetTime(glfwGetTime() + 2);
						turtle_list[i].setMode(0);
					}
				}
			}
			//If turtle dies increase players shield by 25
			else {
				m_player.setShield(m_player.getShield() + 25);
				//remove that turtle from the game
				turtle_list.erase(turtle_list.begin() + i);
			}
		}

		//Jellyfish initialization
		if (jellyfish_list.empty()) {
			for (int i = 0; i < 5; i++) {
				float xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
				float zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
				float minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos));
				float ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
				m_jellyfish.initialise(build_jellyfish, glm::vec3(xpos, ypos, zpos));
				jellyfish_list.push_back(m_jellyfish);
			}
		}
		//Jellyfish updater, collisions
		else {
			for (int i = 0; i < jellyfish_list.size(); i++) {
				//If jellyfish collidees with the camera (the player), sting them
				if (jellyfish_list[i].getBox().collisionPoint(m_3d_camera.position()) && jellyfish_list[i].getStingTimer() <= glfwGetTime()) {
					m_player.setHealth(m_player.getHealth() - 5);
					jellyfish_list[i].setStingTimer(glfwGetTime() + 2);
				}
				//If the jellyfish collides with another jelly fish, turn and travel awy from them
				for (int o = 0; o < jellyfish_list.size(); o++) {
					if (i != o && jellyfish_list[i].getBox().collision(jellyfish_list[o].getBox())) {
						if (i < o) {
							jellyfish_list[i].setTarget(jellyfish_list[i].get_position() + (jellyfish_list[i].get_position() - jellyfish_list[o].get_position()));
							jellyfish_list[i].setChangeTargetTime(glfwGetTime() + 2);
							break;
						}
					}
				}
				//Updates the jelly fish
				jellyfish_list[i].on_update(time_step, m_3d_camera, m_player);
			}
		}
		//Initialize dolphin List
		if (dolphin_list.empty()) {
			//Creates 7 dolphins
			float xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			float zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			float minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 1.f);
			float ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));

			m_dolphin.initialise(build_dolphin, glm::vec3(xpos, ypos, zpos));
			dolphin_list.push_back(m_dolphin);

			xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 1.f);
			ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
			m_dolphin2.initialise(build_dolphin, glm::vec3(xpos, ypos, zpos));
			dolphin_list.push_back(m_dolphin2);

			xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 1.f);
			ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
			m_dolphin3.initialise(build_dolphin, glm::vec3(xpos, ypos, zpos));
			dolphin_list.push_back(m_dolphin3);

			xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 1.f);
			ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
			m_dolphin4.initialise(build_dolphin, glm::vec3(xpos, ypos, zpos));
			dolphin_list.push_back(m_dolphin4);

			xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 1.f);
			ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
			m_dolphin5.initialise(build_dolphin, glm::vec3(xpos, ypos, zpos));
			dolphin_list.push_back(m_dolphin5);

			xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 1.f);
			ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
			m_dolphin6.initialise(build_dolphin, glm::vec3(xpos, ypos, zpos));
			dolphin_list.push_back(m_dolphin6);

			xpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			zpos = -19 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - (-19))));
			minY = (*m_heightmap).ground_height(glm::vec3(xpos, 0, zpos) + 1.f);
			ypos = minY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (19 - minY)));
			m_dolphin7.initialise(build_dolphin, glm::vec3(xpos, ypos, zpos));
			dolphin_list.push_back(m_dolphin7);
		}
		//Dolphin Collision and Update
		else {
			//Sets player boost factor off
			m_player.setBoost(false);
			//For each dolphin
			for (int i = 0; i < dolphin_list.size(); i++) {
				//Retrieve pre-update position
				glm::vec3 pos = dolphin_list[i].get_position();
				//Update
				dolphin_list[i].on_update(time_step, m_3d_camera, m_player);
				//If dolphin is not fleeing
				if (dolphin_list[i].getMode() != 2) {
					//Check collision with another dolphin
					for (int o = 0; o < dolphin_list.size(); o++) {
						if (i != o) {
							//If outer sphere collision
							if (dolphin_list[i].getSphereSet()[1].collision(dolphin_list[o].getSphereSet()[1])) {
								//If the colliding dolphins are not in a pod, form one and set state to pod
								if (dolphin_list[i].getPod() == 0 && dolphin_list[o].getPod() == 0) {
									dolphin_list[i].setMode(1);
									dolphin_list[o].setMode(1);
									dolphin_list[i].setPod(i + 1);
									dolphin_list[o].setPod(i + 1);
								}
								//If the dolphins are not of the same pod
								else if (dolphin_list[i].getPod() != dolphin_list[o].getPod()) {
									//Dolphin o joins dolphin i's pod
									dolphin_list[o].setPod(dolphin_list[i].getPod());
								}
							}
						}
					}
				}
				//Flocking Mechanics
				//If the dolphin is in a pod
				if (dolphin_list[i].getMode() == 1) {
					//Store the pod of current dolphin
					int changingPod = dolphin_list[i].getPod();
					//How many in the pod
					int podTotal = 0;
					//Sum of pod's travel direction
					glm::vec3 podDirectionSum = glm::vec3(0, 0, 0);
					//Which dolphins in the list are of the desired pod
					std::vector<int> positions;
					//Average position of all doplhins in pod
					glm::vec3 podPosition = glm::vec3(0, 0, 0);
					for (int o = 0; o < dolphin_list.size(); o++) {
						//If the other dolphin is in the same pod as the current one
						if (dolphin_list[o].getPod() == changingPod) {
							//Add direction to poddirection
							podDirectionSum += dolphin_list[o].getTarget();
							//Increment number of dolphins in pod
							podTotal = podTotal + 1;
							//Add to sum of positions of dolphin in the pod
							podPosition = podPosition + dolphin_list[o].get_position();
							//Adds the dolphins position to the list of dolphins in the pod
							positions.push_back(o);
						}
					}
					//Calculate the average target of the pod
					glm::vec3 podTarget = podDirectionSum / (float)podTotal;
					//Retrieves the y value of the terrain at the point
					float minY = (*m_heightmap).ground_height(glm::vec3(podTarget.x, 0, podTarget.y));
					//If the target is below the terrain or above the water, adjust accordingly
					if (podTarget.y <= minY) {
						podTarget.y = minY + 0.5f;
					}
					if (podTarget.y > 19) {
						podTarget.y = 19;
					}
					//For each dolphin in the pod
					for (int o : positions) {
						//Calculate the new forward by getting the pods vecctor to target and adding it to the dolphin's current position
						glm::vec3 forward = podTarget - (podPosition / (float)podTotal);
						glm::vec3 dolphinTarget = dolphin_list[o].get_position() + forward;
						//Adjusts the dolphins target if it falls outside of the waterbox/below the terrain
						if (dolphinTarget.x < -19) {
							dolphinTarget.x = -19;
						}
						else if (dolphinTarget.x > 19) {
							dolphinTarget.x = 19;
						}
						if (dolphinTarget.z < -19) {
							dolphinTarget.z = -19;
						}
						else if (dolphinTarget.z > 19) {
							dolphinTarget.z = 19;
						}
						minY = (*m_heightmap).ground_height(glm::vec3(dolphinTarget.x, 0, dolphinTarget.y)) + 1.f;
						if (dolphinTarget.y < minY) {
							dolphinTarget.y = minY;
						}
						else if (dolphinTarget.y > 19) {
							dolphinTarget.y = 19;
						}
						//Sets the dolphins target
						dolphin_list[o].setTarget(dolphin_list[o].get_position() + forward);
						dolphin_list[o].setChangeTargetTime(glfwGetTime() + (rand() % 10 + 5));
					}
					//Flocking attract and reple
					for (int o = 0; o < dolphin_list.size(); o++) {
						if (i != o) {
							//If the dolphins inner spheres are not colliding
							if (!dolphin_list[i].getSphereSet()[0].collision(dolphin_list[o].getSphereSet()[0])) {
								//Adjust the forward of the dolphin to be towards the centre of the pod
								glm::vec3 newforward = ((podPosition / (float)podTotal) - dolphin_list[i].get_position());
								dolphin_list[i].object()->set_forward(dolphin_list[i].object()->forward() + newforward);
								//If the dolphin is in front of the colliding dolphin speed up
								if (length(podTarget - (podPosition / (float)podTotal)) < length(podTarget - dolphin_list[i].get_position())) {
									dolphin_list[i].setSpeed(2.2f);
								}
								//If they are behind, slow down
								else if (length(podTarget - (podPosition / (float)podTotal)) > length(podTarget - dolphin_list[i].get_position())) {
									dolphin_list[i].setSpeed(1.9f);
								}
							}
							//If the dolphins inner spheres are colliding
							//Adjust the forward to being away from the colliding dolphin
							else if (dolphin_list[i].getSphereSet()[0].collision(dolphin_list[o].getSphereSet()[0])) {
								glm::vec3 newforward = (dolphin_list[i].get_position() - dolphin_list[o].get_position());
								dolphin_list[i].object()->set_forward(dolphin_list[i].object()->forward() - normalize(newforward));
								if (length(podTarget - dolphin_list[o].get_position()) < length(podTarget - dolphin_list[i].get_position())) {
									dolphin_list[i].setSpeed(1.8f);
									dolphin_list[i].back(pos);
								}
								//If the dolphin is in front of the colliding dolphin speed up
								else if (length(podTarget - dolphin_list[o].get_position()) > length(podTarget - dolphin_list[i].get_position())) {
									dolphin_list[i].setSpeed(2.2f);
								}
								//If they are behind, slow down
								else {
									dolphin_list[i].setSpeed(2.f);
								}
							}
						}
					}
				}
				//If colliding with shark's collision box
				for (int o = 0; o < shark_list.size(); o++) {
					if (dolphin_list[i].getSphereSet()[1].collisionBox(shark_list[o].getBox())) {
						//Flee
						dolphin_list[i].setMode(3);
					}
				}
				//If colliding with the player
				if (dolphin_list[i].getSphereSet()[1].collisionPoint(m_3d_camera.position())) {
					//Set the player boost to true
					m_player.setBoost(true);
				}

			}
			//If overall player boost is true, increase the movcement speed
			if (m_player.getBoost()) {
				m_3d_camera.set_movement_speed(2.2f);
			}
			//If overall playerboost is false, adjust speed back to normal
			else {
				m_3d_camera.set_movement_speed(1.0f);
			}

		}
		//Code sampled from lecture 5
		//Gun position updateer
		glm::vec3 frontVector = m_3d_camera.front_vector();
		glm::vec3 forward = m_3d_camera.front_vector();
		glm::vec3 right = m_3d_camera.right_vector();
		glm::vec3 up = m_3d_camera.up_vector();

		float theta = engine::PI / 2.0f - acos(forward.y);
		float phi = engine::PI - atan2(forward.x, forward.z);
		glm::vec3 pos = m_3d_camera.position();
		glm::vec3 p = pos + 0.3f * forward + 0.10f * right - 0.05f * up;
		m_gun->set_position(p);
		//Code no longer sampled

		//Places the torch on the gun and rotates the direction to the direction the player is facing
		m_torch.Position = m_gun->position();
		m_torch.Direction = normalize(m_3d_camera.front_vector());

		//If player loses health during update play crossfade
		//Crossfade code taken from tutorial FX file
		if (m_player.getHealth() < playerHealth) {
			m_cross_fade->activate();
			//If the player's health is 0 or below, show death menu and play menu music
			if (m_player.getHealth() <= 0) {
				deathMenuOn = true;
				m_audio_manager->pause("music");
				m_audio_manager->play("menuMusic");
			}
		}
	}
	m_3d_camera.on_update(time_step, *m_heightmap);

} 

void example_layer::on_render() 
{
		engine::render_command::clear_color({ 0.0f, 0.0f, 0.0f, 1.0f });
		engine::render_command::clear();

		// Set up  shader. (renders textures and materials)
		const auto mesh_shader = engine::renderer::shaders_library()->get("mesh");
		//If not in a menu, render the game level
	if (!menuOn && !deathMenuOn) {
		engine::renderer::begin_scene(m_3d_camera, mesh_shader);
		// Set up some of the scene's parameters in the shader
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

		//Removes fog when above the water's surface
		if (m_3d_camera.position().y > 20.24f) {
			m_fog = false;
		}
		else {
			m_fog = true;
		}
		//Applies fog
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("fog_on", m_fog);

		//Sets up point lighting
		num_point_lights = pointLight_list.size();
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("gNumPointLights", (int)num_point_lights);
		for (int i = 0; i < num_point_lights; i++) {
			pointLight_list[i].submit(mesh_shader, i);
		}

		m_material->submit(mesh_shader);
		//Sets up spotlights
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->
			set_uniform("gNumSpotLights", (int)num_spot_lights);
		m_spotLight.submit(mesh_shader, 0);
		m_torch.submit(mesh_shader, 1);

		// Position the skybox centred on the player and render it
		glm::mat4 skybox_transform(1.0f);
		skybox_transform = glm::translate(skybox_transform, m_3d_camera.position());
		for (const auto& texture : m_skybox->textures())
		{
			texture->bind();
		}

		//Positons waterbox around centre of map area
		glm::mat4 waterbox_transform(1.0f);
		waterbox_transform = glm::translate(waterbox_transform, glm::vec3(0.f, 0.f, 0.f));
		for (const auto& texture : m_waterbox->textures())
		{
			texture->bind();
		}

		//Sky and Waterbox Renders
		engine::renderer::submit(mesh_shader, m_skybox, skybox_transform);
		engine::renderer::submit(mesh_shader, m_waterbox, waterbox_transform);

		//Terrain Render
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_material->submit(mesh_shader);
		engine::renderer::submit(mesh_shader, m_terrain);
		m_surface_material->submit(mesh_shader);
		engine::renderer::submit(mesh_shader, m_surface);


		//Searches list of pick-up items, if it is active it is rendered
		//Will optimize to remove non-active items from list
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_material->submit(mesh_shader);
		for (pickup item : pickup_list) {
			if (item.active()) {
				if (item.getType() == "oxygen") {
					std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
					m_bubble_material->submit(mesh_shader);
				}
				item.on_render(mesh_shader);
				std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
				m_material->submit(mesh_shader);
			}
		}

		//Renders and transforms the gun
		//Rotates the gun to the direction the camera is facing
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_gun->textures().at(0)->bind();
		float PI = 3.14159265358979323846;
		glm::mat4 gun_transform(1.0f);
		//Taken from Lec 5
		glm::vec3 frontVector = m_3d_camera.front_vector();
		glm::vec3 forward = m_3d_camera.front_vector();
		glm::vec3 right = m_3d_camera.right_vector();
		glm::vec3 up = m_3d_camera.up_vector();

		float theta = engine::PI / 2.0f - acos(forward.y);
		float phi = engine::PI - atan2(forward.x, forward.z);
		glm::vec3 pos = m_3d_camera.position();
		glm::vec3 p = pos + 0.3f * forward + 0.09f * right - 0.05f * up;
		gun_transform = glm::translate(gun_transform, m_gun->position());
		//Transforms the gun
		gun_transform = glm::rotate(gun_transform, -phi, glm::vec3(0.f, 1.f, 0.f));
		gun_transform = glm::rotate(gun_transform, theta, glm::vec3(1.f, 0.f, 0.f));
		gun_transform = glm::scale(gun_transform, m_gun->scale());

		engine::renderer::submit(mesh_shader, gun_transform, m_gun);
		m_gun->set_position(p);

		//If the shark list is not empty, render the sharks
		if (!shark_list.empty())
		{
			//Renders all sharks in the shark list and fits the shark material to it
			std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
			m_shark_material->submit(mesh_shader);
			for (int i = 0; i < shark_list.size(); i++) {
				shark_list[i].on_render(mesh_shader, m_3d_camera);
			}
		}
		//If the turtle list is not empty, render the turtles
		if (!turtle_list.empty()) {
			std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
			for (int i = 0; i < turtle_list.size(); i++) {
				turtle_list[i].on_render(mesh_shader, m_3d_camera);
			}
		}

		//If the dolphin list is not empty, render the dolphins
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
		for (int i = 0; i < dolphin_list.size(); i++) {
			dolphin_list[i].on_render(mesh_shader, m_3d_camera);
		}

		//Renders emitters if emitter list is not empty
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		for (int i = 0; i < emiiter_list.size(); i++) {
			emiiter_list[i].on_render(mesh_shader);
		}

		//Renders all bullets in the ballistic list and fits the ballistic material to it
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);
		m_ballistic_material->submit(mesh_shader);
		for (int i = 0; i < ballistic_list.size(); i++) {
			ballistic_list[i].on_render(mesh_shader);		}

		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", false);

		//Renders all jellyfish
		for (int i = 0; i < jellyfish_list.size(); i++) {
			m_jelly_material->submit(mesh_shader);
			jellyfish_list[i].on_render(mesh_shader, m_3d_camera);
		}

		//Renders sunken ship
		m_boat->textures().at(0)->bind();
		engine::renderer::submit(mesh_shader, m_boat);

		//Renders Ship's lamp
		m_material->submit(mesh_shader);
		engine::renderer::submit(mesh_shader, m_lamp);

		//Renders rock prism
		std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("has_texture", true);
		m_material->submit(mesh_shader);
		engine::renderer::submit(mesh_shader, m_prism);
		
		engine::renderer::end_scene();
		// Render text of Player's information as well as current wave and a + shaped cross hair
		const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
		m_text_manager->render_text(text_shader, "Health: " + std::to_string(m_player.getHealth()), 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 1.5f, 1.f, 0.9f));
		m_text_manager->render_text(text_shader, "Shield: " + std::to_string(m_player.getShield()), 10.f, (float)engine::application::window().height() - 51.f, 0.5f, glm::vec4(1.f, 1.5f, 1.f, 0.9f));
		m_text_manager->render_text(text_shader, "Ammo: " + std::to_string(m_player.getAmmoCount()), 10.f, (float)engine::application::window().height() - 76.f, 0.5f, glm::vec4(1.f, 1.5f, 1.f, 0.9f));
		m_text_manager->render_text(text_shader, "Oxygen: " + std::to_string(m_player.getOxygen()), 10.f, (float)engine::application::window().height() - 100.f, 0.5f, glm::vec4(1.f, 1.5f, 1.f, 0.9f));
		m_text_manager->render_text(text_shader, std::to_string(current_wave), 10.f, 5.f, 1.f, glm::vec4(1.f, 1.5f, 1.f, 0.9f));
		m_text_manager->render_text(text_shader, "+", (float)engine::application::window().width() / 2, ((float)engine::application::window().height() / 2) - 4.f, 0.5f, glm::vec4(1.f, 1.5f, 1.f, 0.6f));

		engine::renderer::begin_scene(m_2d_camera, mesh_shader);
		m_cross_fade->on_render(mesh_shader);
		engine::renderer::end_scene();
	}
	else {
		//If the main menu is on render the main menu
		if (menuOn) {
			engine::renderer::begin_scene(m_2d_camera, mesh_shader);
			m_menu->on_render(mesh_shader);
			engine::renderer::end_scene();
		}
		//If the death menu is on render the death menu
		else if (deathMenuOn) {
			engine::renderer::begin_scene(m_2d_camera, mesh_shader);
			death_menu->on_render(mesh_shader);
			engine::renderer::end_scene();
		}
	}
}


void example_layer::on_event(engine::event& event) 
{
	//If in game
	if (!menuOn && !deathMenuOn) {
		//Check key event
		if (event.event_type() == engine::event_type_e::key_pressed)
		{
			auto& e = dynamic_cast<engine::key_pressed_event&>(event);
			if (e.key_code() == engine::key_codes::KEY_TAB)
			{
				engine::render_command::toggle_wireframe();
			}
			//Used to test wave spawn mechanics, cheat mode progresses through waves
			if (e.key_code() == engine::key_codes::KEY_P)
			{
				shark_list.clear();
			}
		}

		//On mouse click, create bullet and add to ballistic list if the play has ammo and is not reloading
		if (event.event_type() == engine::event_type_e::mouse_button_pressed) {
			auto& e = dynamic_cast<engine::mouse_button_event&>(event);
			//When Mouse Button is pressed, initialize a new projectile, fires it and adds it to the ballistic list
			if (e.mouse_button() == 0 && m_player.getAmmoCount() > 0) {
				if (glfwGetTime() >= m_player.getShotCooldown()){
					m_ballistic.initialise(build_proj);
						glm::vec3 pos = m_gun->position();
						glm::vec3 forward = m_3d_camera.front_vector();
						glm::vec3 right = m_3d_camera.right_vector();
						glm::vec3 up = m_3d_camera.up_vector();

						glm::vec3 p = pos - 0.9f * forward + 0.013f * up;
						m_ballistic.fire(m_3d_camera, 5.f, p);
						m_audio_manager->play_spatialised_sound("shot", m_3d_camera.position(), p);
						ballistic_list.push_back(m_ballistic);
						m_player.setAmmoCount(m_player.getAmmoCount() - 1);
						m_player.setShotCooldown();
				}
			}

		}
	}
	else {
		if (event.event_type() == engine::event_type_e::key_pressed) {
			//If in main menu, pause menu music and play game music, turns off menu
			if (menuOn) {
				menuOn = false;
				m_audio_manager->pause("menuMusic");
				m_audio_manager->unpause("music");
			}
			//If in death menu, resets the level to wave 1 and resets all entities to their initial values and states
			//Plays back-ground music and turns off menu music
			else {
				current_wave = 0;
				shark_list.clear();
				turtle_list.clear();
				jellyfish_list.clear();
				dolphin_list.clear();
				pickup_list.clear();
				for (int i = 0; i < pickup_list.size(); i++) {
					if (pickup_list[i].getType() != "oxygen") {
						pickup_list[i].setActive();
					}
					else {
						pickup_list.erase(pickup_list.begin() + i);
						i -= 1;
					}
				}
				ballistic_list.clear();
				m_3d_camera.reset();
				m_player.reset();
				deathMenuOn = false;
				m_audio_manager->play("music");
				m_audio_manager->unpause("music");
			}
		}
	
		
	}


}
