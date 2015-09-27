#include "MainGame.h"
#include "Text.h"
#include "GPUProfiler.h"
#include "CPUProfiler.h"
#include <string>
#include <GLM/gtx/transform.hpp>
#include "SphereCollider.h"
#include "AABBCollider.h"
#include "GJK.h"




namespace Ryno{

	void MainGame::start(){
		CPUProfiler::begin();
		CPUProfiler::start_time();

		//initializations
		m_camera->position = glm::vec4(0,150,-500, 1);
		

		CPUProfiler::next_time();

		//loading graphics
		white = m_texture_manager->loadPNG("white_pixel",GAME_FOLDER);
		Texture white_normal = m_texture_manager->loadPNG("normal_pixel", GAME_FOLDER);
		Texture texture_bricks = m_texture_manager->loadPNG("pack/177", GAME_FOLDER);
		Texture normal_map_bricks = m_texture_manager->loadPNG("pack/177_norm", GAME_FOLDER);
		Texture at = m_texture_manager->loadPNG("pack/196", GAME_FOLDER);
		Texture an = m_texture_manager->loadPNG("pack/196_norm", GAME_FOLDER);
		Texture bt = m_texture_manager->loadPNG("pack/161", GAME_FOLDER);
		Texture bn = m_texture_manager->loadPNG("pack/161_norm", GAME_FOLDER);
		Texture solaire = m_texture_manager->loadPNG("solaire", GAME_FOLDER);
		Texture sun = m_texture_manager->loadPNG("sun", GAME_FOLDER);
		Texture doge = m_texture_manager->loadPNG("doge", GAME_FOLDER);

		CPUProfiler::next_time();

		//loading models
		I32 cube_mesh = m_mesh_manager->load_mesh("cube", 1, GAME_FOLDER);
		I32 sphere_mesh = m_mesh_manager->load_mesh("sphere", 1, GAME_FOLDER);
		I32 cone_mesh = m_mesh_manager->load_mesh("cone", 1, GAME_FOLDER);
		marker_mesh = m_mesh_manager->load_mesh("marker", 1, GAME_FOLDER);

		CPUProfiler::next_time();

		//loading skyboxes
		m_camera->skybox = m_texture_manager->loadCubeMap("full_moon_small", GAME_FOLDER);

		CPUProfiler::next_time();

		//loading audio
		sound = m_audio_manager->load_sound("stomp.wav", GAME_FOLDER);
		music = m_audio_manager->load_music("journey.ogg", GAME_FOLDER);
		sound.set_volume(0.0f);
		music.set_volume(0.0f);
		music.play();
	

		CPUProfiler::next_time();

		//Build the environnement

		//Cones
		GameObject* go = new GameObject();
		go->model = new Model();
		go->model->set_color_and_flatness(255, 255, 255, 0);
		go->model->set_texture_normal(white, white_normal);
		go->model->mesh = cone_mesh;
		go->transform->set_scale(100,100,100);
		go->transform->set_position(0, 55, 50);
		
	
		go = new GameObject(go);
		go->transform->set_scale(20, 20, 20);
		go->transform->set_position(-150, 15, -170);

		go = new GameObject(go);
		go->transform->set_position(150, 15, -170);


		//Base
		go = new GameObject(go);
		go->transform->set_scale(400, 10, 400);
		go->model->set_texture_normal(bt, bn);

		go->model->mesh = cube_mesh;
		go->model->set_tiling(3, 3);
		go->transform->set_position(0, 0, 0);
		//Left
		go = new GameObject(go);
		go->transform->set_scale(10, 200, 400);
		go->transform->set_position(-200, 105, 0);
		//Right
		go = new GameObject(go);
		go->transform->set_scale(10, 200, 400);
		go->transform->set_position(200, 105, 0);
		//Front
		go = new GameObject(go);
		go->transform->set_scale(400, 200, 10);
		go->transform->set_position(0, 105, 200);
		//Roof
		go = new GameObject(go);
		go->transform->set_scale(400, 10, 400);
		go->transform->set_position(0, 205, 0);
	
		SpotLight* s = new SpotLight();
		s->set_direction(-90, 0);
		s->cutoff = 30;
		s->set_diffuse_color(0, 255, 0);
		s->diffuse_intensity = 30;
		s->attenuation = .001;
		s->specular_intensity = 10;
		s->set_specular_color(0, 255, 0);

		GameObject* spot_light = new GameObject();
		spot_light->transform->set_position(0, 190, 50);
		spot_light->spot_light = s;
		spheres[4] = spot_light;

		PointLight* p = new PointLight();
		p->set_diffuse_color(255, 80, 0);
		p->diffuse_intensity = 3;
		p->attenuation = .001;
		p->specular_intensity = 10;
		p->set_specular_color(255, 80, 0);

		go = new GameObject(go);
		go->model->set_color_and_flatness(255, 255, 255, 255);
		go->model->cast_shadows = false;
		go->model->set_texture_normal(white, white_normal);
		go->transform->set_scale(12, 12, 12);
		go->transform->set_position(180, 20, 180);
		go->model->mesh = sphere_mesh;
		go->point_light = p;
		spheres[0] = go;


		go = new GameObject(go);
		go->transform->set_position(-180, 20, 180);
		go->point_light = new PointLight(p);
		spheres[1] = go;


		go = new GameObject(go);
		go->transform->set_position(-180, 20, -180);
		go->point_light = new PointLight(p);
		spheres[2] = go;

	
		go = new GameObject(go);
		go->transform->set_position(180, 20, -180);
		go->point_light = new PointLight(p);
		spheres[3] = go;



		//PHYSICS
		ball = new GameObject(go);
		ball->transform->set_scale(30,30,30);
		ball->point_light->set_diffuse_color(0, 100, 255);
		ball->point_light->set_specular_color(0, 100, 255);
		ball->point_light->attenuation = 0.01;
		ball->point_light->specular_intensity = 100.0f;
		ball->point_light->diffuse_intensity = 40.0f;
		ball->model->texture = white;
		


		CPUProfiler::next_time();

		//build light

		GameObject* bl;
	
		
	

		
		DirectionalLight* l = new DirectionalLight();
		l->set_direction(-65, 150);
		l->diffuse_intensity = 0.2;
		l->set_diffuse_color(255, 255, 200);
		l->specular_intensity = .05;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = .05;
		l->set_ambient_color(255, 255, 200);
		GameObject* dir_light = new GameObject();
		dir_light->dir_light = l;
	

		
		/*Sprite* sp = new Sprite();
		sp->depth = 20;
		sp->angle = 0;
		sp->set_color(255, 255, 255, 255);
		sp->set_texture(doge);
		sp->set_position(0.25,0.75);
		sp->anchor_point = CENTER;
		sp->set_scale(150, 150);
		sp->set_tiling(1, 1);
		sp->angle = 0;


		Font* f = new Font("Aaargh",24, GAME_FOLDER);
		Text* t = new Text();
		t->depth = 10;
		t->font = f;
		t->set_position(1,1);
		t->anchor_point = TOP_RIGHT;
		t->set_scale(1,1);
		t->set_color(255,255,0, 255);
		t->text = "Ryno Engine";*/

		
		
		CPUProfiler::end_time();
		CPUProfiler::print_time();


	


		go_a = new GameObject(ball);
		go_a->model->set_color_and_flatness(255, 255, 255, 0);
		go_a->model->texture = texture_bricks;
		go_a->model->normal_map = normal_map_bricks;
		go_a->model->cast_shadows = true;
		go_a->transform->set_scale(50, 50, 50);
		go_a->model->mesh = cube_mesh;
		delete go_a->point_light;
		go_a->point_light = nullptr;
		go_a->transform->position = glm::vec3(-155, 223, -100);
		go_a->collider = new AABBCollider();
		body = go_a;

		go_b = new GameObject(go_a);
		go_b->model->cast_shadows = false;
		go_b->model->set_color_and_flatness(255, 255, 255, 255);
		go_b->model->texture = white;
		go_b->model->normal_map = white_normal;
		go_b->point_light = new PointLight(p);
		go_b->collider = new SphereCollider(0, 0, 0, 1);
		go_b->model->mesh = sphere_mesh;
		go_b->transform->position = glm::vec3(10,10, -100);
		go_b->transform->set_scale(20, 20, 20);

		go_c = new GameObject(go_b);
		go_c->transform->set_scale(10, 10, 10);
		go_c->collider = new SphereCollider(0, 0, 0, 1);
		go_c->model->mesh = sphere_mesh;
		go_d = new GameObject(go_c);
		go_d->transform->set_scale(30, 30,30);
		go_d->model->mesh = cube_mesh;
		go_d->collider = new AABBCollider();
		go_e = new GameObject(go_d);
		go_e->transform->set_scale(50, 50, 50);



		set_physics();
		

		
		
	}
	
	

	void MainGame::update(){

		if (GJK::gjk(go_a, go_b)) go_b->point_light->active = true; else go_b->point_light->active = false;
		if (GJK::gjk(go_a, go_c)) go_c->point_light->active = true; else go_c->point_light->active = false;
		if (GJK::gjk(go_a, go_d)) go_d->point_light->active = true; else go_d->point_light->active = false;
		if (GJK::gjk(go_a, go_e)) go_e->point_light->active = true; else go_e->point_light->active = false;

		


		for (I32 i = 0; i < 4; i++){
			spheres[i]->transform->position.y = 40 + sin(m_time_manager->current_time/1000.0f) * 20;
		}

		static I32 cnt = 0;

		if (shell->restart_physics){
			shell->restart_physics = false;
			set_physics();
		}
		//if (!shell->active || shell->phys_step){
		//	glm::vec3 force = glm::vec3(0, 0, 0);
		//	F32 time_step = delta_time * 0.001; //from ms to s


		//	glm::vec3 F0 = Fw.get_force();
		//	glm::vec3 F1 = Fd.get_force();





		//	force = F0 + F1;

		//	glm::vec3 old_a = acceleration;
		//	ball->transform->add_position(100.0f * (velocity * time_step + (0.5f * old_a * time_step * time_step)));
		//	acceleration = force / Fw.m;




		//	glm::vec3 avg_a = 0.5f * (acceleration + old_a);

		//	velocity += avg_a * time_step;
		//	//Log::print(" avga: "); //Log::print(avg_a.x);
		//	//Log::print(" v: "); //Log::print(velocity.x);


		//	Fd.v = glm::length(velocity);
		//	Fd.dir = glm::normalize(velocity);

		//	//Collision
		//	if (ball->transform->position.y - ball->transform->scale.y / 2.0f < 5 && velocity.y < 0)
		//	{
		//		/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
		//		velocity.y *= -0.95;
		//		/* Move the ball back a little bit so it's not still "stuck" in the wall. */
		//		ball->transform->position.y = 5 + ball->transform->scale.y / 2.0f;
		//	}
		//	else if (ball->transform->position.y + ball->transform->scale.y / 2.0f >200 && velocity.y > 0)
		//	{
		//		/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
		//		velocity.y *= -0.95;
		//		/* Move the ball back a little bit so it's not still "stuck" in the wall. */
		//		ball->transform->position.y = 200 - ball->transform->scale.y / 2.0f;
		//	}
		//	if (ball->transform->position.x + ball->transform->scale.x / 2.0f > 195 && velocity.x > 0)
		//	{
		//		/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
		//		velocity.x *= -0.95;
		//		/* Move the ball back a little bit so it's not still "stuck" in the wall. */
		//		ball->transform->position.x = 195 - ball->transform->scale.x / 2.0f;
		//	}
		//	else if (ball->transform->position.x - ball->transform->scale.x / 2.0f < -195 && velocity.x < 0)
		//	{
		//		/* This is a simplification of impulse-momentum collision response. e should be a negative number, which will change the velocity's direction. */
		//		velocity.x *= -0.95;
		//		/* Move the ball back a little bit so it's not still "stuck" in the wall. */
		//		ball->transform->position.x = -195 + ball->transform->scale.x / 2.0f;
		//	}


		//	shell->phys_step = false;


		//	if (++cnt > 0){
		//		cnt = 0;
		//		for (GameObject* o : markers){
		//			if (o->model->color.g > 0)o->model->color.g-=2;
		//		}
		//		GameObject* temp = markers.back();
		//		if (markers.back()->model->color.g == 0){
		//			temp->transform->position = ball->transform->position;
		//			temp->model->set_color(254, 254, 0);
		//			markers.pop_back();
		//			markers.push_front(temp);
		//		}
		//		else
		//		{
		//			GameObject* go = new GameObject(temp);
		//			go->transform->position = ball->transform->position;
		//			go->model->set_color(254, 254, 0);
		//			markers.push_front(go);
		//		}

		//	}
		//}

		


	}

	
	void MainGame::set_physics()
	{

		ball->transform->set_position(0, 50, -50);
		velocity = glm::vec3(3,5, 0);

		for (GameObject* go : markers)
			delete go;
		markers.clear();

		Fw.a = glm::vec3(0, -9.8, 0);
		Fw.m = 200;


		Fd.A = M_PI * ball->transform->scale.x * ball->transform->scale.x / 10000.0f;
		Fd.Cd = 0.47f;
		Fd.p = 1.2f;
	
	
		marker = new GameObject();
		marker->transform->set_scale(2,2,2);
		marker->model = new Model();
		marker->model->mesh = marker_mesh;
		marker->model->texture = white;
		marker->transform->position = ball->transform->position;
		marker->model->set_color_and_flatness(254, 254, 0, 255);
		marker->model->cast_shadows = false;
		markers.push_front(marker);
	}

	void MainGame::input(){
		if (!shell->active){
			if (m_input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				sound.play();

			}
			if (m_input_manager->is_key_pressed(SDLK_v, KEYBOARD)){
				if (body == go_a)
					body = go_b;
				else if (body == go_b)
					body = go_c;
				else if (body == go_c)
					body = go_d;
				else if (body == go_d)
					body = go_e;
				else body = go_a;
			}
		




			if (m_input_manager->is_key_down(SDLK_LEFT, KEYBOARD)){
				body->transform->position.x -= 0.1f * delta_time;
			}
			if (m_input_manager->is_key_down(SDLK_RIGHT, KEYBOARD)){
				
					body->transform->position.x += 0.1f * delta_time;
			}
	

			if (m_input_manager->is_key_down(SDLK_n, KEYBOARD)){
				
					body->transform->position.z -= 0.1f * delta_time;
						

			}
			if (m_input_manager->is_key_down(SDLK_m, KEYBOARD)){
				
					body->transform->position.z += 0.1f * delta_time;
								
			}
			if (m_input_manager->is_key_down(SDLK_UP, KEYBOARD)){
				
					body->transform->position.y += 0.1f * delta_time;
			


			}
			if (m_input_manager->is_key_down(SDLK_DOWN, KEYBOARD)){
			
					body->transform->position.y -= 0.1f * delta_time;
			
			}
		
		}
		

	}
	
}