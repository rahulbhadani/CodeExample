#!/usr/bin/env python
# coding: utf-8


import zmq
import time
import numpy as np
from ZTF import ZTF
from ZTF import PID
import ctypes

libc = ctypes.CDLL('libc.so.6')


class ZMQPublisher:
    def __init__(self, pub_topic='pubtopic', pub_port = 5555, sub_host="127.0.0.1", sub_topic = 'subtopic', 
            sub_port='5556', P = 100.0, I = 1.0, D = 1.0, N = 10, Ts = 0.05):

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
        self.N = N
        self.Ts = Ts

        print("P = {}".format(self.P))
        print("I = {}".format(self.I))
        print("D = {}".format(self.D))
        print("N = {}".format(self.N))
        print("Ts = {}".format(self.Ts))


        self.pid = PID(self.P, self.I, self.D, self.N, self.Ts)

        # self.ZTF1 = ZTF([self.P], [1.0])
        # self.ZTF2 = ZTF([self.I*self.Ts], [-1.0, 1.0])
        # self.ZTF3 = ZTF([-self.D*self.N, self.D*self.N], [(self.N*self.Ts- 1), 1])



        self.t0 = time.time_ns() / (10 ** 9) 


    def control(self):

        topic = None
        message = None
        while True:
            t_now_main = time.time_ns() / (10 ** 9)
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
            # F = np.sin( t_now - self.t0)
            error = self.reference - self.current_data
            control_signal = self.pid.processing(error)
            u = control_signal

            self.publisher.send_string("{} {}".format(self.pub_topic, u))
            t_now_main2 = time.time_ns() / (10 ** 9)
           

            time_to_sleep_inmicrosec = (self.Ts - (t_now_main2 - t_now_main))*1e6 
            if(time_to_sleep_inmicrosec > 0):
                libc.usleep( int(time_to_sleep_inmicrosec))
            #time.sleep(self.Ts)


ZPUB = ZMQPublisher(pub_topic = 'input', pub_port = 4848, sub_host='localhost', sub_topic = 'angle', sub_port = 4849, 
        P = 12.2285752621432, I = 7.40871870543199, D = 4.88097496311707, N = 37.0569659936971, Ts = 0.05 )
ZPUB.control()
