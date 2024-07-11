#ifndef _HOME_H_
#define _HOME_H_

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "Assets.cpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "NewSprite.h"
#include<Windows.h>

class Home {
    sf::RenderWindow& window;
    sf::Clock deltaClock;
    ImFont* font1 = nullptr;
    ImFont* font2 = nullptr;
    ImFont* font0 = nullptr;
    sf::Texture fontTexture;
    int curscene=0;
    std::vector<std::pair<std::string, std::string>> levels;
    sf::Texture bg;
    sf::Sprite bgsp;
    assets& a;
    bool drag=false,dragdone=true;
    std::vector<MySprite*> sprites;
    MySprite* temp=nullptr;
    MySprite* wintemp = nullptr;
    bool spr_win = false;
    bool player_found = false;
    int leftx=0, bottomy=0, winx=0;
    std::string existing_l;
    bool writing = false;
    char buffer[30];
public:
    Home(sf::RenderWindow& window_, assets &a_) : window(window_) ,a(a_) {
        ImGui::SFML::Init(window);

        bg.loadFromFile("background.png");
        bgsp.setTexture(bg);

        ImGuiIO& io = ImGui::GetIO();
        font0 = io.Fonts->AddFontFromFileTTF("Fonts/AovelSansRounded.ttf", 24);
        font1 = io.Fonts->AddFontFromFileTTF("Fonts/Wonderbar-pALD.ttf", 80);
        font2 = io.Fonts->AddFontFromFileTTF("Fonts/Wonderbar-pALD.ttf", 20);

        if (font1 && font2) {
            unsigned char* tex_pixels = nullptr;
            int tex_width, tex_height;
            io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);

            fontTexture.create(tex_width, tex_height);
            fontTexture.update(tex_pixels);

            io.Fonts->TexID = (void*)fontTexture.getNativeHandle();

            io.Fonts->ClearTexData();
        }
        LoadLevels();

        sf::View view = window.getView();
        view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
        window.setView(view);
    }

    ~Home() {
        ImGui::SFML::Shutdown();
    }

    void render(std::string &str) {
        update(str);
        display();
        ImGui::SFML::Render(window);
    }

    void handleDrop(sf::Event &event) {
        if (writing) return;
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !spr_win) {
            if (curscene == 1 && temp) {
                sprites.push_back(temp);
                temp = nullptr;
                drag = false;
            }
            if (curscene == 1 && dragdone) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                if (temp) delete temp;
                temp = getClickedSprite(worldPos);
                if(temp) drag = true;
            }
            if (!drag) dragdone = true;
            if (drag) dragdone = false;
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && !drag) {
            if (!spr_win) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                wintemp = getClickedSprite(worldPos);
                if (wintemp) spr_win = true;
                else spr_win = false;
                return;
            }
            if (spr_win) {
                if (wintemp)sprites.push_back(wintemp);
                spr_win = false;
                wintemp = nullptr;
            }
        }
        if (event.type == sf::Event::KeyPressed) {
            if (curscene == 1) {
                sf::View view = window.getView();
                if (event.key.code == sf::Keyboard::D) {
                    view.move(20, 0);
                }
                if (event.key.code == sf::Keyboard::A) {
                    view.move(-20, 0);
                }
                if (event.key.code == sf::Keyboard::W) {
                    view.move(0, -20); // Move up
                }
                if (event.key.code == sf::Keyboard::S) {
                    view.move(0, 20); // Move down
                }
                window.setView(view);
            }
        }
    }

private:

    void update(std::string &str) {

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::SetWindowSize(ImVec2(window.getSize().x, window.getSize().y));

        if (font0) ImGui::PushFont(font0);
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::MenuItem("Home")) {
                reset();
                curscene = 0;
            }
            if (ImGui::BeginMenu("Play")) {
                ImGui::BeginChild("ScrollingRegion", ImVec2(130, 100), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                for (std::pair<std::string, std::string>& level : levels) {
                    if (ImGui::MenuItem(level.first.c_str())) {
                        str = level.second;
                        reset();
                    }
                }
                ImGui::EndChild();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Create")) {
                if (ImGui::BeginMenu("New Level")) {
                    if (ImGui::MenuItem("Save", "Ctrl+S")) {
                        if (existing_l != "") {
                            write_level(existing_l);
                        }
                        else if(sprites.size()!=0 && player_found) {
                            writing = true;
                        }
                        else {
                            MessageBox(NULL, L"InCorrect Format", L"Error", MB_OK | MB_ICONINFORMATION);
                        }
                    }
                    if (ImGui::MenuItem("Create New")) {
                        reset();
                        curscene = 1;
                    }
                    if (ImGui::BeginMenu("Load Existing level")) {
                        ImGui::BeginChild("ScrollingRegion", ImVec2(130, 100), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                        for (std::pair<std::string, std::string>& level : levels) {
                            if (ImGui::MenuItem(level.first.c_str())) {
                                reset();
                                curscene = 1;
                                existing_l = level.second;
                                Load_from_existing(level.second);
                            }
                        }
                        ImGui::EndChild();
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Animations")) {
                    reset();
                    curscene = 2;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Settings")) {
                if (ImGui::MenuItem("Option 1")) {
                    reset();
                    curscene = 3;
                }
                if (ImGui::MenuItem("Option 2")) {
                    reset();
                    curscene = 4;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        if (font0) ImGui::PopFont();
    }

    void render_Home() {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.14f, 0.17f, 0.2f));
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
        ImGui::SetNextWindowPos(ImVec2(0, 30));
        ImGui::SetNextWindowSize(ImVec2(window.getSize().x, window.getSize().y - 30));
        ImGui::Begin("Home", nullptr, windowFlags);
        if (font1) ImGui::PushFont(font1);
        ImGui::SetCursorPos(ImVec2(window.getSize().x * 0.5f - ImGui::CalcTextSize("My Game 1").x / 2, window.getSize().y * 0.2f));
        ImGui::Text("Mega Man");
        if (font1) ImGui::PopFont();
        ImGui::End();
        ImGui::PopStyleColor();
    }

    void render_Newlevel() {
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Assets", nullptr, windowFlags);

        if (font0) ImGui::PushFont(font0);

        ImGui::SeparatorText("All assets");

        if (ImGui::BeginTable("Assets", 4, ImGuiTableFlags_Borders)) {

            for (auto& asset : a.textures) {
                ImGui::TableNextColumn();

                // Convert SFML texture to OpenGL texture ID
                GLuint my = asset.second[0].getNativeHandle();
                ImTextureID texID = (ImTextureID)(intptr_t)my;

                ImGui::PushID(asset.first); 

                if (ImGui::ImageButton(texID, ImVec2(40, 40))) {
                    if (!drag && !spr_win) {
                        sf::Vector2i mpos = sf::Mouse::getPosition(window);
                        sf::Vector2f wpos = window.mapPixelToCoords(mpos);
                        temp = new MySprite(asset.first, asset.second[0]);
                        sf::Vector2u temp_size = asset.second[0].getSize();
                        temp->sprite->setOrigin(temp_size.x / 2, temp_size.y / 2);
                        temp->sprite->setPosition(wpos.x, wpos.y);
                        drag = true;
                        dragdone = false;
                        break;
                    }
                }
                if (!spr_win) continue;
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    ImGui::SetDragDropPayload("Texture_Id", &asset.first, sizeof(int));
                    ImGui::Image(texID, ImVec2(40, 40));
                    ImGui::EndDragDropSource();
                }

                ImGui::PopID();
            }

            ImGui::EndTable();
        }
        if (font0) ImGui::PopFont();
        ImGui::SeparatorText("winning and world positions");
        ImGui::InputInt("Winning X:", &winx);
        ImGui::InputInt("Left World X: ", &leftx);
        ImGui::InputInt("Bootom World Y: ", &bottomy);
        ImGui::TextDisabled("Enter cordinates of world like left most x cordinate of world and bottom most y cordinate in world");

        sf::Vector2i Pos = sf::Mouse::getPosition(window);
        sf::Vector2f wPos = window.mapPixelToCoords(Pos);
        ImGui::SeparatorText("Mouse position");
        ImGui::Text("X: %d  Y: %d", (int)wPos.x, (int)wPos.y);

        ImGui::SeparatorText("Create Level");
        if(ImGui::Button("Save")) {

        }

        ImGui::End();
    }

    void display() {
        sf::Vector2u view = window.getSize();
        sf::Vector2u imsize = bg.getSize();

        float scaleX = static_cast<float>(view.x) / imsize.x;
        float scaleY = static_cast<float>(view.y) / imsize.y;
        float scale = std::max(scaleX, scaleY);

        bgsp.setScale(scale, scale);
        sf::Vector2f bgspSize(bgsp.getGlobalBounds().width, bgsp.getGlobalBounds().height);
        bgsp.setPosition(view.x/2-bgspSize.x/2,view.y/2-bgspSize.y/2);

        if (curscene == 0) {
            render_Home();
            window.draw(bgsp);
        }
        if (curscene == 1) {
            draw_spaites();
            if (writing) {
                write_new_level();
                return;
            }
            render_Newlevel();
        }
    }

    void LoadLevels() {
        std::ifstream file("Levels.txt");
        if (!file.is_open()) {
            std::cout << "failed";
            return;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string levelName, levelLocation;

            if (std::getline(iss, levelName, ',')) {
                if (std::getline(iss, levelLocation)) {
                    levels.push_back({ levelName, levelLocation });
                    std::cout << levelName<<','<< levelLocation;
                }
            }
        }
        file.close();
    }

    void draw_spaites() {
        for (auto& item : sprites) {
            window.draw(*item->sprite);
        }
        if (temp) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window);
            sf::Vector2f wpos = window.mapPixelToCoords(mpos);
            temp->sprite->setPosition(wpos.x, wpos.y);
            window.draw(*temp->sprite);
        }
        if (wintemp && spr_win) {
            window.draw(*wintemp->sprite);
            draw_sprie_win(wintemp);
        }
        return;
    }

    MySprite* getClickedSprite(const sf::Vector2f& worldPos) {
        MySprite* drag = nullptr;
        std::vector<MySprite*>::iterator it;

        
        for (auto tsprite = sprites.rbegin(); tsprite != sprites.rend(); ++tsprite) {
            
            if ((*tsprite)->sprite->getGlobalBounds().contains(worldPos)) {
                drag = *tsprite;
                it = tsprite.base();
                break;
            }
        }

        if (drag) {
            sprites.erase(--it);
        }
        return drag;
    }

    void draw_sprie_win(MySprite* ptr) {
        sf::Vector2f pos = ptr->sprite->getPosition();
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
        ImGui::SetNextWindowSize(ImVec2(300, 300));
        ImGui::Begin("Entity Properties",nullptr,windowFlags);
        ImGui::SeparatorText("Position");
        ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.00f, 1.00f, 1.00f, 1.00f));
        ImGui::TextDisabled("Position x: %d  y: %d", (int)pos.x, (int)pos.y);
        ImGui::PopStyleColor();
        int e = ptr->object_id % 8;
        bool isplayer=false;
        ImGui::SeparatorText("Entity Type");
        ImGui::Text("Object Id: %d", ptr->object_id);
        if ((player_found && e != 1) == false) {
            ImGui::RadioButton("Player", &e, 1);
            if (player_found) isplayer = true;
        }
        ImGui::RadioButton("Enemy", &e, 2);
        ImGui::RadioButton("Background", &e, 4);
        ptr->object_id -= ptr->object_id % 8;
        ptr->object_id += e;
        if (isplayer && e != 1) player_found = false;
        if (e == 1) {
            player_found = true;
            isplayer = true;
            ImGui::SeparatorText("Player Run Animation");
            ImGui::InputInt("Texture Id", &(ptr->texture_id2));
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture_Id")) {
                    IM_ASSERT(payload->DataSize == sizeof(int));
                    int payload_n = *(const int*)payload->Data;
                    ptr->texture_id2 = payload_n;
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.00f, 1.00f, 1.00f, 1.00f));
            ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
            ImGui::TextDisabled("Drag the Texture from the Assets window and drop to set automatically");
            ImGui::PopTextWrapPos();
            ImGui::PopStyleColor();
        }
        if (e != 1) ImGui::SeparatorText("Movement");
        bool objtrans = (((ptr->object_id >> 3) & 1) == 1);
        if (e != 1)ImGui::Checkbox("Has Movement", &objtrans);
        if (e == 1 || objtrans) ptr->object_id |= (1 << 3);
        else ptr->object_id &= ~(1 << 3);
        if (e != 1 && objtrans) {
            ImGui::InputInt("X-Speed", &(ptr->velx));
            ImGui::InputInt("Y-Speed", &(ptr->vely));
            ImGui::InputInt("Limit X-left", &(ptr->xl));
            ImGui::InputInt("Limit X-Right", &(ptr->xr));
            ImGui::InputInt("Limit Y-top", &(ptr->yt));
            ImGui::InputInt("Limit Y-bottom", &(ptr->yb));
        }
        ImGui::SeparatorText("Has Animation at Death");
        bool deathani = (((ptr->object_id >> 4) & 1) == 1);
        ImGui::Checkbox("Death Animation", &deathani);
        if (deathani) ptr->object_id |= (1 << 4);
        else ptr->object_id &= ~(1 << 4);
        if (deathani) {
            ImGui::InputInt("Texture Id", &(ptr->death));
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture_Id")) {
                    IM_ASSERT(payload->DataSize == sizeof(int));
                    int payload_n = *(const int*)payload->Data;
                    ptr->death = payload_n;
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.00f, 1.00f, 1.00f, 1.00f));
            ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
            ImGui::TextDisabled("Drag the Texture from the Assets window and drop to set automatically");
            ImGui::PopTextWrapPos();
            ImGui::PopStyleColor();
        }
        if (e != 1) {
            ImGui::SeparatorText("Is effected by Gravity");
            bool gravity = (((ptr->object_id >> 5) & 1) == 1);
            ImGui::Checkbox("Gravity", &gravity);
            if (gravity) ptr->object_id |= (1 << 5);
            else ptr->object_id &= ~(1 << 5);
        }
        else {
            ptr->object_id |= (1 << 5);
        }
        ImGui::SeparatorText("Delete Entity");
        if (ImGui::Button("Remove")) {
            if (isplayer) player_found = false;
            delete wintemp;
            wintemp = nullptr;
            spr_win = false;
        }
        ImGui::End();
    }

    void Load_from_existing(std::string& level) {
        sprites.clear();
        std::ifstream Level(level);
        MySprite* temp;
        int objid, textureid,xpos,ypos;
        
        Level >> leftx >> bottomy >> winx;
        while (Level >> objid >> textureid) {
            temp = new MySprite(textureid, a.textures[textureid][0]);
            temp->object_id = objid;
            sf::Vector2u temp_size = a.textures[textureid][0].getSize();
            temp->sprite->setOrigin(temp_size.x / 2, temp_size.y / 2);
            if ((objid & 1) == 1) {
                Level >> textureid;
                temp->texture_id2 = textureid;
            }
            if (((objid >> 4) & 1) == 1) {
                Level >> textureid;
                temp->death = textureid;
            }
            Level >> xpos >> ypos;
            temp->sprite->setPosition(xpos, ypos);
            if ((objid & 1) == 1) {
                sprites.push_back(temp);
                continue;
            }
            if (((objid >> 3) & 1) == 1) {
                Level >> xpos >> ypos;
                temp->velx = xpos;
                temp->vely = ypos;
                Level >> xpos >> ypos;
                temp->xl = xpos;
                temp->xr = ypos;
                Level >> xpos >> ypos;
                temp->yt = xpos;
                temp->yb = ypos;
            }
            sprites.push_back(temp);
        }
    }
    void reset() {
        if (curscene != 1) return;
        sf::View view = window.getView();
        view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
        window.setView(view);
        sprites.clear();
        if (temp) delete temp;
        drag = false;
        dragdone = true;
        spr_win = false;
        leftx = 0;
        bottomy = 0;
        winx = 0;
        existing_l = "";
        writing = false;
    }

    void write_level(std::string &level) {
        std::ofstream File(level,std::ios::trunc);
        File << leftx << ' ' << bottomy << ' ' << winx<<'\n';
        if (temp) sprites.push_back(temp);
        for (MySprite*& x : sprites) {
            File << x->object_id << ' '<<x->texture_id<<' ';
            if (((x->object_id) & 1) == 1) {
                File << x->texture_id2<<' ';
            }
            if (((x->object_id>>4) & 1) == 1) {
                File << x->death << ' ';
            }
            sf::Vector2f pos = x->sprite->getPosition();
            File << pos.x << ' ' << pos.y << ' ';
            if ((x->object_id & 1) == 1) {
                File << '\n';
                continue;
            }
            if (((x->object_id >> 3) & 1) == 1) {
                File << x->velx << ' ' << x->vely << ' ' << x->xl << ' ' << x->xr << ' ' << x->yt << ' ' << x->yb << ' ';
            }
            File << '\n';
        }
        reset();
    }

    void write_new_level() {
        ImGui::Begin("File Name");

        ImGui::InputText("Level Name", buffer, 30);

        if (ImGui::Button("Cancel")) {
            writing = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Save")) {
            std::string levelName(buffer);
            if (!levelName.empty()) {
                std::string levelFileName = "Levels/" + levelName + ".txt";
                write_level(levelFileName);
                std::ofstream file("Levels.txt", std::ios::app);
                if (file.is_open()) {
                    file << levelName << "," << levelFileName << '\n';
                    file.close();
                }
                else {
                    MessageBox(NULL, L"Failed to open Levels.txt", L"Error", MB_OK | MB_ICONERROR);
                }
            }
            else {
                MessageBox(NULL, L"Level name cannot be empty", L"Error", MB_OK | MB_ICONERROR);
            }
            writing = false;
        }

        ImGui::End();
    }

};

#endif
