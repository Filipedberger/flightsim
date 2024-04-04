// Inherit from state
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "state.h"
#include "object.h"
#include "map.h"
#include <vector>


class Game_State : public State {
    public:
    Game_State();
    void keyboard(unsigned char key, int x, int y) override;
    void keyboard_up(unsigned char key, int x, int y) override;
    void mouse(int x, int y) override;
    void update() override;
    void display() override;
    ~Game_State() override;

    private:
    std::vector<Object*> objects;
    Map* map;
};

#endif