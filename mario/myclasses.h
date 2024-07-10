#pragma once
#ifndef classes_game
#define classes_game
#include"vec2.h"
#include<ctime>
class objtrans {
public:
	vec2 pos;
	vec2 vel;
	vec2 maxvel;
	int basevel=0;
	bool base=false;
	objtrans(const vec2& p = vec2(0, 0), const vec2& v = vec2(0, 0),const vec2& maxvel_=vec2(20,40) , const int r = 0) :pos(p), vel(v),maxvel(maxvel_), basevel(r) {}
	void change_pos(const vec2 pos_) {
		pos = pos_;
	}
	void upd_pos() {
		pos = pos + vel;
		if (base) pos.x += basevel;
	}
	void change_vel(const vec2 vel_) {
		if (vel_.x >= 0)vel.x = std::min(vel_.x, maxvel.x);
		else vel.x = std::max(vel_.x, maxvel.x * -1);
		if (vel_.y >= 0)vel.y = std::min(vel_.y, maxvel.y);
		else vel.y = std::max(vel_.y, maxvel.y * -1);
	}
	void accelerate(const vec2 vel_) {
		vel=vel + vel_;
		if (abs(vel.x) > maxvel.x)vel.x = maxvel.x* vel.x/abs(vel.x);
		if (abs(vel.y) > maxvel.y)vel.y = maxvel.y* vel.y / abs(vel.y);
	}
	void update() {
		upd_pos();
	}
};
class lifespan {
public:
	int timeleft;
	int totaltime;
	lifespan(int time) :totaltime(time), timeleft(time) {}
	void update() {
		timeleft--;
	}
	bool alive() {
		if (timeleft <= 0) return false;
		return true;
	}
	int getalpha() {
		return (int)((255.0f / totaltime) * timeleft);
	}
};
class inputs {
public:
	sf::Keyboard::Key A, S, W, D, shoot;
	inputs() :A(sf::Keyboard::A), S(sf::Keyboard::S), W(sf::Keyboard::W), D(sf::Keyboard::D), shoot(sf::Keyboard::Space) {}
};
#endif // !classes_game
