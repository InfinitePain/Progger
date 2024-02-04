#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <vector>
#include <memory>

class GameObject {
public:
    enum class Type {
        PLAYER,
        STREET,
        GOAL,
        MAX
    };
    struct Coordinates {
        int x;
        int y;
        Coordinates(int x, int y) : x(x), y(y) {}
    };
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual Type GetType() = 0;
    virtual std::vector<Coordinates> GetCollisionMap(Coordinates coordniates, int radius) = 0;
    virtual void CollideWith(std::shared_ptr<GameObject> object) = 0;
    virtual void OnEvent() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
};

#endif /*GAMEOBJECT_H_*/