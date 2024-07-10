#pragma once
#ifndef _assets_
#define _assets_
#include<string>
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<SFML/Graphics.hpp>
#include"Animations.h"
#include<unordered_map>
class assets {
public:
	std::unordered_map<int, std::vector<sf::Texture>> textures;
    assets(std::string filename="./Animations/Animations.txt") {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error opening file\n";
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string animationName;
            int numberOfFrames, left, top, width, height,id;

            if (!(iss >> id >> animationName  >> numberOfFrames >> left >> top >> width >> height)) {
                std::cout << "Error reading line: " << line << "\n";
                continue;
            }

            for (int i = 0; i < numberOfFrames; i++) {
                sf::Texture temp;
                if (temp.loadFromFile("./Animations/"+animationName + ".png", sf::IntRect(left + (i * width), top, width, height))) {
                    textures[id].push_back(temp);
                }
                else {
                    std::cout << "Error loading texture for " << animationName << "\n";
                }
            }
        }

        file.close();

    }
};
#endif // !_assets_
