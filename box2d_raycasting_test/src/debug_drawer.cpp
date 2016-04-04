// Rachel Crawford 2016
// A lightly modified version of Mativa Lovrekovic's SFMLDebugDraw from https://github.com/MrPlow442/Box2D-SFML2-Debug-Draw

#include "debug_drawer.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/ConvexShape.hpp"
#include "SFML/Graphics/CircleShape.hpp"

void DebugDrawerSFML::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; ++i) {
		sf::Vector2f pos = B2VecToSFVec(vertices[i], m_scale);
		polygon.setPoint(i, pos);
	}
	polygon.setOutlineThickness(-1.0f);
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(B2ColorToSFColor(color));

	m_target->draw(polygon);
}

void DebugDrawerSFML::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; ++i) {
		sf::Vector2f pos = B2VecToSFVec(vertices[i], m_scale);
		polygon.setPoint(i, pos);
	}
	polygon.setOutlineThickness(-1.0f);
	polygon.setOutlineColor(B2ColorToSFColor(color));
	sf::Color c = B2ColorToSFColor(color);
	c.a /= 2;
	polygon.setFillColor(c);

	m_target->draw(polygon);
}

void DebugDrawerSFML::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
	float scaled_radius = radius * m_scale;
	sf::CircleShape circle(scaled_radius);
	circle.setOrigin(scaled_radius, scaled_radius);
	circle.setPosition(B2VecToSFVec(center, m_scale));
	circle.setOutlineThickness(-1.0f);
	circle.setOutlineColor(B2ColorToSFColor(color));

	m_target->draw(circle);
}

void DebugDrawerSFML::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
	float scaled_radius = radius * m_scale;
	sf::CircleShape circle(scaled_radius);
	circle.setOrigin(scaled_radius, scaled_radius);
	circle.setPosition(B2VecToSFVec(center, m_scale));
	circle.setOutlineThickness(-1.0f);
	circle.setOutlineColor(B2ColorToSFColor(color));
	sf::Color c = B2ColorToSFColor(color);
	c.a /= 2;
	circle.setFillColor(c);

	b2Vec2 endPoint = center + radius * axis;
	sf::Vertex line[2] =
	{
		sf::Vertex(B2VecToSFVec(center, m_scale), B2ColorToSFColor(color)),
		sf::Vertex(B2VecToSFVec(endPoint, m_scale), B2ColorToSFColor(color)),
	};

	m_target->draw(circle);
	m_target->draw(line, 2, sf::Lines);
}

void DebugDrawerSFML::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	sf::Vertex line[] =
	{
		sf::Vertex(B2VecToSFVec(p1, m_scale), B2ColorToSFColor(color)),
		sf::Vertex(B2VecToSFVec(p2, m_scale), B2ColorToSFColor(color))
	};

	m_target->draw(line, 2, sf::Lines);
}

void DebugDrawerSFML::DrawTransform(const b2Transform& xf) {
	float linelength = 0.4f;

	b2Vec2 xAxis = xf.p + linelength * xf.q.GetXAxis();
	sf::Vertex redLine[] =
	{
		sf::Vertex(B2VecToSFVec(xf.p, m_scale), sf::Color::Red),
		sf::Vertex(B2VecToSFVec(xAxis, m_scale), sf::Color::Red)
	};

	b2Vec2 yAxis = xf.p + linelength * xf.q.GetYAxis();
	sf::Vertex greenLine[] =
	{
		sf::Vertex(B2VecToSFVec(xf.p, m_scale), sf::Color::Green),
		sf::Vertex(B2VecToSFVec(yAxis, m_scale), sf::Color::Green)
	};

	m_target->draw(redLine, 2, sf::Lines);
	m_target->draw(greenLine, 2, sf::Lines);
}