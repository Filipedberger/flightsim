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
    void keyboard_up(unsigned char key, int x, int y) override;
    void mouse(int x, int y) override;
    void update(int time_elapsed) override;
    void display() override;
    ~Game_State() override;

    private:
    std::vector<Object*> objects;
};

#endif