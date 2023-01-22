#!/usr/bin/env python
# coding: utf-8


import zmq
import time
import numpy as np

class ZMQPublisher:
    def __init__(self, pub_topic='pubtopic', pub_port = 5555, Ts = 0.05):

        self.pub_port = pub_port
        self.pub_topic = pub_topic
        self.context = zmq.Context()
        self.publisher = self.context.socket(zmq.PUB)
        self.publisher.bind("tcp://*:{}".format(self.pub_port))
        self.Ts = Ts
        
    def publish(self, data):
        self.publisher.send_string("{} {}".format(self.pub_topic, data))
        time.sleep(self.Ts)

ZP = ZMQPublisher(pub_topic='phi2', pub_port=5556, Ts = 0.05)


t0 = time.time_ns() / (10 ** 9)

while True:
    t_now = time.time_ns() / (10 ** 9)
    Data = 3.0*np.sin(t_now -t0)
    ZP.publish(Data)

