#pragma once
#include <cmath>
#include <glm/glm.hpp>

namespace engine
{
	class Vector3;

	//Two floats in one
	class Vector2
	{
	public:
		Vector2();
		Vector2(float _x, float _y);
		Vector2(Vector3 vec3);

		bool operator==(const Vector2& rhs);

		Vector2 operator+(float add);
		Vector2 operator+(Vector2 add);
		Vector2& operator+=(const Vector2& add);

		Vector2 operator-(Vector2 sub);
		Vector2& operator-=(const Vector2& sub);

		Vector2 operator*(float mult);
		Vector2& operator*=(float mult);

		Vector2 operator/(float div);
		Vector2 Normalize();

		float Squared() const;
		float x, y;
	};

	//Three floats in one
	class Vector3
	{
	public:
		Vector3();
		Vector3(float all);
		Vector3(float _x, float _y, float _z);
		Vector3(Vector2 vec2, float _z = 0);
		Vector3 operator+(float add);
		Vector3 operator+(Vector3 add);
		Vector3& operator+=(const Vector3& add);
		Vector3 operator*(float mult);
		Vector3 operator*(Vector3 mult);
		Vector3& operator*=(float mult);
		Vector3 Normalize();
		Vector3 Pow(float power);
		glm::vec3 ToGlm();
		float x, y, z;
	};
}