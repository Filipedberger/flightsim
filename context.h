#ifndef CONTEXT_H
#define CONTEXT_H

#include "state.h"
//#include <json.h>


struct Context
{
    State*                      next_state;
    //Json::Value                 settings;
};

#endif
