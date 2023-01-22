#!/usr/bin/env python
# coding: utf-8


import zmq
import time
import numpy as np
from DigitalPID import DigitalPID

class ZMQPublisher:
    def __init__(self, pub_topic='pubtopic', pub_port = 5555, sub_host="127.0.0.1", sub_topic = 'subtopic', 
            sub_port='5556', P = 100.0, I = 1.0, D = 1.0):

        self.pub_port = pub_port
        self.pub_topic = pub_topic
        self.sub_port = sub_port
        self.sub_topic = sub_topic
        
        self.sub_host = sub_host
        

        self.context = zmq.Context()
        self.publisher = self.context.socket(zmq.PUB)
        self.publisher.bind("tcp://*:{}".format(self.pub_port))
        
        self.subscriber = self.context.socket(zmq.SUB)
        self.subscriber.connect("tcp://{}:{}".format(self.sub_host, self.sub_port))
        self.subscriber.setsockopt_string(zmq.SUBSCRIBE, self.sub_topic)
        
        self.poller = zmq.Poller()
        self.poller.register(self.subscriber, zmq.POLLIN)


        self.reference = 0.0
        self.previous_data = 0.0
        self.current_data = 0.0

        self.P = P
        self.I = I
        self.D = D
        self.N = 20
        self.Ts = 0.05

        self.PID = DigitalPID(self.P, self.I, self.D, self.Ts, self.N)

        self.t0 = time.time_ns() / (10 ** 9) 


    def control(self):

        topic = None
        message = None
        while True:
            # timeout in miliseconds
            if self.poller.poll(timeout=self.Ts*1000):
                L = self.subscriber.recv_string(zmq.DONTWAIT).split()
                #topic, message = self.subscriber.recv_string(zmq.DONTWAIT).split()
                if(len(L) == 1):
                    if(L[0] == self.sub_topic):
                        topic = L[0]
                        continue
                    else:
                        message = L[0]

                if(topic != self.sub_topic):
                    self.current_data = self.previous_data
                
                else:
                    if len(message)> 0:
                        try:
                            self.current_data = float(message)
                            self.previous_data = self.current_data
                        except ValueError as ve:
                            self.current_data = self.previous_data
                    else:
                        self.current_data = self.previous_data
            else:
                # in case of time out, use previous data
                self.current_data = self.previous_data

            t_now = time.time_ns() / (10 ** 9)
            F = np.sin( t_now - self.t0)
            error = self.reference - self.current_data
            control_signal = self.PID.control(error)
            u = F + control_signal

            self.publisher.send_string("{} {}".format(self.pub_topic, u))
            time.sleep(self.Ts)


ZPUB = ZMQPublisher(pub_topic = 'input', pub_port = 4848, sub_host='localhost', sub_topic = 'angle', sub_port = 4849)
ZPUB.control()
