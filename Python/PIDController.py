#!/usr/bin/env python
# coding: utf-8

# author: Rahul Bhadani

class PIDController:
    """
    Implementing a PID Controller

    u[k]  = u[k-1] + P(e[k] - e[k-1])  + Ts*I*e[k] + D*(edot[k] - edot[k-1])

    """
    def __init__(self, P, I, D, Ts, **kwargs):
        """
        Parameters
        ----------


        """
        self.P = P
        self.I = I
        self.D = D
        self.Ts = Ts

        self.uold = 0.0
        self.eold = 0.0
        self.edotold = 0.0

        


    def control(self, error):

        
        edot = (error - self.eold)/self.Ts
        u = self.uold + self.P*(error - self.eold) + self.Ts*self.I*error + self.D*(edot - self.edotold)

        self.uold = u
        self.eold = error
        self.edotold = edot
        
        return u
