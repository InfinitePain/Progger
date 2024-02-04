#ifndef GAME_H_
#define GAME_H_

#include "Utils.h"
#include "Street.h"
#include "Configuration.h"
#include "Player.h"
#include "KeyEvent.h"
#include <string>
#include <string_view>
#include <vector>
#include <memory>

class Finish : public GameObject {
public:
    Finish(std::shared_ptr<Player> player) : m_Player(player) {}
    ~Finish() = default;

    virtual std::vector<Coordinates> GetCollisionMap(Coordinates coordinates, int radius) {
        std::vector<Coordinates> collisionMap;

        // Calculate distance between input coordinates and m_Coordinates
        int distance = std::sqrt(std::pow(coordinates.x - m_Coordinates.x, 2) + std::pow(coordinates.y - m_Coordinates.y, 2));

        // Check if m_Coordinates is outside the radius
        if (distance <= radius) {
            // If inside radius, add to collision map
            collisionMap.push_back(m_Coordinates);
        }

        // Return the collision map
        return collisionMap;
    }

    virtual void CollideWith(std::shared_ptr<GameObject> object) {

    }

    virtual void OnEvent() {
    }

    virtual void OnUpdate() {
    }

    virtual void OnRender() {
        Utils::Draw(m_Coordinates.x, m_Coordinates.y, std::string_view(&m_Symbol, 1), m_ColorPair);
    }

    bool IsReached() {
        return m_Player->GetX() == m_Coordinates.x && m_Player->GetY() == m_Coordinates.y;
    }

    virtual inline Type GetType() { return Type::GOAL; }
private:
    Coordinates m_Coordinates = Coordinates(
        PLAYGROUND_OFFSET + PLAYGROUND_WIDTH / 2,
        PLAYGROUND_OFFSET + 2);
    std::shared_ptr<Player> m_Player;
    char m_Symbol = 'F';
    Color::Pair m_ColorPair =
        Color::Pair(Color::Foreground::WHITE, Color::Background::LIGHT_YELLOW);
};

class Game {
public:
    Game() {
        // Empty Terminal
        std::cout << "\33[2J";
        // Hide Cursor
        std::cout << "\33[?25l";
    }
    ~Game() {
        // Show Cursor
        std::cout << "\33[?25h";
    }

    void Init() {
        srand(time(NULL));
        // Initialize Streets
        for (int i = PLAYGROUND_OFFSET + 4; i <= PLAYGROUND_OFFSET + PLAYGROUND_HEIGHT - 3; i++) {
            std::shared_ptr<Street> street = std::make_shared<Street>(i, i % 2 == 0);
            Vehicle vehicle;
            vehicle.paint();
            vehicle.SetLength(Utils::Random(2, 4));
            street->AddVehicle(vehicle);
            m_Streets.push_back(street);
            m_GameObjects.push_back(street);
        }
        // Initialize Player
        m_Player = std::make_shared<Player>();
        m_GameObjects.push_back(m_Player);
        // Initialize Finish
        m_Finish = std::make_shared<Finish>(m_Player);
        m_GameObjects.push_back(m_Finish);
    }

    bool run() {
        if (m_Player->IsAlive()) {
            Event();
            Update();
            CollisionCheck();
            Render();
            return true;
        }
        else {
            End();
            return false;
        }
    }

    void Event() {
        while (!EventQueue::Get().IsEmpty()) {
            for (auto& object : m_GameObjects) {
                object->OnEvent();
            }
        }
    }

    void Update() {
        for (auto& object : m_GameObjects) {
            object->OnUpdate();
        }
    }

    void CollisionCheck() {
        int radius = 1;
        GameObject::Coordinates playerCoordinates = GameObject::Coordinates(m_Player->GetX(), m_Player->GetY());
        for (uint i = 0; i < m_GameObjects.size(); i++) {
            std::vector<GameObject::Coordinates> comp1 =
                m_GameObjects.at(i)->GetCollisionMap(playerCoordinates, radius);

            for (uint j = i + 1; j < m_GameObjects.size(); j++) {
                std::vector<GameObject::Coordinates> comp2 =
                    m_GameObjects.at(j)->GetCollisionMap(playerCoordinates, radius);

                for (auto& coord1 : comp1) {
                    for (auto& coord2 : comp2) {
                        if (coord1.x == coord2.x && coord1.y == coord2.y) {
                            m_GameObjects.at(i)->CollideWith(m_GameObjects.at(j));
                            m_GameObjects.at(j)->CollideWith(m_GameObjects.at(i));
                        }
                    }
                }
            }
        }

    }

    void Render() {
        // Draw Game Logo
        Utils::Draw(PLAYGROUND_WIDTH + 2 * PLAYGROUND_OFFSET + 6,
            7,
            "PROGGER");

        // Draw Game Frame
        Utils::DrawRectangle(
            PLAYGROUND_OFFSET,
            PLAYGROUND_OFFSET,
            PLAYGROUND_WIDTH,
            PLAYGROUND_HEIGHT,
            Color::Pair(Color::Foreground::BLACK, Color::Background::LIGHT_BLUE));

        // Draw Game Stats
        Utils::DrawRectangle(
            PLAYGROUND_WIDTH + 2 * PLAYGROUND_OFFSET + 3,
            PLAYGROUND_OFFSET,
            15,
            6,
            Color::Pair(Color::Foreground::BLACK, Color::Background::LIGHT_RED));
        Utils::Draw(
            PLAYGROUND_WIDTH + 2 * PLAYGROUND_OFFSET + 5,
            PLAYGROUND_OFFSET + 2,
            "Points :  " + std::to_string(m_Player->GetPoints()));
        Utils::Draw(
            PLAYGROUND_WIDTH + 2 * PLAYGROUND_OFFSET + 5,
            PLAYGROUND_OFFSET + 3,
            "Lives :   " + std::to_string(m_Player->GetLives()));
        for (auto& object : m_GameObjects) {
            object->OnRender();
        }
    }

    void Restart() {
        // Empty Terminal
        std::cout << "\33[2J";
        // Clear existing game objects and streets
        m_GameObjects.clear();
        m_Streets.clear();

        // Reinitialize the game
        Init();
    }

    void End() {
        std::cout << "\33[2J";
        Utils::Draw(5, 5, "You have reached " + std::to_string(m_Player->GetPoints()) + " points!");
        Utils::Draw(5, 6, "Press esc to exit");
        Utils::Draw(5, 7, "Press any other key to restart");
    }

    inline bool IsRunning() { return m_Player->IsAlive(); }
private:
    std::shared_ptr<Player> m_Player;
    std::shared_ptr<Finish> m_Finish;
    std::vector<std::shared_ptr<Street>> m_Streets;
    std::vector<std::shared_ptr<GameObject>> m_GameObjects;
};

#endif /*GAME_H_*/
