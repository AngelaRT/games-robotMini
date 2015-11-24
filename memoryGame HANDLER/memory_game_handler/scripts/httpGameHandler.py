#!/usr/bin/env python
from BaseHTTPServer import BaseHTTPRequestHandler
import Queue
import rospy
from std_msgs.msg import String

class PostHandler(BaseHTTPRequestHandler):
    def __init__(self, grh, *args):
        self.grh = grh
        BaseHTTPRequestHandler.__init__(self, *args)

    def sendHeaders(self):
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Content-type',    'text/html')
        self.end_headers()
        self.wfile.write('Client: %s\n' % str(self.client_address))
        self.wfile.write('User-agent: %s\n' % str(self.headers['user-agent']))
        self.wfile.write('Path: %s\n' % self.path)

    def do_OPTIONS(self):  
        self.send_response(200, "ok")       
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header("Access-Control-Allow-Headers", "X-Requested-With") 

    def do_POST(self):
        if self.grh.getStart():
            content_len = int(self.headers.getheader('content-length', 0))
            post_body = self.rfile.read(content_len)
            #I can receive two types of POST:
            # - Ask for change phase
            # - Ask for any action to do --> we answer to this one
            if not self.grh.setPhase(post_body) and not self.grh.isSuccessMsg(post_body):
                self.sendHeaders()
                phase=self.grh.getPhase()
                if phase==1 and self.grh.getLevel()!="zero":
                    self.wfile.write('Level: %s' % self.grh.getLevel())
                elif phase==2:
                    self.wfile.write('Requested content: %s' % self.grh.getContent())
                elif phase==3 and self.grh.getRestart():
                    self.wfile.write('Restart: ok')
                    self.grh.reset()
        else:
            self.sendHeaders()  
            self.wfile.write('Requested content: -1')          

def handleWrap(grh):
  return lambda *args: PostHandler(grh, *args)

class HTTPHandler():
    def __init__(self,grh):
        from BaseHTTPServer import HTTPServer
        postHandlerClass = handleWrap(grh)
        ip = grh.getIP()
        port = grh.getPort()
        server = HTTPServer((ip,port), postHandlerClass)
        print '>>> %s[HTTPServer] Starting server on %s:%s, use <Ctrl-C> to stop' % (grh.getStartString(),ip,port)
        server.serve_forever()
        
   
