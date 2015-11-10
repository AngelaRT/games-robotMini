#!/usr/bin/env python
import sys
import rospy
from httpGameHandler import *
from std_msgs.msg import String
import threading
from genericGameRequestHandler import *

# This node listens to "alz/ask4Diff" (published by alz_game_session when the user
# indicates any difference and queues the request
# When the client (tablet) sends a POST, this node answers with
# the first element of the queue

class DifferencesRequestsHandler(GenericGameRequestsHandler):

    def __init__(self):
        GenericGameRequestsHandler.__init__(self)
        self.port=9090
        self.level=1
        self.phase=2          


if __name__ == "__main__":
	drh=DifferencesRequestsHandler();
	t2=threading.Thread(target=HTTPHandler, args=(drh,))
	t2.start()
	drh.listener()
