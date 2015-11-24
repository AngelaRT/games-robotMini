#include "alz_memory_game.h"

////////////////////////////////////////////////////////////////////////////////
///AUX FUNCTIONS FOR MEMORY GAME HANDLING
////////////////////////////////////////////////////////////////////////////////
void openGameInBrowser(string game){
  std::stringstream openInBrowser;
  openInBrowser << "chromium-browser file:///home/user/catkin_ws/src/memory_game_handler/games%20BROWSER/" << game << ".html &";
  system(openInBrowser.str().c_str());
}

bool matchRE(string input, string regex){
  /*Lo suyo es pasarle una regex. De momento lo hago adhoc*/
  string::size_type image_i = input.rfind("image");
  string::size_type diff_i = input.rfind("diff");
  return std::string::npos != image_i && std::string::npos != diff_i
    && image_i<diff_i;
}

bool isANumber(string msg){
    int i=0; bool found=false;
    if(gameStarted){
      while(!found && i<numCards){
          if(msg==numbers[i])
              found=true;
          else
              i++;
      }
    }
    return found;
}
bool isADiff(string msg){
  return matchRE(msg,"image(\\d+)diff(\\d+)");
}

bool isAGameCommand(string msg, string game){
    return (game=="memory_game" && (isANumber(msg) || msg=="easy_level" 
            || msg=="medium_level" || msg=="hard_level" 
            || msg=="get_hint" || msg=="restart"))
          || (game=="differences_game" && matchRE(msg,"image(\\d+)diff(\\d+)"));
}

void sendGameCommand(string command){
    // Publish alz/ask4GameCommand --> we publish on this topic the number of the card to show
    // cardsRequestHandler will be listening to this topic, in order to tell the browser to show that card

    card2show.data = command;
    std::cout << "PUBLICANDO alz/ask4GameCommand con valor " << command << "\n";
    ask4GameCommand_memGame_pub.publish(card2show);
}

void ask4Hint(){
  // We can use the communication channel between browser and robot to ask for a hint
  sendGameCommand("hint");
}

void startGame(string game){
  cout << "START GAME " << game << "\n";
  sendGameCommand(game);
  gameStarted=true; 
}
void setLevelGame(string level, string game){
  if(level!=""){
    if(level=="easy_level")
            numCards=4; //4 cards --> we check to the third position in the array
    else if(level=="medium_level")
            numCards=8; //8 cards --> we check to the seventh position in the array
    else if(level=="hard_level")
            numCards=12; //12 cards --> we check to the eleventh position in the array
    cout << "Level of game established: " << level << "\n";
    sendGameCommand(level);
  }   
}

void dialogCallback_memGame(const std_msgs::String::ConstPtr& msg){
  etts_msg.priority = etts_msgs::Utterance::QUEUE_SENTENCE;
  cout << "I receive from alz/reportCommandResult " << msg->data << "\n";
  if(msg->data == "t"){
    //User found a pair of cards
    etts_msg.text = ad::tts::SpeechSnippets::getRandomSnippet("GAME_ENCOURAGEMENT_SUCCESS");
    etts_say_text.publish(etts_msg);  
  }else if(msg->data=="f"){
    //Cards selected are not a pair
    etts_msg.text = ad::tts::SpeechSnippets::getRandomSnippet("GAME_ENCOURAGEMENT_FAIL");
    etts_say_text.publish(etts_msg);  
  }else if(msg->data=="a"){
    //User found all the pairs
    etts_msg.text = ad::tts::SpeechSnippets::getRandomSnippet("GAME_CONGRATULATION_SUCCESS");
    etts_say_text.publish(etts_msg);  
    sleep(5);
    etts_msg.text = ad::tts::SpeechSnippets::getRandomSnippet("GAME_RESTART_QUESTION");
    etts_say_text.publish(etts_msg);  
  }
}

////////////////////////////////////////////////////////////////////////////////
///CALLBACK
////////////////////////////////////////////////////////////////////////////////

void dialogCallback(const std_msgs::String::ConstPtr& msg){

  printf("Me has pedido que haga: %s\n", msg->data.c_str());
  printf("Antes pediste: %s\n", last_presented.c_str());
  

  if(msg->data == "pause") {
    if (status == ACTIVE)    status = PAUSED;
  }

  else if (msg->data == "previous"){
      if (status == PAUSED || status == STOPPED)    status = ACTIVE;
      requested_dialog = last_presented;
  }

  else if (msg->data == "next"){
      if (status == PAUSED || status == STOPPED)    status = ACTIVE;
      requested_dialog = next_to_present;
  }

  else if (msg->data == "resume"){
      if (status == PAUSED)    status = ACTIVE;
  }

  else if (msg->data == "stop"){
     status = STOPPED;
     requested_dialog = "";
  }

  else if(isAGameCommand(msg->data,last_presented)){
    status = ACTIVE;
    requested_dialog = msg->data;
  }

  else{
      status = ACTIVE;
      requested_dialog = msg->data;
      last_presented = "";
      printf("dialogCallback OK\n");
  }
}

////////////////////////////////////////////////////////////////////////////////
///ROBOT PRESENTATION
////////////////////////////////////////////////////////////////////////////////

void play_game_std(string game, string package){

  //Read the .xml document
    std::string pathPKG = ros::package::getPath(package);
    char pathXML[200];
    sprintf(pathXML,"%s/%s.xml", pathPKG.c_str(), game.c_str());
    doc_games.load_from_file(pathXML);

  XmlDocument::Node * n_game = doc_games.get_node_at_direction(doc_games.root(),game);
  string game_intro=doc_games.get_value(n_game,"intro");
  string time_intro_str=doc_games.get_value(n_game,"time_intro");
  int time_intro=atoi(time_intro_str.c_str());
  string gesture_intro=doc_games.get_value(n_game,"gesture_intro");

  etts_msg.emotion = etts_msg.EMOTION_HAPPY;
  string_msg.data = gesture_intro.c_str();
  make_gesture.publish(string_msg);
  etts_msg.text =game_intro.c_str();
  etts_say_text.publish(etts_msg);
  sleep(time_intro);

  if (stopPresentation())   return;

  string str_game_option=game+".item";
  doc_games.get_all_nodes_at_direction(doc_games.root(),str_game_option,nodes_game);
  printf("#objects = %i\n", nodes_game.size());

    for (unsigned int i = 0; i < nodes_game.size(); ++i)
    {

      XmlDocument::Node * n_obj = nodes_game.at(i);
      string n_question=doc_games.get_value(n_obj,"question");
      string n_multimedia_init=doc_games.get_value(n_obj,"multimedia_init");
      string n_multimedia_init_type=doc_games.get_value(n_obj,"multimedia_init_type");
      string n_gesture_init=doc_games.get_value(n_obj,"gesture_init");
      string n_wait_time_str=doc_games.get_value(n_obj,"wait_time");
      int n_wait_time=atoi(n_wait_time_str.c_str());

      string n_solut=doc_games.get_value(n_obj,"solution");
      string n_multimedia_solut=doc_games.get_value(n_obj,"multimedia_solut");
      string n_multimedia_solut_type=doc_games.get_value(n_obj,"multimedia_solut_type");
      string n_gesture_solut=doc_games.get_value(n_obj,"gesture_solut");
      string n_time_after_str=doc_games.get_value(n_obj,"time_after");
      int n_time_after=atoi(n_time_after_str.c_str());

      etts_msg.text = n_question.c_str();
      etts_say_text.publish(etts_msg);
      string_msg.data = n_gesture_init.c_str();
      make_gesture.publish(string_msg);
      showMultimedia(n_multimedia_init.c_str(),n_multimedia_init_type.c_str());

      if (stopPresentation())   return;
      sleep(n_wait_time);
      if (stopPresentation())   return;

      etts_msg.text = n_solut.c_str();
      etts_say_text.publish(etts_msg);
      string_msg.data = n_gesture_solut.c_str();
      make_gesture.publish(string_msg);
      showMultimedia(n_multimedia_solut.c_str(),n_multimedia_solut_type.c_str());
      sleep(n_time_after);
      stopMultimedia();
      sleep(1);

      if (stopPresentation())   return;
    }

  sleep(2);

  if (stopPresentation())   return;

  if (presentation==true) requested_dialog = next_to_present;
  else  requested_dialog = "";

}
void play_game(string game,string package){
  // CUIDADO, NO ES LA VERSIÓN ESTÁNDAR PARA TODOS LOS JUEGOS
  // TIENE CONTENIDO PROPIO PARA MEMORY GAME

    //Read the .xml document
    std::string pathPKG = ros::package::getPath(package);
    char pathXML[200];
    sprintf(pathXML,"%s/%s.xml", pathPKG.c_str(), game.c_str());
    doc_games.load_from_file(pathXML);

  XmlDocument::Node * n_game = doc_games.get_node_at_direction(doc_games.root(),game);
  string game_intro=doc_games.get_value(n_game,"intro");
  string time_intro_str=doc_games.get_value(n_game,"time_intro");
  int time_intro=atoi(time_intro_str.c_str());
  string gesture_intro=doc_games.get_value(n_game,"gesture_intro");

  etts_msg.emotion = etts_msg.EMOTION_HAPPY;
  string_msg.data = gesture_intro.c_str();
  make_gesture.publish(string_msg);
  etts_msg.text =game_intro.c_str();
  etts_say_text.publish(etts_msg);
  sleep(time_intro);

  if (stopPresentation())   return;

  string str_game_option=game+".item";
  doc_games.get_all_nodes_at_direction(doc_games.root(),str_game_option,nodes_game);
  printf("#objects = %i\n", nodes_game.size());

    for (unsigned int i = 0; i < nodes_game.size(); ++i)
    {

      XmlDocument::Node * n_obj = nodes_game.at(i);
      string n_question=doc_games.get_value(n_obj,"question");
      string n_gesture_init=doc_games.get_value(n_obj,"gesture_init");
      string n_wait_time_str=doc_games.get_value(n_obj,"wait_time");
      int n_wait_time=atoi(n_wait_time_str.c_str());

      etts_msg.text = n_question.c_str();
      etts_say_text.publish(etts_msg);
      string_msg.data = n_gesture_init.c_str();
      make_gesture.publish(string_msg);

      //Launch browser when Mini refers to its content
      openGameInBrowser(game); 
      startGame(game);

      if (stopPresentation())   return;
      sleep(n_wait_time);
      if (stopPresentation())   return;

    }
  sleep(2);

  if (stopPresentation())   return;

 /* if (presentation==true) requested_dialog = next_to_present;
  else  requested_dialog = "";*/

}


////////////////////////////////////////////////////////////////////////////////
///GOODBYE
////////////////////////////////////////////////////////////////////////////////

void sayGoodbye(){

  last_presented = "goodbye";
  requested_dialog = "";
  etts_msg.emotion = etts_msg.EMOTION_HAPPY;

  string_msg.data = "alz_happy";
  make_gesture.publish(string_msg);
  etts_msg.text = "Chicos, por hoy ya hemos terminado";
  etts_say_text.publish(etts_msg);
  sleep(8);

  string_msg.data = "alz_happy";
  make_gesture.publish(string_msg);
  string_msg.data = "alz_back_to_normal";
  make_gesture.publish(string_msg);

  requested_dialog = "";

}

////////////////////////////////////////////////////////////////////////////////
///MULTIMEDIA FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void showMultimedia(string multim_url, string multim_type){

  tablet_msg.text = multim_type;
  tablet_msg.url = multim_url;
  tablet_msg.type = multim_type;
  tablet_player.publish(tablet_msg);
}

void stopMultimedia(){
  tablet_msg.text = "back";
  tablet_msg.url = "back";
  tablet_msg.type = "back";
  tablet_player.publish(tablet_msg);
}

////////////////////////////////////////////////////////////////////////////////
///AUX FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
void presentSession(){

    etts_msg.emotion = etts_msg.EMOTION_HAPPY;
    string_msg.data ="alz_test_02";
    make_gesture.publish(string_msg);
    etts_msg.text ="¿A qué quieres jugar?";
    etts_say_text.publish(etts_msg);
    sleep(12);

    if (stopPresentation())   return;

  requested_dialog = next_to_present;

}

bool stopPresentation(){
  ros::Rate loop_rate(10);
  ros::spinOnce();

  if(status == PAUSED){
    string_msg.data = "alz_back_to_normal";
    make_gesture.publish(string_msg);
    etts_msg.text ="\\emphasis-- Hago una pausa";
    etts_msg.priority = etts_msgs::Utterance::SHUTUP_IMMEDIATLY_AND_SAY_SENTENCE;
    etts_say_text.publish(etts_msg);
    while(status == PAUSED){
        ros::spinOnce();
        loop_rate.sleep();
    }
    etts_msg.priority = etts_msgs::Utterance::QUEUE_SENTENCE;
    if (status == STOPPED) return true;
    else return false;
  }else if(status == STOPPED){
    string_msg.data = "alz_back_to_normal";
    make_gesture.publish(string_msg);
    stopMultimedia();
    etts_msg.text ="\\emphasis-- Ya me callo";
    etts_msg.priority = etts_msgs::Utterance::SHUTUP_IMMEDIATLY_AND_SAY_SENTENCE;
    etts_say_text.publish(etts_msg);
    requested_dialog = "";
    etts_msg.priority = etts_msgs::Utterance::QUEUE_SENTENCE;
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
///PROCESS
////////////////////////////////////////////////////////////////////////////////
void process(){

  if (status == PAUSED || status == STOPPED){
        return;
    }

  else {
    if (requested_dialog == "presentSession"){
          presentation=true;
          last_presented = "presentSession";
          next_to_present = "memory_game";
          string_msg.data = requested_dialog;
          dialog_started_pub.publish(string_msg);
          presentSession();
          //Tell the dialog we have finished answering this question
          string_msg.data = last_presented;
          dialog_finished_pub.publish(string_msg);
        }
    else if (requested_dialog =="memory_game" && last_presented!="memory_game"){
         last_presented = "memory_game";
         next_to_present = "";
         etts_msg.text ="Empecemos";
         etts_msg.priority = etts_msgs::Utterance::SHUTUP_IMMEDIATLY_AND_SAY_SENTENCE;
         etts_say_text.publish(etts_msg);
         string_msg.data = requested_dialog;
         dialog_started_pub.publish(string_msg);
         play_game(requested_dialog,"memory_game_handler");
         string_msg.data = last_presented;
         dialog_finished_pub.publish(string_msg);
       //  startGame(requested_dialog);
   }
   else if(requested_dialog =="differences_game" && last_presented!="differences_game"){
       last_presented = "differences_game";
       next_to_present = "";
       string_msg.data = requested_dialog;
       dialog_started_pub.publish(string_msg);

       //////// **** DE MOMENTO METO EL XML EN EL MISMO PAQUETE PERO LUEGO HABRÁ QUE PONER CADA XML EN SU PKG
       play_game(requested_dialog,"memory_game_handler");
       string_msg.data = last_presented;
       dialog_finished_pub.publish(string_msg);
      // startGame(requested_dialog);
   }
   else if (requested_dialog == "goodbye"){
      string_msg.data = requested_dialog;
      dialog_started_pub.publish(string_msg);
      sayGoodbye();
      string_msg.data = "goodbye";
      dialog_finished_pub.publish(string_msg);
    }
   else if (isAGameCommand(requested_dialog,last_presented)){
      if(requested_dialog=="restart"){
        sendGameCommand(requested_dialog);
      }else{
        if(last_presented=="memory_game"){
          if(isANumber(requested_dialog)){
            sendGameCommand(requested_dialog);
          }else if(requested_dialog=="get_hint"){
            etts_msg.text ="Voy a marcar en verde dos cartas que forman una pareja!";
            etts_msg.priority = etts_msgs::Utterance::QUEUE_SENTENCE;
            etts_say_text.publish(etts_msg);
            ask4Hint();
          }else{
            setLevelGame(requested_dialog,last_presented);
          }
        }else if (last_presented=="differences_game"){
          if(isADiff(requested_dialog)){
            sendGameCommand(requested_dialog);
          }
        }
        requested_dialog="";
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
///MAIN
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){
  ros::init(argc, argv, "alz_memory_game");
  ros::NodeHandle n;


  //Initially the robot is paused and hasn't presented anything
  status = PAUSED;
  last_presented = "";
  requested_dialog = "";

  //Define all the publishers
  tales_start = n.advertise<std_msgs::Int16>("MEMORY_GAME_SESSION_START", 100);
  tablet_player = n.advertise<tv_on_demand::multimediaPlayer>("tablet_player", 100);
  etts_say_text = n.advertise<etts_msgs::Utterance>("etts", 100);
  make_gesture = n.advertise<std_msgs::String>("keyframe_gesture_filename", 100);
  etts_shut_up = n.advertise<std_msgs::Int16>("ETTS_SHUT_UP", 100);
  screens_exp = n.advertise<screens_msgs::ScreensExpressions>("screens", 1);

  dialog_started_pub = n.advertise<std_msgs::String>("game_dialog_started", 1);
  dialog_finished_pub = n.advertise<std_msgs::String>("presentation_dialog_finished", 1);

  ask4GameCommand_memGame_pub = n.advertise<std_msgs::String>("ask4GameCommand", 1);

  //Define subscribers
  dialog_sub = n.subscribe("game_dialog_request", 1, dialogCallback);
  reportCommandResult_sub = n.subscribe("reportCommandResult", 1, dialogCallback_memGame);

  //Start the necessary nodes
  std_msgs::Int16 m;
  tales_start.publish(m);

  //Default params for etts_msg
  etts_msg.primitive = ETTS_PRIMITIVE;
  etts_msg.language = ETTS_LANGUAGE;



  ros::Rate loop_rate(10);
  while (ros::ok()){
    process();
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}


