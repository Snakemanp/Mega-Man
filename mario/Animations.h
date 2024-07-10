#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include<iostream>
using TextureList = std::vector<sf::Texture>;

class Animation {
public:

    Animation(TextureList& textures, int life = 0, int speed = 6)
        : textures(textures), speed(speed), life(life) {}

    ~Animation() {}

    const sf::Texture& update() {
        frames++;
        if (isAlive()) {
            return textures[(frames / speed) % textures.size()];
        }
        if (life != 0 && frames > life) {
            alive = false;
        }
        return textures.front();
    }
    const sf::Texture& init() {
        frames = 0;
        alive = true;
        return textures[0];
    }
    bool isAlive() const {
        return alive && (life == 0 || frames <= life);
    }


private:
    TextureList& textures;
    int frames = 0;
    bool alive = false;
    int life;
    int speed;
};

#endif // !_ANIMATION_H_
