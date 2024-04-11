// Inherit from state
#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "state.h"
#include "object.h"
#include "ground.h"

#include "LittleOBJLoader.h"
#include "VectorUtils4.h"

#include <vector>


class Menu_State : public State {
    public:
    Menu_State(Context* c);
    void keyboard(unsigned char key, int x, int y) override;
    void keyboard_up(unsigned char key, int x, int y) override;
    void mouse(int x, int y) override;
    void update(int time_elapsed) override;
    void display() override;
    ~Menu_State() override;

    private:
    std::vector<Model*> planes;

};

#endif