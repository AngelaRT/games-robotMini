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
        self.ip='localhost'#'192.168.1.33'
        self.port=8080
        self.start=False
        self.successMsg="partialSuccess"
        self.commandResultPublisher = rospy.Publisher('reportCommandResult', String, queue_size=10)

    def reset(self):
        self.level="zero"
        self.phase=1
        self.restart=False 
        self.start=False      
    def setRestart(self):
        self.restart=True
        self.level="zero"
        self.start=False 
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
            if self.phase==3:
                self.commandResultPublisher.publish("a") #a=all --> we have found all the pairs
                self.setRestart()
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
    def getStart(self):
        return self.start
    def getStartString(self):
        return self.startString;

    def isSuccessMsg(self,post_body):
        if self.successMsg in post_body:
            self.commandResultPublisher.publish(post_body[len(self.successMsg)+1])
            return True
        else:
            return False        

    def callback(self,reqContent):
        print ">>>> %s Receiving %s at alz/ask4GameCommand"%(self.startString,reqContent.data)
        if reqContent.data == self.startString:
            self.start=True
        elif self.start:
            if self.getPhase()==1:
                #First, set the level
                self.setLevel(reqContent.data)
            elif self.getPhase()==2:
                #Play the game --> requesting cards
                # Queue the reqContent in the httpHandler
                self.queueContent(reqContent.data)
            else:
                #Finally, restart game
                self.setRestart()

    def listener(self):
        rospy.init_node('cardsRequestHandler', anonymous=True)
        print ">>>> %s Listening to alz/ask4GameCommand"%self.startString
        rospy.Subscriber("ask4GameCommand", String, self.callback)
        rospy.spin()
