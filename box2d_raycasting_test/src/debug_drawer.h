// Rachel Crawford 2016
// A lightly modified version of Mativa Lovrekovic's SFMLDebugDraw from https://github.com/MrPlow442/Box2D-SFML2-Debug-Draw

#ifndef DEBUG_DRAWER_H_
#define DEBUG_DRAWER_H_

#include "Box2D/Common/b2Draw.h"

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"

namespace sf {
	class RenderTarget;
}

inline sf::Vector2f B2VecToSFVec(const b2Vec2& vector, float scale) {
	return sf::Vector2f(vector.x * scale, vector.y * scale);
}

inline sf::Color B2ColorToSFColor(const b2Color& color) {
	return sf::Color(sf::Uint8(color.r * 255), sf::Uint8(color.g * 255),
		sf::Uint8(color.b * 255), sf::Uint8(color.a * 255));
}

// Box2D debug drawer which uses SFML graphics.
class DebugDrawerSFML : public b2Draw {
public:
	sf::RenderTarget* m_target = nullptr;
	float m_scale = 32.0f;

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
};

#endif//DEBUG_DRAWER_H_