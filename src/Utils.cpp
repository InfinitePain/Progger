#include "Utils.h"

std::map<Color::Foreground, std::string> Color::m_ForegroundMap;
std::map<Color::Background, std::string> Color::m_BackgroundMap;

int Utils::Random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void Utils::Draw(int x, int y, std::string_view string, Color::Pair colorPair) {
    std::cout
        // Set Foreground Color
        << colorPair.foreground
        // Set Background Color
        << colorPair.background
        // Move cursor
        << "\33[" << y << ";" << x << "H"
        // Draw string
        << string
        // Reset to default
        << "\033[0m"
        << std::flush;
}

void Utils::Draw(int x, int y, std::string_view string) {
    std::cout
        // Move cursor
        << "\33[" << y << ";" << x << "H"
        // Draw string
        << string
        << std::flush;
}

void Utils::DrawRectangle(int x, int y, int width, int height, Color::Pair colorPair) {
    std::string temp =
        // Top side
        std::string(width, ' ');

    // Sides
    for (int i = y + 1; i < y + height; ++i) {
        temp +=
            +"\033[" + std::to_string(i) + ';' + std::to_string(x) + 'H'
            + ' '
            // Move cursor
            + "\033[" + std::to_string(i) + ';' + std::to_string(x + width - 1) + 'H'
            + ' ';
    }

    // Bottom side
    temp +=
        // Move cursor
        "\033[" + std::to_string(y + height) + ';' + std::to_string(x) + 'H'
        + std::string(width, ' ');

    // Draw
    Draw(x, y, temp, colorPair);
}
