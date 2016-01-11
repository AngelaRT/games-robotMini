#include "game_classes.h"

/*********************************************************************************
********************************alzXMLGameClass***********************************
*********************************************************************************/
void alzXMLGameClass::showMultimedia(string multim_url, string multim_type){

  tv_on_demand::multimediaPlayer tablet_msg;
  tablet_msg.text = multim_type;
  tablet_msg.url = multim_url;
  tablet_msg.type = multim_type;
  tablet_player.publish(tablet_msg);
}

void alzXMLGameClass::stopMultimedia(){
  
  tv_on_demand::multimediaPlayer tablet_msg;
  tablet_msg.text = "back";
  tablet_msg.url = "back";
  tablet_msg.type = "back";
  tablet_player.publish(tablet_msg);
}

void alzXMLGameClass::sendFinishGame(){
  memory_game_handler::Feedback feedback_msg;
  feedback_msg.game_name=game_name;
  feedback_msg.game_status=STOPPED;
  this->internalFeedback_pub.publish(feedback_msg);
}
void alzXMLGameClass::startProactiveGame(){
  cout << "XML --- STARTPROACTIVEGAME\n";
  etts_msgs::Utterance etts_msg;
  std_msgs::String string_msg;
  XmlDocument doc_games;
  std::vector<XmlDocument::Node*> node_games,atr_game,items_game;

  etts_msg.priority = etts_msgs::Utterance::SHUTUP_IMMEDIATLY_AND_SAY_SENTENCE;
  etts_msg.text ="";

  std::string pathPKG = ros::package::getPath(package);
  char pathXML[200];
  sprintf(pathXML,"%s/data/game_xml.xml", pathPKG.c_str());
  doc_games.load_from_file(pathXML);
  doc_games.get_all_nodes_at_direction(doc_games.root(),"game",node_games);
}
void alzXMLGameClass::startGame(){
  cout << "XML --- STARTGAME\n";
  etts_msgs::Utterance etts_msg;
  std_msgs::String string_msg;
  XmlDocument doc_games;
  std::vector<XmlDocument::Node*> node_games,atr_game,items_game;

  etts_msg.priority = etts_msgs::Utterance::SHUTUP_IMMEDIATLY_AND_SAY_SENTENCE;
  etts_msg.text ="";

  std::string pathPKG = ros::package::getPath(package);
  char pathXML[200];
  sprintf(pathXML,"%s/data/game_xml.xml", pathPKG.c_str());
  doc_games.load_from_file(pathXML);
  doc_games.get_all_nodes_at_direction(doc_games.root(),"game",node_games);

  cout << "XML --- NODOS: "<<node_games.size()<<"\n";
  for (unsigned int i = 0; i < node_games.size(); ++i)
  {
      XmlDocument::Node * n_game = node_games.at(i);
      std::string game_name=doc_games.get_value(n_game,"name");

      if (game_name==this->game_name){
          //etts_say_text.publish(etts_msg);
          sleep(1);
          printf("se ha encontrado el juego\n");
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
          cout<<"LEYENDO LA INTRO\n";

          if (stopPresentation())   return;

          doc_games.get_all_nodes_at_direction(n_game,"item",items_game);
          printf("#objects = %i\n", items_game.size());

            for (unsigned int i = 0; i < items_game.size(); ++i)
            {
              cout<<"LEYENDO ITEM\n";
              XmlDocument::Node * n_obj = items_game.at(i);
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
              alzXMLGameClass::showMultimedia(n_multimedia_init.c_str(),n_multimedia_init_type.c_str());

              if (stopPresentation())   return;
              sleep(n_wait_time);
              if (stopPresentation())   return;

              etts_msg.text = n_solut.c_str();
              etts_say_text.publish(etts_msg);
              string_msg.data = n_gesture_solut.c_str();
              make_gesture.publish(string_msg);
              alzXMLGameClass::showMultimedia(n_multimedia_solut.c_str(),n_multimedia_solut_type.c_str());
              sleep(n_time_after);
              alzXMLGameClass::stopMultimedia();
              sleep(1);

              if (stopPresentation())   return;
            }
      }
  }
cout<<"JUEGO XML TERMINADO\n";
  sleep(2);
  alzXMLGameClass::sendFinishGame();
}

bool alzXMLGameClass::stopPresentation(){
  ros::Rate loop_rate(10);
  ros::spinOnce();
  std_msgs::String string_msg;
  etts_msgs::Utterance etts_msg;

  if(this->game_status == PAUSED){
    string_msg.data = "alz_back_to_normal";
    make_gesture.publish(string_msg);
    etts_msg.text ="\\emphasis-- Hago una pausa";
    etts_msg.priority = etts_msgs::Utterance::SHUTUP_IMMEDIATLY_AND_SAY_SENTENCE;
    etts_say_text.publish(etts_msg);
    while(this->game_status == PAUSED){
        ros::spinOnce();
        loop_rate.sleep();
    }
    etts_msg.priority = etts_msgs::Utterance::QUEUE_SENTENCE;
    if (this->game_status == STOPPED) 
    {        
        this->sendFinishGame();
        return true;
    }
    else return false;
  }else if(this->game_status == STOPPED){
    string_msg.data = "alz_back_to_normal";
    make_gesture.publish(string_msg);
    this->stopMultimedia();
    sleep(1);
    this->sendFinishGame();
    return true;
  }
  return false;
}

void alzXMLGameClass::handleGame(const memory_game_handler::Command& gameCommand){
  cout<<"HANDLE GAME "<<gameCommand.game_type<<", "<<gameCommand.command<<","<<this->game_status<<"\n";
  if(gameCommand.game_type==XML){
      switch(gameCommand.command){
        case START:
          if(alzXMLGameClass::game_status==STOPPED || alzXMLGameClass::game_status==PAUSED){
            alzXMLGameClass::game_status=ACTIVE;
            if(this->game_subType==XML_INTERACTIVE)
              alzXMLGameClass::startProactiveGame();
            else
              alzXMLGameClass::startGame();
          }
          break;
        case STOP:
            alzXMLGameClass::game_status=STOPPED;
          break;
        case PAUSE:
            alzXMLGameClass::game_status=PAUSED;
          break;
      }
  }
}
void alzXMLGameClass::playGame(int argc, char** argv,string game, string package, int game_subType){
  cout << "XML -- PLAY GAME\n";

  this->package = package;
  this->game_name = game;
  this->game_subType = game_subType;
  ros::init(argc, argv, "alz_game_xml_node");
  ros::NodeHandle n;
  etts_say_text = n.advertise<etts_msgs::Utterance>("etts", 100);
  tablet_player = n.advertise<tv_on_demand::multimediaPlayer>("tablet_player", 100);
  make_gesture = n.advertise<std_msgs::String>("keyframe_gesture_filename", 100);


  internalFeedback_pub = n.advertise<memory_game_handler::Feedback>("GAME_INTERNAL_FEEDBACK",100);
cout <<"XML me subscribo\n";
  internalCommand_sub = n.subscribe("GAME_INTERNAL_COMMAND", 1, &alzXMLGameClass::handleGame, this);
}

bool alzXMLGameClass::isAGameCommand(string msg){
  cout << "XML -- ISGAMECOMMAND\n";
    return true;
}
int alzXMLGameClass::handleGameCommand(string command){
  if(this->game_status==ACTIVE){
    cout<< "XML -- HANDLEGAMECOMMAND\n";
  }
  return this->game_status;
}
