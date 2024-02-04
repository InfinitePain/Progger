#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <cstdlib>
#include <string>
#include <string_view>
#include <map>

class Color {
public:
    enum class Foreground : int {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        PURPLE,
        CYAN,
        LIGHT_GRAY,
        GRAY,
        LIGHT_RED,
        LIGHT_GREEN,
        LIGHT_YELLOW,
        LIGHT_BLUE,
        LIGHT_PURPLE,
        LIGHT_CYAN,
        WHITE,
        MAX
    };

    enum class Background : int {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        PURPLE,
        CYAN,
        LIGHT_GRAY,
        GRAY,
        LIGHT_RED,
        LIGHT_GREEN,
        LIGHT_YELLOW,
        LIGHT_BLUE,
        LIGHT_PURPLE,
        LIGHT_CYAN,
        WHITE,
        MAX
    };

    struct Pair {
        std::string foreground;
        std::string background;
        Pair(Foreground foreground, Background background) :
            foreground(GetString(foreground)), background(GetString(background)) {
        }
    };

    static std::string GetString(Foreground color) {
        if (color == Foreground::MAX) {
            throw std::invalid_argument("Color::GetString() does not accept Foreground::MAX as argument");
        }
        auto it = m_ForegroundMap.find(color);
        if (it != m_ForegroundMap.end())
            return it->second;

        std::string temp = "\033[";
        if (static_cast<int>(color) / 8 == 0)
            temp += "0;3";
        else
            temp += "0;9";
        temp += std::to_string(static_cast<int>(color) % 8) + 'm';
        m_ForegroundMap[color] = temp;
        return temp;
    }

    static std::string GetString(Background color) {
        if (color == Background::MAX) {
            throw std::invalid_argument("Color::GetString() does not accept Background::MAX as argument");
        }
        auto it = m_BackgroundMap.find(color);
        if (it != m_BackgroundMap.end())
            return it->second;

        std::string temp = "\033[";
        if (static_cast<int>(color) / 8 == 0)
            temp += "0;4";
        else
            temp += "0;10";
        temp += std::to_string(static_cast<int>(color) % 8) + 'm';
        m_BackgroundMap[color] = temp;
        return temp;
    }
private:
    Color() = delete;
    ~Color() = delete;
private:
    static std::map<Foreground, std::string> m_ForegroundMap;
    static std::map<Background, std::string> m_BackgroundMap;
};

namespace Utils
{
    // Returns a random number between min and max (both inclusive)
    int Random(int min, int max);
    // Draws a string at the given position with the given color
    void Draw(int x, int y, std::string_view string, Color::Pair colorPair);
    // Draws a string at the given position with no color modification
    void Draw(int x, int y, std::string_view string);
    void DrawRectangle(int x, int y, int width, int height, Color::Pair colorPair);
}

#endif /*UTILS_H_*/