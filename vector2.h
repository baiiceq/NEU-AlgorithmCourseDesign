#pragma once

#include <cmath>

class Vector2
{
public:
	float x = 0;
	float y = 0;
public:
	
	Vector2() = default;
	~Vector2() = default;

	Vector2(float x, float y)
		:x(x), y(y) {}

	Vector2(const Vector2& v)
		:x(v.x), y(v.y) {}



	Vector2 operator+(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	Vector2 operator-(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	bool operator<(const Vector2& vec) const
	{
		if (std::abs(x - vec.x) > 1e-6) 
		{ 
			return x < vec.x;
		}

		if (std::abs(y - vec.y) > 1e-6)
		{
			return y < vec.y;
		}
		return false;
	}


	float operator*(const Vector2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(float val) const
	{
		return Vector2(x * val, y * val);
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x, y += vec.y;
	}

	bool operator==(const Vector2& vec)
	{
		return std::fabs(x - vec.x) < 1e-6 && std::fabs(y - vec.y) < 1e-6;
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x, y -= vec.y;
	}

	void operator*=(int val)
	{
		x *= val, y *= val;
	}

	float length()
	{
		return sqrt(x * x + y * y);
	}

	Vector2 normalize()
	{
		float len = length();

		if (len == 0)
			return Vector2(0, 0);

		return Vector2(x / len, y / len);
	}
};

