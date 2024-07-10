#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "entities.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <algorithm>

typedef std::vector<std::shared_ptr<Entity>> entity_list;

class EntityManager {
    entity_list entities;
    std::map<int, entity_list> entity_map;
    long int no_entities = 0;
    entity_list changes;

public:
    EntityManager() {}

    void update() {
        for (auto& ele : changes) {
            if ((ele->tags & 1) == 1) entity_map[1].push_back(ele);
            if (((ele->tags >> 1) & 1) == 1) entity_map[1 << 1].push_back(ele);
            if (((ele->tags >> 2) & 1) == 1) entity_map[1 << 2].push_back(ele);
            if (((ele->tags >> 5) & 1) == 1) entity_map[1 << 5].push_back(ele);
            if (((ele->tags >> 6) & 1) == 1) entity_map[1 << 6].push_back(ele);
            entities.push_back(ele);
        }
        changes.clear();
    }

    std::shared_ptr<Entity> add_entity(const int& Tags) {
        std::shared_ptr<Entity> temp = std::make_shared<Entity>(no_entities++, Tags);
        changes.push_back(temp);
        return temp;
    }

    entity_list& get_entities(const int& Tag) {
        return entity_map[Tag];
    }

    void get_entities(const int Tags, entity_list& list) {
        list.clear();
        for (auto& temp : entities) {
            if ((temp->tags & Tags) == Tags) {
                list.push_back(temp);
            }
        }
    }

    entity_list& get_entities() {
        return entities;
    }

    void del_entity(std::shared_ptr<Entity> edel) {
        // Remove from entities vector
        entities.erase(std::remove(entities.begin(), entities.end(), edel), entities.end());

        // Remove from entity_map
        for (auto& pair : entity_map) {
            if ((pair.first & edel->tags) > 0) {
                entity_list& list = pair.second;
                list.erase(std::remove(list.begin(), list.end(), edel), list.end());
            }
        }
    }
};

#endif // ENTITY_MANAGER_H
