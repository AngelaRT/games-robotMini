#!/usr/bin/env python
import sys
import rospy
from httpGameHandler import *
from std_msgs.msg import String
import threading
from genericGameRequestHandler import *

# This node listens to "alz/ask4Card" (published by alz_game_session when the user
# asks "Carta #" where # is the id of the card) and queues the request
# When the client (tablet) sends a POST, this node answers with
# the first element of the queue

class CardsRequestsHandler(GenericGameRequestsHandler):

    def __init__(self):
        GenericGameRequestsHandler.__init__(self)
        self.port=8080
        self.startString="memory_game"


if __name__ == "__main__":
    crh=CardsRequestsHandler();
    t2=threading.Thread(target=HTTPHandler, args=(crh,))
    t2.start()
    crh.listener()
