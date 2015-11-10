#!/usr/bin/env python
import sys
import rospy
from httpGameHandler import *
from std_msgs.msg import String
import threading

# All game handler classes wich use http to communicate with front-end browser
# and listen to a ros topic, should inheritate this class

class GenericGameRequestsHandler():

    def __init__(self):
        self.q=Queue.Queue()
        self.level="zero"
        self.phase=1
        self.restart=False
        self.ip='192.168.1.33'
        self.port=8080

    def reset(self):
        self.level="zero"
        self.phase=1
        self.restart=False       
    def setRestart(self):
        self.restart=True
        self.level="zero"
    def getRestart(self):
        return self.restart
    def setLevel(self,level):
        self.level=level
    def getLevel(self):
        return self.level
    def getIP(self):
        return self.ip
    def getPort(self):
        return self.port
    def setPhase(self,post_body):
    #Change the phase when we receive a post with the following content:
    # "Phase X", where X is the ID of the phase
        if "Phase" in post_body:
            self.phase=int(post_body[6]);
            print "Phase %d"%self.phase
            return True
        else:
            return False
    def getPhase(self):
        return self.phase
    def getContent(self):
        #If there is anything to be requested, we return it. If not, we return -1
        if not self.q.empty():
            return self.q.get()
        else:
            return -1
    def queueContent(self,reqContent):
        self.q.put(reqContent)

    def callback(self,reqContent):
        if self.getPhase()==1:
            #First, set the level
            print "Level: %s"%reqContent
            self.setLevel(reqContent.data)
        elif self.getPhase()==2:
            #Play the game --> requesting cards
            print "Requested content: %s"%reqContent
            # Queue the reqContent in the httpHandler
            self.queueContent(reqContent.data)
        else:
            #Finally, restart game
            print "Play again!!"
            self.setRestart()

    def listener(self):
        rospy.init_node('cardsRequestHandler', anonymous=True)
        print "Listening to alz/ask4GameCommand"
        rospy.Subscriber("ask4GameCommand", String, self.callback)
        rospy.spin()
