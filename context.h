#ifndef CONTEXT_H
#define CONTEXT_H

#include "state.h"
#include <jsoncpp/json/json.h>

class State; // Forward declaration


struct Context
{
    State*          next_state{};
    Json::Value     settings;
};

#endif
