#pragma once
#ifndef _physics_
#define _physics_
#include"entities.h"
#include<algorithm>
#include"vec2.h"
namespace phy {

	typedef enum {
		left,
		top,
		right,
		bottom,
		none
	} side;
	
	side c_side(Entity& a, Entity& b) {
		vec2 apos(a.sprite.getPosition().x, a.sprite.getPosition().y);
		vec2 bpos(b.sprite.getPosition().x, b.sprite.getPosition().y);
		if (a.obj_trans) apos = a.obj_trans->pos;
		if (b.obj_trans) bpos = b.obj_trans->pos;
		vec2 aprepos(apos), bprepos(bpos);
		if (a.obj_trans) aprepos = apos - a.obj_trans->vel;
		if (b.obj_trans) bprepos = bpos - b.obj_trans->vel;
		float width = (a.size.x + b.size.x) / 2;
		float height = (a.size.y + b.size.y) / 2;
		float dx = width-std::abs(apos.x - bpos.x);
		float dy = height-std::abs(apos.y - bpos.y);
		if (dx < 0 || dy < 0) return none;
		dx = width - std::abs(aprepos.x - bprepos.x);
		dy = height - std::abs(aprepos.y - bprepos.y);
		if (dx <= 0 && dy > 0) {
			if (apos.x > bpos.x) return right;
			else return left ;
		}
		if (apos.y > bpos.y) return bottom;
		else return top;
	}

}
#endif // !_physics_
