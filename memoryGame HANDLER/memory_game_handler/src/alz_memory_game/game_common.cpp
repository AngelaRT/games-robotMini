#include "game_classes.h"
/*********************************************************************************
********************************alzGameMainClass**********************************
*********************************************************************************/
alzGameMainClass::alzGameMainClass(){
  alzGameMainClass::game_name="";
  alzGameMainClass::game_type=-1;
  alzGameMainClass::game_status=STOPPED;
  alzXMLGameClass::game_name="";
  alzXMLGameClass::game_status=STOPPED;
  alzBrowserGameClass::game_name="";
  alzBrowserGameClass::game_status=STOPPED;

  this->FLAG=false;
}

string alzGameMainClass::get_game_name(){
  return alzGameMainClass::game_name;
}
int alzGameMainClass::get_game_type(){
  return alzGameMainClass::game_type;
}
int alzGameMainClass::get_game_status(){
  return alzGameMainClass::game_status;
}

void alzGameMainClass::updateGame(int game_status, bool isReset){
  cout << "COMMON 1--> alzGameMainClass::game_status=" << alzGameMainClass::game_status << "\n";
  if(isReset) alzGameMainClass::sendInternalCommand(alzGameMainClass::game_name,alzGameMainClass::game_type, STOP);
  alzGameMainClass::game_name=isReset ? "" : alzGameMainClass::game_name;
  alzGameMainClass::game_type=isReset ? -1 : alzGameMainClass::game_type;
  alzGameMainClass::game_status=game_status;
  
  if(alzGameMainClass::game_type==XML || alzGameMainClass::game_type==-1){
    alzXMLGameClass::game_name=  isReset ? "" : alzXMLGameClass::game_name;
    alzXMLGameClass::game_status=game_status;
  }
  if(alzGameMainClass::game_type==BROWSER || alzGameMainClass::game_type==-1){
    alzBrowserGameClass::game_name=  isReset ? "" : alzBrowserGameClass::game_name;
    alzBrowserGameClass::game_status=game_status;
    cout << "isReset = "<<isReset<<", game_type=="<<game_type<<"\n";
    if (isReset) alzBrowserGameClass::closeBrowser();
  }
  cout << "COMMON 2--> alzGameMainClass::game_status=" << alzGameMainClass::game_status << "\n";
}

void alzGameMainClass::playGame(int argc, char** argv,string package, string game_name, int game_type){
  
  ros::init(argc, argv, "alz_game_main_node");
  ros::NodeHandle n;  
  alzGameMainClass::start_game_pub = n.advertise<memory_game_handler::Command>("GAME_INTERNAL_COMMAND",100);

  int game_subType = game_type;
  if(game_type==XML || game_type==XML_INTERACTIVE){
    game_subType=game_type;
    game_type=XML;
  }

  this->FLAG=true;
  alzGameMainClass::game_name=game_name;
  alzGameMainClass::game_type=game_type;
  alzGameMainClass::game_status=ACTIVE;  

  if(game_type==XML && alzXMLGameClass::game_status==PAUSED || 
    game_type==BROWSER && alzBrowserGameClass::game_status==PAUSED){
    updateGame(ACTIVE,game_type);
  }else{
    cout << "main --- playgame \n";
    //int status = alzGameMainClass::game_status;
    // First prepare the game
    switch(game_type){
      case XML:
        alzXMLGameClass::playGame(argc,argv,game_name,package,game_subType);
        break;
      case BROWSER:
        alzBrowserGameClass::playGame(argc,argv,game_name,package);
        break;
    }
    // Then start the game
    alzGameMainClass::sendInternalCommand(game_name,game_type,START);
  }
  //return status;
}

void alzGameMainClass::sendInternalCommand(string game_name, int game_type, int command){
  if(this->FLAG){
    memory_game_handler::Command startCommand;
    startCommand.game_name=game_name;
    startCommand.game_type=game_type;
    startCommand.command=command;
    cout <<"common --> publish "<<startCommand<<"\n";
    alzGameMainClass::start_game_pub.publish(startCommand);
  }
}
int alzGameMainClass::handleGameCommand(string command){
  int status = alzGameMainClass::game_status;
  if(alzGameMainClass::game_type==BROWSER && alzBrowserGameClass::isAGameCommand(command)){
    status = alzBrowserGameClass::handleGameCommand(command);
  }
  if(alzGameMainClass::game_type==XML && alzXMLGameClass::isAGameCommand(command)){
    status = alzXMLGameClass::handleGameCommand(command);
  }
  if(status==STOPPED) alzGameMainClass::finishGame();
  return status;
}

void alzGameMainClass::finishGame(){
  alzGameMainClass::sendInternalCommand(game_name,game_type,STOP);
  alzGameMainClass::updateGame(STOPPED, true);
  if(alzGameMainClass::game_type==BROWSER) alzBrowserGameClass::closeBrowser();
}