#ifndef STREET_H_
#define STREET_H_

#include "GameObject.h"
#include "Configuration.h"
#include "Vehicle.h"
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

class Street : public GameObject {
public:
    Street(int yCoordinate, bool direction) :
        m_Y(yCoordinate), m_Direction(direction) {
        std::string temp = Color::GetString(Color::Background::BLACK);
        for (int i = 0; i < PLAYGROUND_WIDTH + 2 * PLAYGROUND_OFFSET; i++) {
            m_Trafic.push_back(temp);
        }
    }

    void AddVehicle(const Vehicle& vehicle) {
        if (!IsEntranceClear(vehicle.GetLength()))
            return;

        int start = m_Direction ? 0 : m_Trafic.size() - vehicle.GetLength();
        int end = m_Direction ? vehicle.GetLength() : m_Trafic.size();
        std::string temp = vehicle.GetColor();

        for (int i = start; i < end; i++) {
            m_Trafic.at(i) = temp;
        }
    }

    bool IsEntranceClear(int lenght) {
        int start = m_Direction ? 0 : m_Trafic.size() - lenght;
        int end = m_Direction ? lenght : m_Trafic.size();
        for (int i = start; i < end; i++) {
            if (m_Trafic.at(i) != Color::GetString(Color::Background::BLACK))
                return false;
        }
        return true;
    }

    virtual std::vector<Coordinates> GetCollisionMap(Coordinates center, int radius) {
        std::vector<Coordinates> collisionMap;
        int distanceY = std::abs(center.y - m_Y);
        if (distanceY > radius)
            return collisionMap;

        // Iterate through the grid within the specified radius
        for (int i = std::max(PLAYGROUND_OFFSET, center.x - radius); i <= std::min(center.x + radius, PLAYGROUND_WIDTH + PLAYGROUND_OFFSET); ++i) {
            // Calculate the distance from the center
            int distance = std::sqrt(std::pow(center.x - i, 2) + std::pow(center.y - m_Y, 2));

            // Check if within radius and color is not black
            if (distance <= radius && m_Trafic.at(i) != Color::GetString(Color::Background::BLACK)) {
                collisionMap.push_back(Coordinates(i, m_Y));
            }
        }

        return collisionMap;
    }

    virtual void CollideWith(std::shared_ptr<GameObject> object) {

    }

    virtual void OnEvent() {

    }

    virtual void OnUpdate() {
        GenerateTraffic();
        auto rotationPivot = !m_Direction ? m_Trafic.begin() + 1 : m_Trafic.end() - 1;
        std::rotate(m_Trafic.begin(), rotationPivot, m_Trafic.end());
    }

    virtual void OnRender() {
        std::vector<std::string> tempVehicle = GetActive();
        std::string tempString = tempVehicle.at(0) + ' ';
        for (uint i = 1; i < tempVehicle.size(); i++) {
            if (tempVehicle.at(i) != tempVehicle.at(i - 1)) {
                tempString += tempVehicle.at(i) + ' ';
            }
            else {
                tempString += ' ';
            }
        }
        Utils::Draw(PLAYGROUND_OFFSET + 1, m_Y, tempString + "\033[0m");
    }

    std::vector<std::string> GetActive() {
        std::vector<std::string> temp;
        for (int i = PLAYGROUND_OFFSET + 1; i < PLAYGROUND_WIDTH + PLAYGROUND_OFFSET - 1; i++) {
            temp.push_back(m_Trafic.at(i));
        }
        return temp;
    }

    virtual inline Type GetType() { return Type::STREET; }
private:
    void GenerateTraffic() {
        if (Utils::Random(0, 100) < 20) {
            Vehicle vehicle;
            vehicle.paint();
            vehicle.SetLength(Utils::Random(2, 4));
            AddVehicle(vehicle);
        }
    }
private:
    std::vector<std::string> m_Trafic;
    int m_Y;
    bool m_Direction;
};

#endif /*STREET_H_*/
