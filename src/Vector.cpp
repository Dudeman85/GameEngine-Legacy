
#include "engine/Vector.h"

namespace engine
{
	Vector2::Vector2()
	{
		x = 0;
		y = 0;
	}
	Vector2::Vector2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
	Vector2::Vector2(Vector3 vec3)
	{
		x = vec3.x;
		y = vec3.y;
	}

	bool Vector2::operator==(const Vector2& rhs)
	{
		return x == rhs.x && y == rhs.y;
	}

	Vector2 Vector2::operator+(float add)
	{
		return Vector2(x + add, y + add);
	}
	Vector2 Vector2::operator+(Vector2 add)
	{
		return Vector2(x + add.x, y + add.y);
	}
	Vector2& Vector2::operator+=(const Vector2& add)
	{
		x += add.x;
		y += add.y;
		return *this;
	}

	Vector2 Vector2::operator-(Vector2 sub)
	{
		return Vector2(x - sub.x, y - sub.x);
	}
	Vector2& Vector2::operator-=(const Vector2& sub)
	{
		x -= sub.x;
		y -= sub.y;
		return *this;
	}

	Vector2 Vector2::operator*(float mult)
	{
		return Vector2(x * mult, y * mult);
	}
	Vector2& Vector2::operator*=(float mult)
	{
		x *= mult;
		y *= mult;
		return *this;
	}

	Vector2 Vector2::operator/(float div)
	{
		return Vector2(x / div, y / div);
	}

	Vector2 Vector2::Normalize() {
		float length = sqrt(x * x + y * y);
		return Vector2(x / length, y / length);
	}

	Vector3::Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3::Vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3::Vector3(Vector2 vec2, float _z)
	{
		x = vec2.x;
		y = vec2.y;
		z = _z;
	}
	Vector3 Vector3::operator+(float add)
	{
		return Vector3(x + add, y + add, z + add);
	}
	Vector3 Vector3::operator+(Vector3 add)
	{
		return Vector3(x + add.x, y + add.y, z + add.z);
	}
	Vector3& Vector3::operator+=(const Vector3& add)
	{
		x += add.x;
		y += add.y;
		z += add.z;
		return *this;
	}
	Vector3 Vector3::operator*(float mult) {
		return Vector3(x * mult, y * mult, z * mult);
	}
	Vector3 Vector3::operator*(Vector3 mult) {
		return Vector3(x * mult.x, y * mult.y, z * mult.z);
	}
	Vector3& Vector3::operator*=(float mult) {
		x *= mult;
		y *= mult;
		z *= mult;
		return *this;
	}

	Vector3 Vector3::Normalize() {
		float length = sqrt(x * x + y * y + z * z);
		return Vector3(x / length, y / length, z / length);
	}

}