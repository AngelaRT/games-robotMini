#ifndef ALZ_GAME_CLASSES
#define ALZ_GAME_CLASSES
//General
#include <stdlib.h> 
#include <sstream>
#include <iostream>
#include <string>
#include <stdio.h>
//ROS
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "xml/XmlDocument.h"

//AD-ROS
#include "tv_on_demand/multimediaPlayer.h"
#include "screens_msgs/ScreensExpressions.h"
#include "etts_msgs/Utterance.h"
#include "utils/speech_snippets/speech_snippets.h"
#include "memory_game_handler/Feedback.h"
#include "memory_game_handler/Command.h"

//STATUS
#define PAUSED 0
#define ACTIVE 1
#define STOPPED 2
//COMMANDS
#define START 1
#define STOP 2
#define PAUSE 0
//PRIORITY
#define QUEUE 1
#define SHUTUP 2
//TYPE OF GAME
#define OTHER 3
#define XML_INTERACTIVE 2
#define XML 1
#define BROWSER 0

#define ETTS_PRIMITIVE etts_msgs::Utterance::PRIM_LOQUENDO
#define ETTS_LANGUAGE "es"

#define ALZ_GAME_NODE_DIR    "@CMAKE_CURRENT_SOURCE_DIR@/"

using namespace std;

class alzXMLGameClass {
   protected:
     string game_name;
     int game_status;
     int game_subType;
     string package;
     ros::Publisher tablet_player;
     ros::Publisher etts_say_text;
     ros::Publisher make_gesture;
     ros::Publisher internalFeedback_pub;
	 ros::Subscriber internalCommand_sub;
     void showMultimedia(string multim_url, string multim_type);
     void stopMultimedia();
     void playGame(int argc, char** argv, string game_name, string package, int game_subType);
     void handleGame(const memory_game_handler::Command& gameCommand);
     void startGame();
     void startProactiveGame();
     bool stopPresentation();
     void sendFinishGame();
     int handleGameCommand(string command);
     bool isAGameCommand(string msg);
};

class alzBrowserGameClass {
	protected:
		string game_name;
	    int status;
		ros::Publisher ask4GameCommand_memGame_pub;
		ros::Publisher make_gesture;
		ros::Publisher etts_say_text;
		ros::Publisher internalFeedback_pub;
		ros::Subscriber reportCommandResult_sub;
		ros::Subscriber internalCommand_sub;
		int numCards;
		int game_status;
		string previousName;
		int previousStatus;
		string package;

		void openGameInBrowser();
		void closeBrowser();
		void dialogCallback_memGame(const std_msgs::String::ConstPtr& msg);
		void sendGameCommand(string command);
		bool isANumber(string msg);
		void ask4Hint();
		void setLevelGame(string level);
		bool matchRE(string input, string regex);
		bool isADiff(string msg);
		void saySentence(string sentence, int priority);
		void makeGesture(string gesture);
		void speakIntro(string package);
		void playGame(int argc, char** argv, string game_name, string package);
	  	int handleGameCommand(string command);
	  	bool isAGameCommand(string msg);
	  	void sendFinishGame();
	  	void handleGame(const memory_game_handler::Command& gameCommand);
	  	string getRandomString(string sentencesToSynthesize);
};
class alzGameMainClass : public alzXMLGameClass, public alzBrowserGameClass {
	public: 
		alzGameMainClass();
		string get_game_name();
		int get_game_type();
		int get_game_status();
		void updateGame(int game_status, bool isReset);
		void playGame(int argc, char** argv,string package, string game_name, int game_type);
		int handleGameCommand(string command);
		void finishGame();
	protected:
		bool FLAG;
		string game_name;
	    int game_status;
     	int game_type;	
     	ros::Publisher start_game_pub;
     	void sendInternalCommand(string game_name, int game_type, int command);
};
#endif