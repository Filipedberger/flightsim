// Inherit from state
#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "state.h"
#include "object.h"
#include "map.h"
#include <vector>


class Menu_State : public State {
    public:
    Menu_State();
    void keyboard(unsigned char key, int x, int y) override;
    void keyboard_up(unsigned char key, int x, int y) override;
    void mouse(int x, int y) override;
    void update() override;
    void display() override;
    ~Menu_State() override;
};

#endif