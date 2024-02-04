#ifndef VEHICLE_H_
#define VEHICLE_H_

#include "Utils.h"

class Vehicle {
public:
    Vehicle() : m_Length(2) {
        m_Color = Color::GetString(Color::Background::YELLOW);
    }
    Vehicle(Color::Background color, int length) : m_Length(length) {
        m_Color = Color::GetString(color);
    }

    void SetLength(int length) {
        m_Length = length;
    }

    std::string GetColor() const {
        return m_Color;
    }

    int GetLength() const {
        return m_Length;
    }

    void paint() {

        int rand = Utils::Random(0, static_cast<int>(Color::Background::MAX) - 2);
        if (static_cast<int>(Color::Background::BLACK) <= rand) {
            rand++;
        }
        m_Color = Color::GetString(static_cast<Color::Background>(rand));
    }
private:
    std::string m_Color;
    int m_Length;
};

#endif /*VEHICLE_H_*/
