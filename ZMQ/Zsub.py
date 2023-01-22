#!/usr/bin/env python
# coding: utf-8



import zmq

class ZMQSubscriber:
    def __init__(self, port=5555, topic='mytopic'):
        self.port = port
        self.topic = topic
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.SUB)
        self.socket.connect("tcp://localhost:{}".format(self.port))
        self.socket.setsockopt_string(zmq.SUBSCRIBE, self.topic)

    def listen(self):
        topic = None
        message = None
        while True:
            L = self.socket.recv_string().split()
            
            if(len(L) == 1):
                if(L[0] == self.topic):
                    topic = L[0]
                    continue
                else:
                    message = L[0]
            
            print("Topic: {} | Message: {}".format(topic, message))


ZS =ZMQSubscriber(port = 4849, topic = 'angle')
ZS.listen()

