// Rachel Crawford 2016
// This file lives at https://github.com/rachelnertia/Box2D-Raycasting-Test

#include <iostream>
#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"
#include "debug_drawer.h"

struct Camera {
	b2Vec2 pos;
	b2Vec2 fwd;
};

class RayCastCallback : public b2RayCastCallback {
public:

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction)
	{
		m_fixture = fixture;
		m_point = point;
		m_normal = normal;
		m_fraction = fraction;
		return fraction;
	}

	b2Fixture* m_fixture = nullptr;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	float32 m_fraction;
};

b2Vec2 RotateVec(const b2Vec2& vec, const float angle) {
	return b2Vec2(
		vec.x*cosf(angle) - vec.y*sinf(angle),
		vec.x*sinf(angle) + vec.y*cosf(angle));
}

float DotProduct(const b2Vec2& a, const b2Vec2& b) {
	return a.x * b.x + a.y * b.y;
}

// Get the projection of this vector on other.
b2Vec2 ProjectVec(const b2Vec2& vec, const b2Vec2& other) {
	b2Vec2 other_dir = other;
	other_dir.Normalize();
	float scalar = DotProduct(vec, other_dir);
	return scalar * other_dir;
}

bool distance_mode_toggle = true;
bool raydir_mode_toggle = true;
float angle_modifier = 1.0f;

void RaycastRender(b2World& world, sf::RenderTarget& target, Camera& camera) 
{
	const float ray_length = 15.0f; // How far rays will travel before they will stop.
	const float view_angle = (3.14f * (0.25f * angle_modifier)); 
	const b2Vec2 view_plane(-camera.fwd.y * angle_modifier, camera.fwd.x * angle_modifier);	
	const b2Vec2 raystart = camera.pos;

	// Cast a ray for each horizontal pixel.
	for (unsigned i = 0; i < target.getSize().x; ++i) {
		// Determine the direction the ray should go in...
		// [-1, 1] How far across the screen from left to right the current ray is.
		float screenx = -1.0f + (2.0f * (i / (float)target.getSize().x)); 
		// There are 2 ways to calculate the ray's direction.
		b2Vec2 raydir = raydir_mode_toggle ?
			// 1: Scale the view plane vector by screenx and add it to the camera's forward vector.
			(camera.fwd) + (screenx * view_plane) :
			// 2: Rotate the camera's forward vector by the viewing angle scaled by screenx. 
			RotateVec(camera.fwd, view_angle * screenx);
		// Determine the end point of the ray in world space.
		b2Vec2 rayend = camera.pos + ray_length * raydir;

		RayCastCallback callback;
		world.RayCast(&callback, raystart, rayend); // Cast the ray!

		if (callback.m_fixture) { // If the ray hit something...
			b2Vec2 ray = (callback.m_point - raystart);
			// Use either the 1) actual distance or 2) perpendicular distance from the camera to the
			// ray hit point.
			float distance = distance_mode_toggle ? ray.Length() : DotProduct(ray, camera.fwd);
			// Use this distance to figure out how tall a line to draw.
			int line_height = abs(int(target.getSize().y / distance));
			// Make far-away lines darker.
			sf::Uint8 f = sf::Uint8((1.f - (distance / ray_length)) * 255.0f);
			sf::Color c(f, f, f);

			sf::Vertex line[2] =
			{
				sf::Vertex(sf::Vector2f(float(i+1), (float)(target.getSize().y / 2) - (line_height / 2)), c),
				sf::Vertex(sf::Vector2f(float(i+1), (float)(target.getSize().y / 2) + (line_height / 2)), c)
			};

			target.draw(line, 2, sf::PrimitiveType::Lines);
		}
	}

}

// Adds a static box body to the given Box2D world.
void AddStaticBox(b2World& world, b2Vec2 centre, b2Vec2 dimensions) {
	b2BodyDef bdef;
	bdef.type = b2_staticBody;
	bdef.position = centre;
	b2Body* body = world.CreateBody(&bdef);
	b2PolygonShape box;
	box.SetAsBox(dimensions.x, dimensions.y);
	b2FixtureDef fixture_def;
	fixture_def.shape = &box;
	body->CreateFixture(&fixture_def);
}

int main() {

	sf::RenderWindow window;
	window.create(sf::VideoMode(512, 512), "SFML Window");

	b2Vec2 gravity(0.0f, -1.0f);

	b2World world(gravity);

	AddStaticBox(world, b2Vec2(2.5f, 2.5f), b2Vec2(1.0f, 1.0f));
	AddStaticBox(world, b2Vec2(7.0f, 2.5f), b2Vec2(1.0f, 1.0f));
	AddStaticBox(world, b2Vec2(7.0f, 7.0f), b2Vec2(1.0f, 1.0f));
	AddStaticBox(world, b2Vec2(2.5f, 7.0f), b2Vec2(1.0f, 1.0f));

	// Add a dynamic circle body.
	{
		b2BodyDef body_def;
		body_def.type = b2_dynamicBody;
		body_def.position.Set(2.5f, 5.5f);
		b2Body* body = world.CreateBody(&body_def);
		b2CircleShape circle;
		circle.m_radius = 0.5f;
		b2FixtureDef fixdef;
		fixdef.shape = &circle;
		fixdef.density = 1.0f;
		fixdef.friction = 0.3f;
		body->CreateFixture(&fixdef);
	}

	DebugDrawerSFML debug_drawer;
	debug_drawer.m_target = &window;
	debug_drawer.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
	world.SetDebugDraw(&debug_drawer);
	

	sf::Clock clock;
	float dt = 0.0f;

	bool render_box2d_debug = false;

	Camera camera;
	camera.pos.x = 10.0f;
	camera.pos.y = 5.0f;
	camera.fwd.x = -1.0f;
	camera.fwd.y = 0.0f;

	unsigned frame_tex_width = 512;
	unsigned frame_tex_height = frame_tex_width;

	sf::RenderTexture frame_tex;
	frame_tex.create(frame_tex_width, frame_tex_height);

	while (window.isOpen()) {

		{
			sf::Event event;
			while (window.pollEvent(event)) {
				switch (event.type) {

				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::Resized:
					//window.setSize(sf::Vector2u(event.size.height, event.size.height));
					break;

				case sf::Event::KeyPressed:
					switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
						break;
					case sf::Keyboard::Q:
						// Toggle the Box2D debug draw mode.
						render_box2d_debug = !render_box2d_debug;
						break;
					case sf::Keyboard::E:
						// Toggle between ways of calculating the distance to the ray
						// hit point.
						distance_mode_toggle = !distance_mode_toggle;
						std::cout << "Distance Mode: " <<
							(distance_mode_toggle ? "true distance" : "perpendicular distance") 
							<< std::endl;
						break;
					case sf::Keyboard::R:
						// Toggle between ways of calculating the direction of the ray.
						raydir_mode_toggle = !raydir_mode_toggle;
						std::cout << "Ray Direction Mode: " <<
							(raydir_mode_toggle ? "view plane" : "rotated forward vector")
							<< std::endl;
						break;
					case sf::Keyboard::F:
						if (frame_tex_width > 64) {
							frame_tex_width /= 2;
							frame_tex_height = frame_tex_width;
						}
						frame_tex.create(frame_tex_width, frame_tex_height);
						std::cout << "Frame Texture Width: " << frame_tex_width << std::endl;
						break;
					case sf::Keyboard::G:
						if (frame_tex_width < 512) {
							frame_tex_width *= 2;
							frame_tex_height = frame_tex_width;
						}
						frame_tex.create(frame_tex_width, frame_tex_height);
						std::cout << "Frame Texture Width: " << frame_tex_width << std::endl;
						break;
					}
					break;
				}

			}
		}

		{
			dt += clock.restart().asSeconds();
			float timestep = 1.0f / 60.0f;
			if (dt >= timestep) { // If 1/60th of a second has passed...
				dt = 0.0f;

				// Step the Box2D world.
				{
					int velocity_iterations = 8;
					int position_iterations = 2;
					world.Step(timestep, velocity_iterations, position_iterations);
				}
				
				// Move and rotate the camera.
				{
					const float movespeed = 0.1f;

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
						camera.pos += movespeed * b2Vec2(camera.fwd.x, camera.fwd.y);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
						camera.pos -= movespeed * b2Vec2(camera.fwd.x, camera.fwd.y);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
						b2Vec2 left(camera.fwd.y, -camera.fwd.x);
						camera.pos += movespeed * left;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
						b2Vec2 left(camera.fwd.y, -camera.fwd.x);
						camera.pos -= movespeed * left;
					}

					const float rotatespeed = 0.05f;

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
						camera.fwd = RotateVec(camera.fwd, -rotatespeed);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
						camera.fwd = RotateVec(camera.fwd, rotatespeed);
					}
				}

				// Change the size of the viewing angle / width of the viewing plane
				{
					const float angle_change_speed = 0.05f;
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
						angle_modifier -= angle_change_speed;
						angle_modifier = std::max(angle_modifier, 0.25f);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
						angle_modifier += angle_change_speed;
						angle_modifier = std::min(angle_modifier, 2.0f);
					}
				}
			}
		}


		window.clear(sf::Color(128, 128, 255));

		if (render_box2d_debug) {
			// Use the DebugDrawerSFML we set up earlier to render the world.
			world.DrawDebugData();
			// Draw camera.
			{
				float scale = 32.0f;
				float radius = camera.fwd.Length() * scale;
				sf::CircleShape circle(radius);
				circle.setOrigin(radius, radius);
				circle.setPosition(B2VecToSFVec(camera.pos, scale));
				circle.setOutlineThickness(-1.0f);
				circle.setFillColor(sf::Color::Transparent);

				b2Vec2 endPoint = camera.pos + camera.fwd;
				sf::Vertex line[2] =
				{
					sf::Vertex(B2VecToSFVec(camera.pos, scale)), 
					sf::Vertex(B2VecToSFVec(endPoint, scale))
				};

				window.draw(circle);
				window.draw(line, 2, sf::Lines);
			}
		}
		else {
			//sf::Clock render_time_clock;

			// Render in EXPERIMENTAL RAYCAST MODE:
			frame_tex.clear(sf::Color::Transparent);
			RaycastRender(world, frame_tex, camera);
			frame_tex.display();
			sf::Sprite frame_sprite(frame_tex.getTexture());
			frame_sprite.setScale((float)window.getSize().x / (float)frame_tex.getSize().x, 
				(float)window.getSize().y / (float)frame_tex.getSize().y);
			window.draw(frame_sprite);

			//unsigned render_time_ms = render_time_clock.getElapsedTime().asMilliseconds();
			//std::cout << render_time_ms << std::endl;
		}
		
		window.display();

	}

	return 0;
}


