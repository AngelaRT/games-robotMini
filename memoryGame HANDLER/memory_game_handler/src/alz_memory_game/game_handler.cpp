#include "game_handler.h"

////////////////////////////////////////////////////////////////////////////////
///CALLBACK
////////////////////////////////////////////////////////////////////////////////

void dialogRequestCallback(const std_msgs::String::ConstPtr& msg){

  printf("Me has pedido que haga: %s\n", msg->data.c_str());
  
  if(msg->data == "list") {
    gamesList();
  }
else if(msg->data == "random") {
    gamesRandom();
  }
  else if(msg->data == "pause" && recent_game != "") {
    if (status == ACTIVE){
        status = PAUSED;
        sendCommand();
    }
  }
  else if (msg->data == "resume" && recent_game != ""){
      if (status == PAUSED)    {
        status = ACTIVE;
        resume=1;
        sendCommand();
        }
  }
 else if (msg->data == "stop"){
    etts_msg.text ="Dejamos este juego";
    etts_say_text.publish(etts_msg);
    sleep(1);
    game_stop=1;
    status = STOPPED;
    sendCommand();
    //last_game=recent_game;
    //recent_game="";
  }
  else if (msg->data == "exit"){
    game_exit=1;
    status = STOPPED;
    sendCommand();
    //last_game=recent_game;
    //recent_game="";
  }
  /*else if (msg->data == "previous"){
      if (status == PAUSED || status == STOPPED)    status = ACTIVE;
      requested_dialog = last_presented;
  }*/
  else{
    if (status == STOPPED){
      game_stop=0;
      game_exit=0;
      status = ACTIVE;
      last_game = recent_game;
      recent_game = msg->data;
      searchGame();
    }else if (status == PAUSED){
      etts_msg.text ="Si quieres que cambie de juego, primero mándame callar y después elige de nuevo. ";
      etts_say_text.publish(etts_msg);
      sleep(4);
    }
  }
}

void feedbackCallback(const memory_game_handler::Feedback::ConstPtr& msg){

printf("estado del juego %s: %d\n", msg->game_name.c_str(),msg->game_status);

  if(msg->game_status == STOPPED && game_exit==0) {
        if(game_stop==0){
          etts_msg.text ="Este juego ya ha acabado";
          etts_say_text.publish(etts_msg);
          sleep(3);
        }
        game_stop=0;        
        last_game = msg->game_name;
        recent_game = "";
        status = STOPPED;
        string_msg.data = "yes";
        game_finish_pub.publish(string_msg);
    }
}

////////////////////////////////////////////////////////////////////////////////
///AUX FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void sendCommand(){
    game_command_msg.game_name=recent_game;
    game_command_msg.game_type=type;
    game_command_msg.command=status;
    game_command_pub.publish(game_command_msg);
   /* if(type==1){
        if(status==ACTIVE && resume==0){
        string_msg.data=recent_game;
        game_xml_pub.publish(string_msg);
        }
        else if(status==ACTIVE && resume==1){
        resume=0;
        string_msg.data="resume";
        game_xml_pub.publish(string_msg);
        }
        else if(status==STOPPED){
        string_msg.data="stop";
        game_xml_pub.publish(string_msg);
        }
        else if(status==PAUSED){
        string_msg.data="pause";
        game_xml_pub.publish(string_msg); 
        }
    }else{
        game_command_msg.game_name=recent_game;
        game_command_msg.game_type=type;
        game_command_msg.command=status;
        game_command_pub.publish(game_command_msg);
    }*/
}
    
void gamesList(){
    
    etts_msg.emotion = etts_msg.EMOTION_HAPPY;
    etts_msg.text ="esta es la lista de juegos que me sé.";
    etts_say_text.publish(etts_msg);
    sleep(7);
    for (unsigned int i = 0; i < node_games.size(); ++i)
    {
        XmlDocument::Node * n_game = node_games.at(i);
        std::string game_description=doc_games.get_value(n_game,"description");
        etts_msg.text =game_description.c_str();
        etts_say_text.publish(etts_msg);
        sleep(3);
    }
    etts_msg.text ="a qué juego quieres jugar,";
    etts_say_text.publish(etts_msg);
    sleep(3);     
}

void gamesRandom(){
    timeval timeStamp;
    gettimeofday( &timeStamp, NULL);
    srand(timeStamp.tv_usec);
    randomNumber =1+(rand() % node_games.size());
    while((randomNumber==opt1)||(randomNumber==opt2)){
        randomNumber =1+(rand() % node_games.size());
    }
    opt1=opt2;
    opt2=randomNumber;

    XmlDocument::Node * n_game = node_games.at(randomNumber);
    std::string game_name=doc_games.get_value(n_game,"name");
    string_msg.data = game_name;
    game_response_pub.publish(string_msg);
}

void searchGame(){
  for (unsigned int i = 0; i < node_games.size(); ++i)
  {
      XmlDocument::Node * n_game = node_games.at(i);
      std::string game_name=doc_games.get_value(n_game,"name");
      if (game_name==recent_game){
          printf("se ha encontrado el juego\n");
          string type_str=doc_games.get_value(n_game,"type");
          type=atoi(type_str.c_str());
          status=ACTIVE;
          sendCommand();
      }
  }
}

////////////////////////////////////////////////////////////////////////////////
///MAIN
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){

  ros::init(argc, argv, "alz_games_handler");
  ros::NodeHandle n;

  //Initially the robot is paused and hasn't presented anything
  status = STOPPED;
  type=0;
  last_game = "";
  recent_game = "";
  requested_dialog = "";

  //Define all the publishers
    etts_say_text = n.advertise<etts_msgs::Utterance>("etts", 100);
    game_xml_pub = n.advertise<std_msgs::String>("game_request_xml", 1);
    game_command_pub = n.advertise<memory_game_handler::Command>("GAME_COMMAND",100);
    game_response_pub = n.advertise<std_msgs::String>("GAMES_HANDLER_RESPONSE", 1);
    game_finish_pub = n.advertise<std_msgs::String>("GAMES_HANDLER_GAME_FINISHED", 100);
  
  //Define subscribers
  dialog_request_sub = n.subscribe("GAMES_HANDLER_REQUEST", 1, dialogRequestCallback);
  game_feedback_sub = n.subscribe("GAME_FEEDBACK", 1, feedbackCallback);
  
  //Default params for etts_msg
  etts_msg.primitive = ETTS_PRIMITIVE;
  etts_msg.language = ETTS_LANGUAGE;

  //Read the .xml document
  std::string xml_route= ros::package::getPath("memory_game_handler") + "/data/games_list.xml";
  doc_games.load_from_file(xml_route);
  doc_games.get_all_nodes_at_direction(doc_games.root(),"game",node_games);

  ros::Rate loop_rate(10);
  while (ros::ok()){
    //process();
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}


