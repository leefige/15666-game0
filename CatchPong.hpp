/*
 * Description: 
 * Created Date: 2019-08-28
 * Author: Yifei Li - yifeili3
 * -----
 * Last Modified: 2019-09-02
 * Modified By: Yifei Li
 * -----
 * Based on CMU 15-666 game0 code
 * Copyright (c) 2019
 */

#include "ColorTextureProgram.hpp"

#include "Mode.hpp"
#include "GL.hpp"

#include <vector>
#include <deque>

/*
 * CatchPong is a game mode that implements a single-player game of Pong.
 */

struct CatchPong : Mode {
	CatchPong();
	virtual ~CatchPong();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	struct Ball {
		glm::vec2 ball;
		glm::vec2 ball_velocity;

		Ball () : ball(.0f, .0f), ball_velocity(1.0f, 1.0f) {}
		Ball (float x, float y) : ball(x, y), ball_velocity(1.0f, 1.0f) {}
		Ball (float x, float y, float vx, float vy) : ball(x, y), ball_velocity(vx, vy) {}

		Ball (glm::vec2 ball_) : ball(ball_), ball_velocity(1.0f, 1.0f) {}
		Ball (glm::vec2 ball_, glm::vec2 vel_) : ball(ball_), ball_velocity(vel_) {}
		Ball (glm::vec2 ball_, float vx, float vy) : ball(ball_), ball_velocity(vx, vy) {}
		
		virtual ~Ball() {}
	};

	//----- game state -----

	glm::vec2 court_radius = glm::vec2(7.0f, 5.0f);
	glm::vec2 paddle_radius = glm::vec2(0.2f, 1.0f);
	glm::vec2 ball_radius = glm::vec2(0.2f, 0.2f);

	glm::vec2 left_paddle = glm::vec2(-court_radius.x + 0.5f, 0.0f);
	glm::vec2 right_paddle = glm::vec2( court_radius.x - 0.5f, 0.0f);

	std::vector <Ball> balls;

	uint32_t left_score = 0;
	uint32_t life_score = 3;
	uint32_t hit_times = 1;

	static uint32_t SPLIT_TIMES;

	float ai_offset = 0.0f;
	float ai_offset_update = 0.0f;

	//----- pretty rainbow trails -----

	float trail_length = 1.3f;
	std::deque< glm::vec3 > ball_trail; //stores (x,y,age), oldest elements first

	//----- opengl assets / helpers ------

	//draw functions will work on vectors of vertices, defined as follows:
	struct Vertex {
		Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
			Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
		glm::vec3 Position;
		glm::u8vec4 Color;
		glm::vec2 TexCoord;
	};
	static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "CatchPong::Vertex should be packed");

	//Shader program that draws transformed, vertices tinted with vertex colors:
	ColorTextureProgram color_texture_program;

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;

	//Solid white texture:
	GLuint white_tex = 0;

	//matrix that maps from clip coordinates to court-space coordinates:
	glm::mat3x2 clip_to_court = glm::mat3x2(1.0f);
	// computed in draw() as the inverse of OBJECT_TO_CLIP
	// (stored here so that the mouse handling code can use it to position the paddle)

};