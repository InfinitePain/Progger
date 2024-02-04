#ifndef SPIELER_H
#define SPIELER_H

#include "GameObject.h"
#include "Utils.h"
#include "Configuration.h"
#include "KeyEvent.h"
#include <string>
#include <cmath>

class Player : public GameObject {
public:
    Player() :
        m_Color(Color::GetString(Color::Background::YELLOW)) {
    }

    Player(Color::Foreground color, char symbol, int x, int y) :
        m_Symbol(symbol), m_Coordinates(x, y) {
        m_Color = Color::GetString(color);
    }

    ~Player() = default;

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
        switch (object->GetType()) {
        case GameObject::Type::STREET:
            ReduceLives();
            ResetPosition();
            break;
        case GameObject::Type::GOAL:
            AddPoints(10);
            ResetPosition();
            break;
        default:
            break;
        }
    }

    virtual void OnEvent() {
        std::unique_ptr<KeyEvent> event = EVENT_POP();
        if (event != nullptr) {
            _Move(event.get()->GetKey());
        }
    }

    virtual void OnUpdate() {
    }

    virtual void OnRender() {
        Utils::Draw(m_Coordinates.x, m_Coordinates.y, m_Color + m_Symbol + "\033[0m");
    }

    inline void SetPosition(int x, int y) {
        m_Coordinates.x = x;
        m_Coordinates.y = y;
    }

    inline void ResetPosition() {
        m_Coordinates.x = PLAYGROUND_OFFSET + PLAYGROUND_WIDTH / 2;
        m_Coordinates.y = PLAYGROUND_OFFSET + PLAYGROUND_HEIGHT - 2;
    }

    virtual inline Type GetType() { return Type::PLAYER; }
    inline int GetX() { return m_Coordinates.x; }
    inline int GetY() { return m_Coordinates.y; }
    inline void AddPoints(int points) { m_Points += points; }
    inline void ReduceLives() { m_Lives--; }
    inline int GetLives() { return m_Lives; }
    inline int GetPoints() { return m_Points; }
    inline bool IsAlive() { return m_Lives > 0; }
private:
    bool _Move(const char* direction) {
        int tempX = m_Coordinates.x;
        int tempY = m_Coordinates.y;
        switch (direction[0]) {
        case UP:
            m_Coordinates.y--;
            break;
        case DOWN:
            m_Coordinates.y++;
            break;
        case RIGHT:
            m_Coordinates.x++;
            break;
        case LEFT:
            m_Coordinates.x--;
            break;
        default:
            return false;
        }
        Utils::Draw(tempX, tempY, " ");
        return true;
    }
private:
    char m_Symbol = 'P';
    Coordinates m_Coordinates = Coordinates(
        PLAYGROUND_OFFSET + PLAYGROUND_WIDTH / 2,
        PLAYGROUND_OFFSET + PLAYGROUND_HEIGHT - 2);
    int m_Points = 0;
    int m_Lives = 3;
    std::string m_Color;
};

#endif 
