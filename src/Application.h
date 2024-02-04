#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Game.h"
#include "InputHandler.h"
#include "KeyEvent.h"
#include <iostream>
#include <condition_variable>
#include <mutex>

class Application {
public:
    Application() {
        std::cout << "\33[?25l";

        m_Game.Init();

        m_InputHandler.Init();
        m_InputHandler.SetCallback([this](char* c) {
            std::unique_ptr<KeyEvent> event = std::make_unique<KeyEvent>(c);
            EVENT_PUSH(std::move(event));
            if (STOP != c[0]) {
                this->m_CV.notify_one();
                return true;
            }
            std::lock_guard<std::mutex> lock(m_MutexRunning);
            this->m_Running = false;
            this->m_CV.notify_one();
            return true;
            });
    }
    ~Application() = default;
    
    void run() {
        using Clock = std::chrono::high_resolution_clock;
        auto nextGameUpdate = Clock::now(); // Set initial update time

        while (true) {
            auto loopStart = Clock::now();

            // Check if the application should continue running
            {
                std::unique_lock<std::mutex> lock(m_MutexRunning);
                if (!m_Running)
                    break;
            }

            if (m_Game.IsRunning()) {
                // Rest of the game loop
                m_Game.Event();
                m_Game.CollisionCheck();
                m_Game.Render();

                // Lock for conditional wait
                std::unique_lock<std::mutex> lockCV(m_MutexCV);

                // Calculate time till next update
                auto timeToNextUpdate = nextGameUpdate - loopStart;

                if (timeToNextUpdate <= std::chrono::milliseconds(0)) {
                    // Update game and set next update time
                    m_Game.Update();
                    nextGameUpdate = Clock::now() + std::chrono::milliseconds(GAME_SPEED);
                }
                else {
                    // Wait for a signal or until the next update time
                    m_CV.wait_for(lockCV, timeToNextUpdate);
                }
            }
            else {
                // Handle game ending - either restart or exit
                m_Game.End();
                std::unique_lock<std::mutex> lockCV(m_MutexCV);
                m_CV.wait(lockCV, [] { return !EventQueue::Get().IsEmpty(); });

                KeyEvent* event = EventQueue::Get().Pop().get();
                if (event->GetKey()[0] != STOP) {
                    _Restart();
                }
            }
        }
    }



private:
    void _Restart() {
        m_Game.Restart();
    }
private:
    std::mutex m_MutexRunning;
    bool m_Running = true;
    Game m_Game;
    std::mutex m_MutexCV;
    std::condition_variable m_CV;
    InputHandler m_InputHandler;
};

#endif /*APPLICATION_H_*/