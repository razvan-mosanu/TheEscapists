#include "game_manager.h"
#include "exceptions.h"
#include "guard.h"
#include "inmate.h"
#include "player.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

const std::string GameManager::ASSETS_PATH = "assets/";

GameManager::GameManager()
    : state(GameState::Menu), currentRoutine(Routine::MorningRollcall),
    dialogueTimer(0.f), inGameTime(8.0f), routinePenaltyTimer(0.f),
    combatMode(false), draggedItemIndex(-1), showFullInventory(false),
    previousRoutine(Routine::LightsOut), rollcallEventTriggered(false) {}

//Deep Copy
GameManager::GameManager(const GameManager& other)
    : map(other.map), font(other.font), state(other.state),
    currentRoutine(other.currentRoutine), dialogueTimer(other.dialogueTimer),
    inGameTime(other.inGameTime), routinePenaltyTimer(other.routinePenaltyTimer),
    dialogueText(other.dialogueText), combatMode(other.combatMode),
    draggedItemIndex(other.draggedItemIndex), infirmaryPos(other.infirmaryPos),
    showFullInventory(other.showFullInventory), previousRoutine(other.previousRoutine),
    rollcallEventTriggered(other.rollcallEventTriggered), searchedCell(other.searchedCell)
{
    for (const auto& entity : other.entities)
        if (entity) this->entities.push_back(entity->Clone()); //se apeleaza Clone() pentru a face o copie
}

void swap(GameManager& first, GameManager& second) noexcept
{
    std::swap(first.entities, second.entities);
    std::swap(first.map, second.map); //swap prin valoare
    std::swap(first.font, second.font);
    std::swap(first.state, second.state);
    std::swap(first.currentRoutine, second.currentRoutine);
    std::swap(first.dialogueTimer, second.dialogueTimer);
    std::swap(first.inGameTime, second.inGameTime);
    std::swap(first.routinePenaltyTimer, second.routinePenaltyTimer);
    std::swap(first.dialogueText, second.dialogueText);
    std::swap(first.combatMode, second.combatMode);
    std::swap(first.draggedItemIndex, second.draggedItemIndex);
    std::swap(first.infirmaryPos, second.infirmaryPos);
    std::swap(first.showFullInventory, second.showFullInventory);
    std::swap(first.previousRoutine, second.previousRoutine);
    std::swap(first.rollcallEventTriggered, second.rollcallEventTriggered);
    std::swap(first.searchedCell, second.searchedCell);
}

GameManager& GameManager::operator=(GameManager other)
{
    swap(*this, other);
    return *this;
}


void GameManager::AddEntity(const std::shared_ptr<Entity> &entity)
{
    entities.push_back(entity);
}

bool GameManager::Initialize()
{
    if (!map.Load(ASSETS_PATH + "prison_map.tmj", ASSETS_PATH + "tileset.png"))
        throw ResourceLoadException(ASSETS_PATH + "prison_map.tmj");
    if (!font.loadFromFile(ASSETS_PATH + "arial.ttf"))
        std::cerr << "Warning: Could not load Arial font. UI text will not display.\n";
    infirmaryPos = sf::Vector2f(20.f * 16.f, 23.f * 16.f);
    return true;
}

void GameManager::DrawMenu(sf::RenderWindow &window)
{
    //fixed full screen problem
    sf::View uiView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(uiView);

    window.clear(sf::Color(30, 30, 30));
    sf::Text title("THE ESCAPISTS", font, 40);
    title.setFillColor(sf::Color::White);
    title.setPosition(400.f - title.getGlobalBounds().width / 2.f, 200.f);
    sf::Text prompt("Press ENTER to Start\nPress ESC to Quit\nPress S for Controls", font, 24);
    prompt.setFillColor(sf::Color(200, 200, 200));
    prompt.setPosition(400.f - prompt.getGlobalBounds().width / 2.f, 280.f);
    window.draw(title);
    window.draw(prompt);
}

void GameManager::DrawHUD(sf::RenderWindow &window, sf::View &camera, const std::shared_ptr<Player>& player)
{
    if (!player) return;

    //fixed full screen problem
    sf::View uiView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(uiView);

    sf::RectangleShape hudBg(sf::Vector2f(240.f, 150.f));
    hudBg.setFillColor(sf::Color(15, 15, 25, 120));
    hudBg.setOutlineColor(sf::Color(80, 80, 120));
    hudBg.setOutlineThickness(2.f);
    hudBg.setPosition(8.f, 8.f);
    window.draw(hudBg);

    sf::Text nameText(player->GetName(), font, 14);
    nameText.setFillColor(sf::Color(220, 220, 255));
    nameText.setPosition(16.f, 14.f);
    window.draw(nameText);

    //Hp
    sf::Text hpLabel("HP", font, 11);
    hpLabel.setFillColor(sf::Color(255, 100, 100));
    hpLabel.setPosition(16.f, 36.f);
    window.draw(hpLabel);
    sf::RectangleShape hpBg(sf::Vector2f(160.f, 11.f));
    hpBg.setFillColor(sf::Color(50, 0, 0));
    hpBg.setPosition(50.f, 37.f);

    short curHp = player->GetHealth();
    short maxHp = player->GetMaxHealth();
    float hpW = (maxHp > 0) ? std::clamp(static_cast<float>(curHp) / static_cast<float>(maxHp), 0.f, 1.f) * 160.f : 0.f;

    sf::RectangleShape hpFill(sf::Vector2f(hpW, 11.f));
    hpFill.setPosition(50.f, 37.f);
    hpFill.setFillColor(curHp < (maxHp / 3) ? sf::Color(255, 50, 50) : sf::Color(220, 30, 30));
    window.draw(hpBg);
    window.draw(hpFill);
    sf::Text hpNum(std::to_string(curHp) + "/" + std::to_string(maxHp), font, 10);
    hpNum.setFillColor(sf::Color(255, 200, 200));
    hpNum.setPosition(215.f, 37.f);
    window.draw(hpNum);

    // Heat Bar
    sf::Text heatLabel("HEAT", font, 11);
    short heat = player->GetHeat();
    heatLabel.setFillColor(heat > 80 ? sf::Color(255, 50, 50) : sf::Color(255, 180, 0));
    heatLabel.setPosition(16.f, 55.f);
    window.draw(heatLabel);
    sf::RectangleShape heatBg(sf::Vector2f(160.f, 11.f));
    heatBg.setFillColor(sf::Color(40, 30, 0));
    heatBg.setPosition(50.f, 56.f);
    float heatW = static_cast<float>(heat) / 100.f * 160.f;
    sf::RectangleShape heatFill(sf::Vector2f(heatW, 11.f));
    heatFill.setPosition(50.f, 56.f);
    heatFill.setFillColor(heat > 80 ? sf::Color(255, 50, 50) : sf::Color(255, 165, 0));
    window.draw(heatBg);
    window.draw(heatFill);

    // Money
    sf::Text moneyText("$  " + std::to_string(player->GetMoney()), font, 13);
    moneyText.setFillColor(sf::Color(100, 230, 100));
    moneyText.setPosition(16.f, 75.f);
    window.draw(moneyText);

    // Time + Routine
    std::string routineStr;
    switch (currentRoutine)
    {
    case Routine::MorningRollcall: routineStr = "Rollcall"; break;
    case Routine::Breakfast: routineStr = "Breakfast"; break;
    case Routine::FreeTime: routineStr = "Free Time"; break;
    case Routine::Work: routineStr = "Work"; break;
    case Routine::EveningRollcall: routineStr = "Rollcall"; break;
    case Routine::LightsOut: routineStr = "Lights Out"; break;
    }

    int h = static_cast<int>(inGameTime);
    int m = static_cast<int>((inGameTime - static_cast<float>(h)) * 60.0f);
    std::string timeStr = (h < 10 ? "0" : "") + std::to_string(h) + ":" + (m < 10 ? "0" : "") + std::to_string(m);
    sf::Text timeText("TIME  " + timeStr + "  [" + routineStr + "]", font, 12);
    timeText.setFillColor(sf::Color(180, 180, 255));
    timeText.setPosition(16.f, 95.f);
    window.draw(timeText);

    if (combatMode)
    {
        sf::Text combatText("Press B: Punch", font, 12);
        combatText.setFillColor(sf::Color(255, 50, 50));
        combatText.setPosition(16.f, 115.f);
        window.draw(combatText);

        float atkCD = player->GetAttackTimer();
        if (atkCD > 0.f)
        {
            float maxCd = 1.0f;
            float barW = std::clamp(atkCD / maxCd, 0.f, 1.f) * 80.f;
            sf::RectangleShape atkBg(sf::Vector2f(80.f, 6.f));
            atkBg.setFillColor(sf::Color(60, 20, 20));
            atkBg.setPosition(16.f, 130.f);
            sf::RectangleShape atkFill(sf::Vector2f(80.f - barW, 6.f));
            atkFill.setFillColor(sf::Color(255, 200, 0));
            atkFill.setPosition(16.f, 130.f);
            window.draw(atkBg);
            window.draw(atkFill);
            sf::Text atkLabel("ATK RDY", font, 9);
            atkLabel.setFillColor(sf::Color(180, 180, 0));
            atkLabel.setPosition(100.f, 129.f);
            window.draw(atkLabel);
        }
        else
        {
            sf::Text atkReady("Ready to Punch", font, 11);
            atkReady.setFillColor(sf::Color(80, 255, 80));
            atkReady.setPosition(16.f, 128.f);
            window.draw(atkReady);
        }
    }
    else
    {
        sf::Text controlsText("I:Inv  C:Craft  Spc:Fight", font, 10);
        controlsText.setFillColor(sf::Color(130, 130, 130));
        controlsText.setPosition(16.f, 118.f);
        window.draw(controlsText);
    }

    if (dialogueTimer > 0.f)
    {
        sf::RectangleShape dialogBox(sf::Vector2f(620.f, 60.f));
        dialogBox.setFillColor(sf::Color(10, 10, 20, 220));
        dialogBox.setOutlineColor(sf::Color(120, 120, 200));
        dialogBox.setOutlineThickness(2.f);
        //fixed overlaping problem
        dialogBox.setPosition(90.f, 450.f);
        window.draw(dialogBox);
        sf::Text dText(dialogueText, font, 16);
        dText.setFillColor(sf::Color(230, 230, 255));
        dText.setPosition(105.f, 464.f);
        window.draw(dText);
    }
    DrawZoneLabel(window, player);
    if (state == GameState::Inventory || showFullInventory) DrawInventoryFull(window, player);
    else DrawInventoryBar(window, player);
    window.setView(camera);
}

void GameManager::DrawZoneLabel(sf::RenderWindow& window, const std::shared_ptr<class Player>& player)
{
    if (!player) return;
    std::string zoneName = map.GetZoneAt(player->GetPosition().x + 8.f, player->GetPosition().y + 12.f);
    sf::Text zoneText("Zone: " + zoneName, font, 14);
    zoneText.setFillColor(sf::Color(255, 255, 200));
    //fixed full screen problem
    zoneText.setPosition(800.f - 150.f, 10.f);
    sf::RectangleShape bg(sf::Vector2f(140.f, 25.f));
    bg.setFillColor(sf::Color(0, 0, 0, 150));
    bg.setPosition(800.f - 155.f, 5.f);
    window.draw(bg);
    window.draw(zoneText);
}

void GameManager::DrawInventoryBar(sf::RenderWindow &window, const std::shared_ptr<Player>& player)
{
    if (!player) return;
    bool isFullInventory = (state == GameState::Inventory);
    //new positions
    float startX = isFullInventory ? 230.f : 220.f;
    float startY = isFullInventory ? 220.f : 530.f;
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    auto items = player->GetInventory().GetItems();
    int maxSlots = isFullInventory ? player->GetInventory().GetCapacity() : 6;

    for (int i = 0; i < maxSlots; ++i)
    {
        int col = i % 3;
        int row = i / 3;
        float x = isFullInventory ? startX + static_cast<float>(col) * 120.f : startX + static_cast<float>(i) * 60.f;
        float y = isFullInventory ? startY + static_cast<float>(row) * 80.f : startY;

        sf::RectangleShape slot(sf::Vector2f(50.f, 50.f));
        slot.setPosition(x, y);

        if (i == draggedItemIndex) slot.setFillColor(sf::Color(100, 100, 50, 200));
        else slot.setFillColor(sf::Color(50, 50, 50, 200));

        slot.setOutlineColor(sf::Color::White);
        slot.setOutlineThickness(2.f);
        window.draw(slot);

        sf::Text keyText(std::to_string(i + 1), font, 14);
        keyText.setPosition(x + 5.f, y + 5.f);
        window.draw(keyText);

        if (i < static_cast<int>(items.size()))
        {
            std::string name = items[i].GetName();
            if (name.length() > 5) name = name.substr(0, 5) + ".";
            sf::Text itemText(name, font, 12);
            itemText.setFillColor(items[i].IsContraband() ? sf::Color::Red : sf::Color::White);

            if (i == draggedItemIndex)
                itemText.setPosition(mousePos.x - 20.f, mousePos.y - 10.f);
            else
                itemText.setPosition(x + 5.f, y + 25.f);
            window.draw(itemText);
        }
    }
}

void GameManager::DrawInventoryFull(sf::RenderWindow& window, const std::shared_ptr<class Player>& player)
{
    if (!player) return;
    sf::RectangleShape bg(sf::Vector2f(400.f, 300.f));
    bg.setFillColor(sf::Color(20, 20, 30, 240));
    bg.setOutlineColor(sf::Color(100, 100, 150));
    bg.setOutlineThickness(3.f);
    bg.setPosition(200.f, 150.f);
    window.draw(bg);

    sf::Text title("INVENTORY", font, 24);
    title.setPosition(330.f, 160.f);
    window.draw(title);

    float startX = 230.f;
    float startY = 220.f;
    auto items = player->GetInventory().GetItems();
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (int i = 0; i < player->GetInventory().GetCapacity(); ++i)
    {
        int col = i % 3;
        int row = i / 3;
        float x = startX + static_cast<float>(col) * 120.f;
        float y = startY + static_cast<float>(row) * 80.f;

        sf::RectangleShape slot(sf::Vector2f(100.f, 60.f));
        slot.setPosition(x, y);
        slot.setFillColor(i == draggedItemIndex ? sf::Color(100, 100, 50, 200) : sf::Color(50, 50, 50, 200));
        slot.setOutlineColor(sf::Color::White);
        slot.setOutlineThickness(1.f);
        window.draw(slot);

        if (i < static_cast<int>(items.size()))
        {
            sf::Text itemText(items[i].GetName(), font, 14);
            itemText.setFillColor(items[i].IsContraband() ? sf::Color(255, 100, 100) : sf::Color::White);
            if (i == draggedItemIndex)
                itemText.setPosition(mousePos.x - 20.f, mousePos.y - 10.f);
            else
                itemText.setPosition(x + 5.f, y + 20.f);
            window.draw(itemText);
        }
    }
}

void GameManager::DrawCrafting(sf::RenderWindow& window)
{
    sf::View uiView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(uiView);
    sf::RectangleShape bg(sf::Vector2f(800.f, 600.f));
    bg.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(bg);
    sf::Text title("CRAFTING MENU", font, 40);
    title.setPosition(400.f - title.getGlobalBounds().width / 2.f, 100.f);
    window.draw(title);
    sf::Text help("Press ESC to return.", font, 24);
    help.setPosition(200.f, 200.f);
    window.draw(help);
}

void GameManager::DrawStats(sf::RenderWindow& window, const std::shared_ptr<class Player>& player)
{
    sf::View uiView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
    window.setView(uiView);
    sf::RectangleShape bg(sf::Vector2f(800.f, 600.f));
    bg.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(bg);
    sf::Text title("PLAYER STATS", font, 40);
    title.setPosition(400.f - title.getGlobalBounds().width / 2.f, 100.f);
    window.draw(title);
    if (player)
    {
        sf::Text stats("Heat: " + std::to_string(player->GetHeat()) + "%\n"
        "Health: " + std::to_string(player->GetHealth()) + "\n"
        "Money: " + std::to_string(player->GetMoney()) + "$\n\nPress ESC to return.", font, 24);
        stats.setPosition(250.f, 200.f);
        window.draw(stats);
    }
}

void GameManager::Run(sf::RenderWindow &window)
{
    const float VIEW_W = 400.f;
    const float VIEW_H = 250.f;
    sf::View camera(sf::FloatRect(0.f, 0.f, VIEW_W, VIEW_H));

    sf::Clock clock;
    bool showControls = false;
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (state == GameState::Menu)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Enter) state = GameState::Play;
                    if (event.key.code == sf::Keyboard::S) showControls = !showControls;
                    if (event.key.code == sf::Keyboard::Escape) window.close();
                }
            }
            else if (state == GameState::Stats || state == GameState::Crafting || state == GameState::Inventory)
            {
                if (event.type == sf::Event::KeyPressed &&
                    (event.key.code == sf::Keyboard::Escape ||
                     event.key.code == sf::Keyboard::I ||
                     event.key.code == sf::Keyboard::C))
                {
                    state = GameState::Play;
                    showFullInventory = false;
                }
            }
            else if (state == GameState::Play)
            {
                std::shared_ptr<Player> playerPtr = nullptr;
                for (auto &entity : entities)
                    if (auto p = std::dynamic_pointer_cast<Player>(entity)) playerPtr = p;

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Q) state = GameState::Stats;
                    else if (event.key.code == sf::Keyboard::C)
                    {
                        if (playerPtr->GetInventory().GetCount() < 2)
                            throw CraftingException("Not enough materials to craft anything!");
                        state = GameState::Crafting;
                    }
                    else if (event.key.code == sf::Keyboard::I)
                    {
                        state = GameState::Inventory;
                        showFullInventory = true;
                    }
                    else if (event.key.code == sf::Keyboard::Space)
                    {
                        combatMode = !combatMode;
                        dialogueText = combatMode ? "Combat Mode ON!" : "Combat Mode OFF!";
                        dialogueTimer = 2.0f;
                    }
                    else if (playerPtr)
                    {
                        if (combatMode && event.key.code == sf::Keyboard::B)
                        {
                            if (playerPtr->GetAttackTimer() > 0.f)
                            {
                                dialogueText = "Attack on cooldown! Wait...";
                                dialogueTimer = 0.5f;
                            }
                            else
                            {
                                bool hitSomeone = false;
                                for (auto &entity : entities)
                                {
                                    if (entity == playerPtr || entity->IsKnockedOut()) continue;
                                    sf::Vector2f playerCenter = playerPtr->GetCenter();
                                    sf::Vector2f entityCenter = entity->GetCenter();
                                    auto dist =  static_cast<float>(std::sqrt(std::pow(entityCenter.x - playerCenter.x, 2) +
                                                                             std::pow(entityCenter.y - playerCenter.y, 2)));
                                    if (dist < 20.f)
                                    {
                                        auto dmg = static_cast<short>(playerPtr->GetPower() / 2);
                                        entity->TakeDamage(dmg);
                                        playerPtr->SetAttackCooldown(1.0f);
                                        playerPtr->SetCombatActive();
                                        if (auto inmate = std::dynamic_pointer_cast<Inmate>(entity))
                                        {
                                            if (!entity->IsKnockedOut()) inmate->SetAggro(playerPtr);
                                            dialogueText = entity->IsKnockedOut()
                                                               ? inmate->GetName() + " is knocked out!"
                                                               : "You punched " + inmate->GetName() + "! (-" + std::to_string(dmg) + " HP)";
                                        }
                                        else if (auto guard = std::dynamic_pointer_cast<Guard>(entity))
                                        {
                                            if (!entity->IsKnockedOut()) guard->SetAggro(playerPtr);
                                            playerPtr->SetHeat(100);
                                            dialogueText = entity->IsKnockedOut()
                                                               ? guard->GetName() + " is knocked out!"
                                                               : "You punched " + guard->GetName() + "! HEAT MAX!";
                                        }
                                        dialogueTimer = 2.f;
                                        hitSomeone = true;
                                        break;
                                    }
                                }
                                if (!hitSomeone)
                                {
                                    dialogueText = "You swing at the air!";
                                    dialogueTimer = 1.0f;
                                }
                            }
                        }
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::View uiView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
                    sf::Vector2f mousePosScreen = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);

                    bool isFullInventory = (state == GameState::Inventory);
                    float startX = isFullInventory ? 230.f : 220.f;
                    float startY = isFullInventory ? 220.f : 530.f;
                    int maxSlots = isFullInventory && playerPtr ? playerPtr->GetInventory().GetCapacity() : 6;

                    for (int i = 0; i < maxSlots; ++i)
                    {
                        int col = i % 3;
                        int row = i / 3;
                        float x = isFullInventory ? startX + static_cast<float>(col) * 120.f : startX + static_cast<float>(i) * 60.f;
                        float y = isFullInventory ? startY + static_cast<float>(row) * 80.f : startY;
                        float w = isFullInventory ? 100.f : 50.f;
                        float h = isFullInventory ? 60.f : 50.f;

                        sf::FloatRect slotBounds(x, y, w, h);
                        if (slotBounds.contains(mousePosScreen.x, mousePosScreen.y))
                        {
                            if (playerPtr && i < static_cast<int>(playerPtr->GetInventory().GetItems().size())) draggedItemIndex = i;
                            break;
                        }
                    }
                }
                else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                {
                    if (draggedItemIndex != -1 && playerPtr)
                    {
                        sf::View uiView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
                        sf::Vector2f mousePosScreen = window.mapPixelToCoords(sf::Mouse::getPosition(window), uiView);

                        bool isFullInventory = (state == GameState::Inventory);
                        float startX = isFullInventory ? 230.f : 220.f;
                        float startY = isFullInventory ? 220.f : 530.f;
                        int maxSlots = isFullInventory ? playerPtr->GetInventory().GetCapacity() : 6;

                        for (int i = 0; i < maxSlots; ++i)
                        {
                            int col = i % 3;
                            int row = i / 3;
                            float x = isFullInventory ? startX + static_cast<float>(col) * 120.f : startX + static_cast<float>(i) * 60.f;
                            float y = isFullInventory ? startY + static_cast<float>(row) * 80.f : startY;
                            float w = isFullInventory ? 100.f : 50.f;
                            float h = isFullInventory ? 60.f : 50.f;

                            sf::FloatRect slotBounds(x, y, w, h);
                            if (slotBounds.contains(mousePosScreen.x, mousePosScreen.y))
                            {
                                if (i != draggedItemIndex && i < static_cast<int>(playerPtr->GetInventory().GetItems().size()))
                                {
                                    auto items = playerPtr->GetInventory().GetItems();
                                    Item temp = items[draggedItemIndex];
                                    items[draggedItemIndex] = items[i];
                                    items[i] = temp;
                                    playerPtr->GetInventory().SetItems(items);
                                }
                                break;
                            }
                        }
                        draggedItemIndex = -1;
                    }
                }
            }
        }
        if (state == GameState::Menu)
        {
            DrawMenu(window);
            if (showControls)
            {
                sf::View uiView(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
                window.setView(uiView);

                sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
                overlay.setFillColor(sf::Color(10, 10, 15, 240));
                window.draw(overlay);
                sf::Text ctrlTitle("HOW TO PLAY", font, 32);
                ctrlTitle.setFillColor(sf::Color(255, 200, 0));
                ctrlTitle.setPosition(400.f - ctrlTitle.getGlobalBounds().width / 2.f, 60.f);
                window.draw(ctrlTitle);
                sf::Text ctrlText(
                    "W, A, S, D - Move your character\n\n"
                    "Space - Toggle Combat Mode ON / OFF\n\n"
                    "B - Punch (You must be in Combat Mode)\n\n"
                    "I - Open Full Inventory\n\n"
                    "Q - View Character Stats\n\n"
                    "C - Open Crafting Menu\n\n"
                    "Left Click - Drag & Drop items in your inventory\n\n\n"
                    "          (Press 'S' again to close)", font, 18);
                ctrlText.setFillColor(sf::Color(220, 220, 230));
                ctrlText.setPosition(180.f, 130.f);
                window.draw(ctrlText);
            }
            window.display();
            continue;
        }

        if (state == GameState::Stats || state == GameState::Crafting)
        {
            if (state == GameState::Stats)
            {
                std::shared_ptr<Player> playerPtr = nullptr;
                for (auto &entity : entities)
                    if (auto p = std::dynamic_pointer_cast<Player>(entity)) playerPtr = p;
                DrawStats(window, playerPtr);
            }
            else DrawCrafting(window);
            window.display();
            continue;
        }
        if (dialogueTimer > 0.f) dialogueTimer -= deltaTime;
        if (state == GameState::Play || state == GameState::Inventory)
        {
            inGameTime += deltaTime * (1.0f / 60.0f);
            if (inGameTime >= 24.0f) inGameTime -= 24.0f;
            if      (inGameTime >= 8.0f && inGameTime < 9.0f  ) currentRoutine = Routine::MorningRollcall;
            else if (inGameTime >= 9.0f && inGameTime < 10.0f ) currentRoutine = Routine::Breakfast;
            else if (inGameTime >= 10.0f && inGameTime < 13.0f) currentRoutine = Routine::FreeTime;
            else if (inGameTime >= 13.0f && inGameTime < 17.0f) currentRoutine = Routine::Work;
            else if (inGameTime >= 17.0f && inGameTime < 22.0f) currentRoutine = Routine::EveningRollcall;
            else                                                currentRoutine = Routine::LightsOut;

            std::shared_ptr<Player> playerPtr = nullptr;
            for (auto &entity : entities)
                if (auto p = std::dynamic_pointer_cast<Player>(entity))  playerPtr = p;
            for (auto &entity : entities)
            {
                if (state == GameState::Play)entity->Update(deltaTime, map);
                if (auto guard = std::dynamic_pointer_cast<Guard>(entity))
                {
                    if (playerPtr && playerPtr->GetHeat() >= 80 && !guard->IsKnockedOut()) guard->SetAggro(playerPtr);
                }
            }

            if (playerPtr && playerPtr->GetHealth() <= 0)
            {
                playerPtr->TakeBeating();
                playerPtr->SetPosition(infirmaryPos);
                playerPtr->SetHeat(0);
                dialogueText = "Knocked out! You wake up in the infirmary...";
                dialogueTimer = 4.0f;
                for (auto &e : entities)
                    e->ClearAggro();
            }

            if (playerPtr && state == GameState::Play)
            {
                sf::Vector2f playerPos = playerPtr->GetPosition();
                float camHalfWidth = camera.getSize().x / 2.f;
                float camHalfHeight = camera.getSize().y / 2.f;
                float camX = std::max(camHalfWidth, std::min(playerPos.x, static_cast<float>(map.GetWidthInPixels()) - camHalfWidth));
                float camY = std::max(camHalfHeight, std::min(playerPos.y, static_cast<float>(map.GetHeightInPixels()) - camHalfHeight));
                camera.setCenter(camX, camY);
                window.setView(camera);
            }
        }
        window.clear(sf::Color::Black);
        window.draw(map);
        std::shared_ptr<Player> playerPtr = nullptr;
        for (auto &entity : entities)
        {
            if (auto p = std::dynamic_pointer_cast<Player>(entity)) playerPtr = p;
            if (entity->IsKnockedOut())
            {
                sf::Sprite koSprite = entity->GetSprite();
                koSprite.setColor(sf::Color(255, 255, 255, 80));
                window.draw(koSprite);
                sf::Vector2f pos = entity->GetPosition();
                float timeLeft = entity->GetKnockoutTimer();
                sf::Text koText("ZZZ " + std::to_string(static_cast<int>(timeLeft)) + "s", font, 9);
                koText.setFillColor(sf::Color(200, 200, 255));
                koText.setPosition(pos.x - 4.f, pos.y - 14.f);
                window.draw(koText);
            }
            else if (entity->GetHealth() > 0)
            {
                entity->Draw(window);
                short hp = entity->GetHealth();
                short maxHp = entity->GetMaxHealth();
                if (0 < hp && hp < maxHp)
                {
                    sf::Vector2f pos = entity->GetPosition();
                    sf::RectangleShape bg(sf::Vector2f(20.f, 4.f));
                    bg.setPosition(pos.x - 2.f, pos.y - 6.f);
                    bg.setFillColor(sf::Color::Black);

                    sf::RectangleShape fill(sf::Vector2f(static_cast<float>(hp) / static_cast<float>(maxHp) * 20.f, 4.f));
                    fill.setPosition(pos.x - 2.f, pos.y - 6.f);
                    fill.setFillColor(sf::Color(220, 30, 30));

                    window.draw(bg);
                    window.draw(fill);
                }
            }
        }
        DrawHUD(window, camera, playerPtr);
        window.display();
    }
}
