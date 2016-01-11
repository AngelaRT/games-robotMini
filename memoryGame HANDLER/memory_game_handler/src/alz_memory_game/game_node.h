#ifndef ALZ_GAME_NODE
#define ALZ_GAME_NODE

//AUX CLASSES (include most of imports needed in alz_game_node)
#include "game_classes.h"

//PUBLISHERS
ros::Publisher gamesFeedback_pub; //Publisher in the handler's topic
//SUBSCRIBERS
ros::Subscriber gamesCommand_sub; //Subscriber to the handler's topic
ros::Subscriber dialog_sub; //Subscriber to the dialog topic (voice game commands)
ros::Subscriber internalFeedback_sub; //Subscriber to the game_classes feedback topic
string defaultPackage = "memory_game_handler";

alzGameMainClass currentGame;
int aux_argc; char** aux_argv;

#endif