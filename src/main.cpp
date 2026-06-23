#include <iostream>
#include <SFML/Graphics.hpp>
#include "game_manager.h"
#include "player.h"
#include "guard.h"
#include "inmate.h"
#include "exceptions.h"
#include "warden.h"

static sf::Vector2f tile(float col, float row)
{
    return {col * 16.f + 8.f, row * 16.f + 8.f};
}

int main()
{
    try
    {
        sf::RenderWindow window(sf::VideoMode(800, 600), "The Escapists - Prison Map");
        window.setFramerateLimit(60);

        GameManager game;
        game.Initialize();

        auto player = std::make_shared<Player>("Rafa");
        player->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(4, 4).x, tile(4, 4).y, sf::Color::White);
        auto guard1 = std::make_shared<Guard>("Officer Smith");
        guard1->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(5, 8).x, tile(5, 8).y, sf::Color(100, 100, 255));
        auto guard2 = std::make_shared<Guard>("Officer Brown");
        guard2->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(20, 8).x, tile(20, 8).y, sf::Color(100, 100, 255));
        auto guard3 = std::make_shared<Guard>("Officer Lee");
        guard3->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(35, 8).x, tile(35, 8).y, sf::Color(80, 80, 220));
        auto inmate1 = std::make_shared<Inmate>("Bob");
        inmate1->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(8, 3).x, tile(8, 3).y, sf::Color(255, 160, 50));
        auto inmate2 = std::make_shared<Inmate>("Joe");
        inmate2->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(14, 3).x, tile(14, 3).y, sf::Color(255, 160, 50));
        auto inmate3 = std::make_shared<Inmate>("Mike");
        inmate3->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(20, 3).x, tile(20, 3).y, sf::Color(255, 130, 30));
        auto inmate4 = std::make_shared<Inmate>("Alex");
        inmate4->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(26, 3).x, tile(26, 3).y, sf::Color(255, 130, 30));
        auto inmate5 = std::make_shared<Inmate>("Tony");
        inmate5->InitGraphics(GameManager::GetAssetsPath() + "player.png", tile(32, 3).x, tile(32, 3).y, sf::Color(220, 110, 20));
        auto warden = std::make_shared<Warden>("The Warden");
        warden->InitGraphics(GameManager::GetAssetsPath() + "player.png", 20.f * 16.f, 20.f * 16.f, sf::Color(200, 50, 50));
        game.AddEntity(warden);
        game.AddEntity(player);
        game.AddEntity(guard1);
        game.AddEntity(guard2);
        game.AddEntity(guard3);
        game.AddEntity(inmate1);
        game.AddEntity(inmate2);
        game.AddEntity(inmate3);
        game.AddEntity(inmate4);
        game.AddEntity(inmate5);
        game.Run(window);
        /**
        Test deep copy
        GameManager gameCopy = game;
        GameManager gameAssign;
        gameAssign = game;
        */
    }
    catch (const GameException& e)
    {
        std::cerr << "Game Error: " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Standard Exception: " << e.what() << "\n";
    }
    return 0;
}
