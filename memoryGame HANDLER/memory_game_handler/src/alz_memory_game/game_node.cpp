#include "game_node.h"

/*
resetCurrentGame --> resets the currentGame var
*/
void resetCurrentGame(){
  currentGame.updateGame(STOPPED,true);
}
/*
sendFinishGame --> resets the node and reports that the game has just finished (alz/games_feedback)
*/
void sendFinishGame(){
  cout << "finished\n";
  string game_name = currentGame.get_game_name();
  if(game_name!=""){
    memory_game_handler::Feedback feedback_msg;
    feedback_msg.game_name=currentGame.get_game_name();
    feedback_msg.game_status=STOPPED;
    gamesFeedback_pub.publish(feedback_msg);
    resetCurrentGame();
  }
}
/*
sendFeedback --> actuates as a proxy between game_classes and handler
*/
void sendFeedback(const memory_game_handler::Feedback& internalGameFeedback){
  cout <<"node: send feedback \n";
  if(internalGameFeedback.game_status==STOPPED)
    sendFinishGame();
  else
    gamesFeedback_pub.publish(internalGameFeedback);
}
/*
pauseGame --> sets the game status to PAUSE
*/
void pauseGame(){
  cout << "paused\n";
  currentGame.updateGame(PAUSED,false);
}
/*
mainStreamCallBack --> run when something is received on alz/gamesCommand
The message received will have the following fields:
- game_name
- game_type
- command
*/
void mainStreamCallBack(const memory_game_handler::Command& gameCommand){
	cout << "receiving..." << gameCommand << "\n";
  cout << gameCommand.command<<"\n";
	switch(gameCommand.command){
		case START:
          currentGame.playGame(aux_argc,aux_argv,defaultPackage,gameCommand.game_name,gameCommand.game_type);
          break;
	    case STOP:
      cout<<"node: stop!\n";
	      	sendFinishGame();
	      	break;
	    case PAUSE:
	      	pauseGame();
	      	break;
	    default:
	      	pauseGame();
	}
    
}
/*
dialogCallback --> run when something is received on alz/game_dialog_request
*/
void dialogCallback(const std_msgs::String::ConstPtr& msg){

  printf("Me has pedido que haga: %s!\n", msg->data.c_str());  

  if(msg->data=="RESTART" || msg->data == "CHANGE_GAME"){
    cout << "node: RESTART!! \n";
    sendFinishGame();
  }else{
   int result = currentGame.handleGameCommand(msg->data);
   if(result==STOPPED)  {cout <<"node callback: stop\n";sendFinishGame();}
  }
  
}


int main(int argc, char **argv){

  aux_argc=argc; aux_argv=argv;
  ros::init(argc, argv, "alz_node_game");

  ros::NodeHandle n;

  //Define publishers
  gamesFeedback_pub = n.advertise<memory_game_handler::Feedback>("GAME_FEEDBACK",100);
  //Define subscribers
  dialog_sub = n.subscribe("game_dialog_request", 1, dialogCallback);
  gamesCommand_sub = n.subscribe("GAME_COMMAND",1,mainStreamCallBack);
  internalFeedback_sub = n.subscribe("GAME_INTERNAL_FEEDBACK",1,sendFeedback);

  ros::Rate loop_rate(10);
  while (ros::ok()){
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}