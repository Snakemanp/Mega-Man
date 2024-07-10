#pragma once
#include "vec2.h"

class vec4 {
public:
	vec2 xlimit, ylimit;
	vec4(vec2& x, vec2& y) : xlimit(x), ylimit(y) {}
	vec4(float xl, float xr, float yt, float yb) : xlimit(xl, xr), ylimit(yt, yb) {}
};