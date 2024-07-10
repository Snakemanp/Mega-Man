#pragma once
#ifndef entity_class
#define entity_class

#include <string>
#include "myclasses.h"
#include"vec2.h"
#include <memory>
#include"Animations.h"
#include <SFML/Graphics.hpp>
#include<map>
#include<iostream>
#include "vec4.h"

class Entity {
public:
    const long int id;
    const int tags;
    Entity(const long int id_, const int& Name_) : id(id_), tags(Name_) {}
    ~Entity() {
        std::cout << "Removed an entity\n";
    }
    bool alive = true;
    vec2 scale=vec2(1,1);
    std::shared_ptr<objtrans> obj_trans;
    sf::Sprite sprite;
    vec2 size;
    vec2 sizescale=(1,1);
    std::shared_ptr<vec4> xlimit = nullptr;
    int death=-1;

    std::shared_ptr<Animation> currentanimation;
    void update() {
        if (obj_trans) obj_trans->update();
        if (currentanimation) sprite.setTexture(currentanimation->update());
        sprite.setScale(scale.x, scale.y);
        if (obj_trans)sprite.setPosition(obj_trans->pos.x, obj_trans->pos.y);
        if (xlimit) {
            if (obj_trans->pos.x < xlimit->xlimit.x) {
                obj_trans->pos.x -= obj_trans->vel.x;
                obj_trans->vel.x *= -1;
                scale.x = -1;
            }
            if (obj_trans->pos.x > xlimit->xlimit.y) {
                obj_trans->pos.x -= obj_trans->vel.x;
                obj_trans->vel.x *= -1;
                scale.x = -1;
            }
            if (obj_trans->pos.y < xlimit->ylimit.x) {
                obj_trans->pos.y -= obj_trans->vel.y;
                obj_trans->vel.y *= -1;
            }
            if (obj_trans->pos.y > xlimit->ylimit.y) {
                obj_trans->pos.y -= obj_trans->vel.y;
                obj_trans->vel.y *= -1;
            }
        }
    }
    void initialise(sf::Texture& texture,vec2 scale1=vec2(1,1)) {
        sprite.setTexture(texture);
        size.x = texture.getSize().x*scale1.x;
        size.y = texture.getSize().y*scale1.y;
        sprite.setOrigin(size.x / 2, size.y / 2);
    }
    void set_position(const vec2 pos) {
        if (obj_trans) obj_trans->change_pos(pos);
        sprite.setPosition(pos.x,pos.y);
    }
    void scale_(const vec2 scale_) {
        scale = scale_;
        sprite.setScale(scale.x, scale.y);
    }
    void setanimation(TextureList &textures,vec2 scale1=vec2(0,0), int life = 0, int speed = 6) {
        float x, y;
        x = textures[0].getSize().x / 2;
        y = textures[0].getSize().y / 2;
        if (scale1.x != 0 && scale1.y != 0)sizescale = scale1;
        size.x = 2 * x * sizescale.x;
        size.y = 2 * y * sizescale.y;
        currentanimation = std::make_shared<Animation>(textures, life, speed);
        sprite.setTexture(currentanimation->init());
        sf::IntRect rect = sprite.getTextureRect();
        rect.width = x * 2;
        rect.height = y * 2;
        sprite.setTextureRect(rect)
            ;
        sprite.setOrigin(x,y);
    }
    bool canremove() {
        if (currentanimation->isAlive() || alive) return false;
        return true;
    }

    void set_death(TextureList& textures) {
        if (!alive) return;
        alive = false;
        if (death == -1) return;
        setanimation(textures, vec2(0, 0), textures.size() * 6);
    }
};

#endif // !entity_class
