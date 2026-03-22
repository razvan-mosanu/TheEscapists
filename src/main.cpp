#include <iostream>
#include <SFML/Graphics.hpp>
#include "prison_map.h"
#include "player.h"
#include <algorithm>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "The Escapists - GUI");
    window.setFramerateLimit(60);
    PrisonMap map;
    if (!map.Load("assets/prison_map.tmj", "assets/tileset.png")){}
    Player jucator("Rafa");
    jucator.InitGraphics("assets/player.png", 50.f, 50.f);
    sf::Clock clock;
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event{};
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();
        jucator.UpdateSFML(deltaTime, map);
        sf::Vector2f playerPos = jucator.GetPoziție();
        sf::View camera(sf::FloatRect(0.f, 0.f, 320.f, 240.f));
        float camHalfWidth = camera.getSize().x / 2.f;
        float camHalfHeight = camera.getSize().y / 2.f;
        float camX = std::max(camHalfWidth, std::min(playerPos.x, (float)map.GetWidthInPixels() - camHalfWidth));
        float camY = std::max(camHalfHeight, std::min(playerPos.y, (float)map.GetHeightInPixels() - camHalfHeight));
        camera.setCenter(camX, camY);
        window.clear(sf::Color::Black);
        window.setView(camera);
        window.draw(map);
        jucator.Draw(window);
        window.display();
    }
    return 0;
}
