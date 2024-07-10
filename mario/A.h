#pragma once
#ifndef _assets_
#define _assets_
#include<string>
#include<vector>
#include<SFML/Graphics.hpp>
#include"animation.h"
#include<unordered_map>
class assets {
public:
	std::unordered_map<std::string, std::vector<sf::Texture>> textures;
	std::unordered_map<std::string, Animation> animations;
	assets() {
		sf::Texture temp;
		for (int i = 0; i < 6; i++) {
			if (temp.loadFromFile(("sprite1.png"), sf::IntRect(256 * i, 0, 256, 256))) {
				textures["PRun"].push_back(temp);
				if (i == 1) textures["Pstand"].push_back(temp);
			}
		}
		temp.loadFromFile(("sprite3.jpg"), sf::IntRect(0, 2150, 5000, 600));
		textures["Ground"].push_back(temp);
		animations["PRun"] = Animation(textures["PRun"]);
		animations["Pstand"] = Animation(textures["Pstand"]);
		animations["PRun"].init();
		animations["Pstand"].init();
	}
};
#endif // !_assets_
