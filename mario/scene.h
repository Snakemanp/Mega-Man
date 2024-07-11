#pragma once
#ifndef _scene_
#define _scene_
#include"EntityManager.h"
#include<fstream>
#include"assets.cpp"
#include"Animations.h"
#include<map>
#include<iostream>
#include"Physics.h"
#include<SFML/Graphics.hpp>
class scene {
private:
	int frames;
	EntityManager manager;
	std::shared_ptr<Entity> player;
	int prun = 1;
	int pstand = 0;
	assets& Assets;
	inputs userinput;
	sf::RenderWindow& window;
	std::string currentscene;
	sf::View view;
	bool fly_able=false,fly_able_2=false,A=false,D=false;
	int end;
public:
	bool paused = false,ended = false;
	inputs keys;
	scene(const std::string& filename, sf::RenderWindow& window_, assets& a1)
		: frames(0), window(window_), Assets(a1), currentscene(filename)
	{
		view = window.getView();
		std::ifstream file(filename);
		int object_type, texture_id,death;
		float position_x, position_y;
		std::shared_ptr<Entity> temp;
		int lx, by;
		file >> lx >> by;
		view.setCenter(sf::Vector2f(lx + (window.getSize().x / 2), by - (window.getSize().y / 2)));
		file >> end;

		while (file >> object_type >> texture_id)
		{
			if (Assets.textures[texture_id].size() == 0) {
				continue;
			}
			temp = manager.add_entity(object_type);
			if ((object_type & 1) == 1) {
				pstand = texture_id;
				file >> texture_id;
				prun = texture_id;
				player = temp;
				temp->setanimation(Assets.textures[pstand], vec2(1, 1));
			}
			else temp->setanimation(Assets.textures[texture_id], vec2(1, 1));
			if (((object_type >> 4) & 1) == 1) {
				file >> death;
				temp->death = death;
			}
			file >> position_x >> position_y;
			temp->set_position(vec2(position_x, position_y));
			if (((object_type >> 3) & 1) == 1) {
				if ((object_type & 1) == 1) {
					temp->obj_trans = std::make_shared<objtrans>(vec2(position_x, position_y));
				}
				else {
					int velx, vely, xl, xr, yt, yb;
					file >> velx >> vely >> xl >> xr >> yt >> yb;
					temp->obj_trans = std::make_shared<objtrans>(vec2(position_x, position_y));
					temp->obj_trans->vel.x = velx;
					temp->obj_trans->vel.y = vely;
					temp->xlimit = std::make_shared<vec4>(xl, xr, yt, yb);
				}
			}
		}
		manager.update();
		window.setView(view);
	}
	~scene() {
		view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
		window.setView(view);
	}
	
private:
	void update() {
		entity_list temp;
		temp=manager.get_entities();
		for (auto &x : temp) {
			if (x->obj_trans) {
				if (((x->tags >> 5) & 1) == 1) x->obj_trans->accelerate(vec2(0, 1));
				if(x->obj_trans->pos.y > window.getSize().y) manager.del_entity(x);
			}
			x->update();
		}
	};

	void physics() {
		entity_list& Enemy = manager.get_entities(1 << 1);
		entity_list& Ground = manager.get_entities(1 << 2);
		vec2 temppos;
		for (int i = 0; i < Enemy.size(); i++) {
			if (Enemy[i]->obj_trans) {
				Enemy[i]->obj_trans->base = false;
				Enemy[i]->obj_trans->basevel = 0;
			}
			for (int j = 0; j < Ground.size(); j++) {
				phy::side Side = phy::c_side(*Enemy[i], *Ground[j]);
				temppos.x = Enemy[i]->obj_trans->pos.x;
				temppos.y = Ground[j]->sprite.getPosition().y;
				temppos.y -= (Ground[j]->size.y) / 2;
				temppos.y -= Enemy[i]->size.y / 2;
				if (Side == phy::top) {
					if (Ground[j]->obj_trans && Enemy[i]->obj_trans) {
						Enemy[i]->obj_trans->base = true;
						Enemy[i]->obj_trans->basevel=Ground[j]->obj_trans->vel.x;
					}
					Enemy[i]->set_position(temppos);
					temppos = Enemy[i]->obj_trans->vel;
					temppos.y = 0;
					Enemy[i]->obj_trans->change_vel(temppos);
				}
				if (Side == phy::left || Side == phy::right) {
					temppos = Enemy[i]->obj_trans->vel;
					temppos.x *= -1;
					Enemy[i]->obj_trans->change_vel(temppos);
				}
			}
		}
		if (!player) return;
		player->obj_trans->base = false;
		player->obj_trans->basevel = 0;
		for (int i = 0; i < Enemy.size(); i++) {
			if (phy::c_side(*player, *Enemy[i]) != phy::none) {
				if (phy::c_side(*player, *Enemy[i]) == phy::top) {
					manager.del_entity(Enemy[i]);
				}
				else {
					ended = true;
				}
			}
		}
		for (int i = 0; i < Ground.size(); i++) {
			temppos.x = player->obj_trans->pos.x;
			temppos.y = Ground[i]->sprite.getPosition().y;
			temppos.y -= (Ground[i]->size.y) / 2;
			temppos.y -= player->size.y / 2;
			phy::side cside = phy::c_side(*player, *Ground[i]);
			if (cside == phy::none) continue;
			if (cside == phy::top) {
				fly_able = true;
				fly_able_2 = true;
				player->set_position(temppos);
				if (player->obj_trans->vel.y > 0) player->obj_trans->vel.y = 0;
				if (Ground[i]->obj_trans) {
					player->obj_trans->base = true;
					player->obj_trans->basevel=Ground[i]->obj_trans->vel.x;
				}
				continue;
			}
			if (cside == phy::bottom) {
				temppos.x = player->obj_trans->vel.x;
				player->obj_trans->change_vel(vec2(temppos.x, 0));
				continue;
			}
			temppos.x = Ground[i]->sprite.getPosition().x;
			float width = player->size.x;
			if (temppos.x > player->obj_trans->pos.x) {
				temppos.x -= (Ground[i]->size.x / 2 + width / 2);
			}
			else {
				temppos.x += (Ground[i]->size.x / 2 + width / 2);
			}
			temppos.y = player->obj_trans->pos.y;
			player->set_position(temppos);
			temppos.x = 0;
			temppos.y = player->obj_trans->vel.y;
			player->obj_trans->change_vel(temppos);
		}
		if (player->obj_trans->pos.y > window.getSize().y) {
			ended = true;
			/*player.reset();
			manager.get_entities(1)[0].reset();
			view.setCenter(view.getSize().x / 2, view.getCenter().y);
			spawn_player();*/
		}
		if (player->obj_trans->pos.x < player->size.x / 2) {
			player->obj_trans->pos.x = player->size.x / 2;
		}
		if (player->obj_trans->pos.x >= end) {
			std::cout << "Congrats\n";
		}
	}

	void display() {
		entity_list temp;
		temp = manager.get_entities();
		for (auto &x : temp) {
			window.draw(x->sprite);
		}
	}
	void setview() {
		int x = player->obj_trans->vel.x+player->obj_trans->basevel;
		if (view.getCenter().x < player->obj_trans->pos.x) {
			view.move(x, 0);
		}
		if (view.getCenter().x > (player->obj_trans->pos.x + window.getSize().x * 0.3) && view.getCenter().x > view.getSize().x / 2) {
			view.move(x, 0);
		}
		view.setSize(window.getSize().x, window.getSize().y);
		window.setView(view);
	}
public:
	void render() {
		update();
		physics();
		setview();
		window.clear(sf::Color(125, 125, 220));
		display();
	}
	void input(sf::Event &event) {
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == keys.D && !D) {
				player->scale_(vec2(1, 1));
				player->obj_trans->change_vel(vec2(10, player->obj_trans->vel.y));
				player->setanimation(Assets.textures[prun]);
				D = true;
			}
			if (event.key.code == keys.W) {
				if (fly_able_2) player->obj_trans->accelerate(vec2(0, -20));
				if (fly_able == false) fly_able_2 = false;
				else fly_able = false;
			}
			if (event.key.code == keys.A && !A) {
				player->obj_trans->change_vel(vec2(-10, player->obj_trans->vel.y));
				player->setanimation(Assets.textures[prun]);
				player->scale_(vec2(-1, 1));
				A = true;
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == keys.D && D) {
				player->obj_trans->change_vel(vec2(0, player->obj_trans->vel.y));
				player->setanimation(Assets.textures[pstand]);
				D = false;
			}
			if (event.key.code == keys.A && A) {
				player->obj_trans->change_vel(vec2(0, player->obj_trans->vel.y));
				player->setanimation(Assets.textures[pstand]);
				A = false;
			}
		}
	}
};

#endif // !1
