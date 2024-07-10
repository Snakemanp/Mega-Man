// mario.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include"Assets.cpp"
#include"scene.h"
#include<string>
#include<SFML/Graphics.hpp>
#include<Windows.h>
#include<iostream>
#include<imgui.h>
#include <imgui-SFML.h>
#include "Home.cpp"

int main()
{
    bool A=false, D=false;
    assets a;
    HWND hwnd;
    sf::RenderWindow window(sf::VideoMode(1200, 800), "my_window");
    hwnd = window.getSystemHandle();
    window.setFramerateLimit(60);
    scene* test = nullptr;
    Home *home = nullptr;
    home = new Home(window,a);
    std::string curlevel = "";

    while (window.isOpen())
    {
        window.clear(sf::Color(125, 125, 220));
        if (home) home->render(curlevel);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized && home) {
                sf::View view = window.getView();
                view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
                window.setView(view);
            }
            if (test) test->input(event);
            if (home) {
                ImGui::SFML::ProcessEvent(event);
                home->handleDrop(event);
            }
        }
        if (test) test->render();
        window.display();
        if (curlevel != "") {
            test = new scene(curlevel, window, a);
            curlevel = "";
            delete home;
            home = nullptr;
        }
        if (test) {
            if (test->ended) {
                delete test;
                home = new Home(window,a);
                test = nullptr;
            }
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
