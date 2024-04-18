// Inherit from state
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "state.h"
#include <vector>
#include "object.h"
#include "LittleOBJLoader.h"
#include "VectorUtils4.h"


class Game_State : public State {
    public:
    Game_State(Context* c);
    void keyboard(unsigned char key, int x, int y) override;
    void mouse(int x, int y) override;
    void update(int time_elapsed) override;
    void display() override;
    ~Game_State() override;

    private:
    // Mouse movement
    double theta = - M_PI / 2.0f;
    double phi = 0;
    int deltaX = -1;
    int deltaY = -1;

    Object* plane;

    void move_camera(int time_elapsed);
};

#endif