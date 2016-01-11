#ifndef GAME_HANDLER
#define GAME_HANDLER

//General
//#include <stdio.h>
//#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
//#include <mysql++/mysql++.h>

//ROS
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "xml/XmlDocument.h"

//AD-ROS
#include "tv_on_demand/multimediaPlayer.h"
#include "memory_game_handler/Command.h"
#include "memory_game_handler/Feedback.h"
#include "etts_msgs/Utterance.h"
#include "primitives/nonverbal/non_verbal.h"
#include "definitions/utterance_params.h"
#include "screens_msgs/ScreensExpressions.h"


#define ETTS_PRIMITIVE etts_msgs::Utterance::PRIM_LOQUENDO
#define ETTS_LANGUAGE "es"

#define PAUSED 0
#define ACTIVE 1
#define STOPPED 2

using namespace std;

//Global variables

int status; //active or paused
int type;
string requested_dialog;
string recent_game;
string last_game;
int opt1=0, opt2=0;
int randomNumber=0;
int game_stop=0,game_exit=0,resume=0;

int objects;
XmlDocument doc_games;
std::vector<XmlDocument::Node*> node_games;

//ros::Publisher

ros::Publisher etts_say_text,game_command_pub,game_response_pub, game_finish_pub,game_xml_pub;
ros::Subscriber dialog_request_sub, game_feedback_sub;

//Messages
memory_game_handler::Command game_command_msg;
std_msgs::String string_msg;
etts_msgs::Utterance etts_msg;

//Callbacks
void dialogRequestCallback(const std_msgs::String::ConstPtr& msg);
void feedbackCallback(const memory_game_handler::Feedback::ConstPtr& msg);

//Auxiliar functions
void sendCommand();
void gamesList();
void gamesRandom();
void searchGame();

#endif // GAME_HANDLER
