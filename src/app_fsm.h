#include "fsm_logic.h"

#ifndef FSM_ENGINE_H
#define FSM_ENGINE_H

void initialiseFSM();

void cleanupFSM();

/* finds the transition for the current state */
Transition* findTransition(AppState state);

/* execting the action for a transition */
ActionStatus executeAction(ActionType actionType, void* param, AppContext* ctx);

/* states prcoessing */
AppState processState(AppContext* ctx);

AppContext* createAppContext();

void freeAppContext(AppContext* ctx);

#endif 