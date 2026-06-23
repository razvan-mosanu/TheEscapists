#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "entity.h"
#include "prison_map.h"
#include "item.h"
#include "inventory.h"
#include "observer.h"
#include <vector>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

enum class GameState { Menu, Play, Stats, Crafting, Inventory, Win, Trade, Stash };
enum class Routine { MorningRollcall, Breakfast, FreeTime, Work, EveningRollcall, LightsOut };

class GameManager : public Observer {
private:
    std::vector<std::shared_ptr<Entity>> entities;
    PrisonMap map;
    sf::Font font;
    GameState state;
    
    Inventory m_playerStash{6};

    Routine currentRoutine;
    float dialogueTimer;
    float inGameTime; // in hours ex: 8.0f = 08:00
    float routinePenaltyTimer;
    std::string dialogueText;
    bool combatMode;
    int draggedItemIndex; // -1 if not dragging
    sf::Vector2f infirmaryPos; // respawn point

     // Full inventory screen
    bool showFullInventory;
    // for events
    Routine previousRoutine;
    bool rollcallEventTriggered;
    std::shared_ptr<class Inmate> tradingInmate;
    static const std::string ASSETS_PATH;

    void DrawMenu(sf::RenderWindow& window);
    void DrawHUD(sf::RenderWindow& window, sf::View& camera, const std::shared_ptr<class Player>& player);
    void DrawInventoryBar(sf::RenderWindow& window, const std::shared_ptr<class Player>& player);
    void DrawInventoryFull(sf::RenderWindow& window, const std::shared_ptr<class Player>& player);
    void DrawTrade(sf::RenderWindow& window, const std::shared_ptr<class Player>& player);
    void DrawStashMenu(sf::RenderWindow& window, const std::shared_ptr<class Player>& player);
    void DrawStats(sf::RenderWindow& window, const std::shared_ptr<class Player>& player);
    void DrawCrafting(sf::RenderWindow& window);
    void DrawZoneLabel(sf::RenderWindow& window, const std::shared_ptr<class Player>& player);

public:
    GameManager();
    ~GameManager() = default;
    // why copy and swap here?
    // if I implement a save system
    // pot face deep copy rapid si salva progresul
    GameManager(const GameManager& other);
    //funcție friend pentru swap
    friend void swap(GameManager& first, GameManager& second) noexcept;
    GameManager& operator=(GameManager other);
    static const std::string& GetAssetsPath();
    void AddEntity(const std::shared_ptr<Entity>& entity);
    bool Initialize();
    void Run(sf::RenderWindow& window);
    void OnNotify(Entity* entity, Event event) override;
};

#endif // GAME_MANAGER_H
