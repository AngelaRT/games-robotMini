#ifndef ALZ_MEMORY_GAME
#define ALZ_MEMORY_GAME

//General
#include <stdlib.h> 
#include <sstream>
//#include <mysql++/mysql++.h>
//ROS
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "xml/XmlDocument.h"

//AD-ROS
#include "tv_on_demand/multimediaPlayer.h"
#include "etts_msgs/Utterance.h"
#include "primitives/nonverbal/non_verbal.h"
#include "definitions/utterance_params.h"
#include "screens_msgs/ScreensExpressions.h"
#include "utils/speech_snippets/speech_snippets.h"
//#include "long_term_memory/ltm_path.h"
//#include "etts/utils/speech_snippets/speech_snippets.h"


#define ETTS_PRIMITIVE etts_msgs::Utterance::PRIM_LOQUENDO
#define ETTS_LANGUAGE "es"

#define PAUSED 0
#define ACTIVE 1
#define STOPPED 2

#define ALZ_MEMORY_GAME_DIR    "@CMAKE_CURRENT_SOURCE_DIR@/"

using namespace std;

//Global variables

int status; //active or paused
bool presentation=false;
string last_presented;
string requested_dialog;
string next_to_present;

int objects;
XmlDocument doc_games;
std::vector<XmlDocument::Node*> nodes_game;

ros::Publisher tales_start,tablet_player,etts_say_text,
               make_gesture, etts_shut_up, screens_exp;
ros::Publisher dialog_finished_pub, dialog_started_pub;
ros::Publisher ask4GameCommand_memGame_pub;

ros::Subscriber dialog_sub;
ros::Subscriber pairFound_sub;

//Messages
tv_on_demand::multimediaPlayer tablet_msg;
etts_msgs::Utterance etts_msg;
std_msgs::String string_msg;
screens_msgs::ScreensExpressions screens_msg;

//MemoryGame vars
string numbers[] = {"one","two","three","four","five","six","seven","eight","nine","ten","eleven","twelve"};
int numCards = 8; //Default --> medium level
bool gameStarted=false;
std_msgs::String card2show;

//Callbacks
void dialogCallback(const std_msgs::String::ConstPtr& msg);

//Auxiliar functions
void presentSession();
void play_game(string game);
void sayGoodbye();
bool stopPresentation();
void showMultimedia(string multim_url, string multim_type);
void stopMultimedia();

//Main function where the processing is done
void process();


#endif // ALZ_MEMORY_GAME
