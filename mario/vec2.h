#pragma once
#ifndef obj_entities
#define obj_entities
#include<cmath>
#include<SFML/System.hpp>
#include<SFML/Graphics.hpp>
class vec2 {
public:
	float x, y;
	vec2(float x_, float y_) : x(x_), y(y_) {}
	vec2(const sf::Vector2f& a) : x(a.x), y(a.y) {}
	vec2(float angle) {
		angle *= 3.14;
		angle /= 180;
		x = sin(angle);
		y = cos(angle);
	}
	vec2() :x(0), y(0) {}
	vec2(const vec2& a) {
		x = a.x;
		y = a.y;
	}
	vec2 operator +(vec2 a) {
		vec2 temp;
		temp.x = x + a.x;
		temp.y = y + a.y;
		return temp;
	}
	vec2 operator -(vec2 a) {
		vec2 temp;
		temp.x = x - a.x;
		temp.y = y - a.y;
		return temp;
	}
	vec2 operator *(float a) {
		vec2 temp;
		temp.x = x * a;
		temp.y = y * a;
		return temp;
	}
	vec2 operator /(float a) const {
		vec2 temp;
		temp.x = x / a;
		temp.y = y / a;
		return temp;
	}
	vec2& operator =(vec2 a) {
		x = a.x;
		y = a.y;
		return(*this);
	}
	float length() const {
		return(std::sqrt(x * x + y * y));
	}
	vec2 normalize()const {
		float len = std::sqrt(x * x + y * y);
		if (len == 0) return(vec2(0, 0));
		vec2 temp;
		temp = (*this) / len;
		return temp;
	}
	float obj_dist(vec2& b) {
		float temp;
		temp = (x - b.x) * (x - b.x);
		temp += (y - b.y) * (y - b.y);
		temp = std::sqrt(temp);
		return temp;
	}
};
#endif // !obj_entities
