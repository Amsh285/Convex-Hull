#pragma once
class vector2f
{
public:
	const float& X() const { return m_x; };
	const float& Y() const { return m_y; };

	vector2f(const float& x, const float& y) : m_x(x), m_y(y) {}
private:
	float m_x, m_y;
};

