#pragma once
#include<SFML/Graphics.hpp>
class MySprite {
public:
	int object_id = 4;
	int texture_id;
	int texture_id2=-1;
	int death=-1;
	int velx=0, vely=0;
	int xl=0, xr=0, yt=0, yb=0;

	sf::Sprite* sprite;
	MySprite(int id_,sf::Sprite* ptr): texture_id(id_),sprite(ptr){}
	MySprite(int id_, sf::Texture& text) : texture_id(id_) {
		sprite = new sf::Sprite(text);
	}
};