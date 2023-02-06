#!/usr/bin/env python
# coding: utf-8

# author: Rahul Bhadani

class DigitalPID:
    """
    Implementing a Digital PID Controller based on backward euler method
    
    b0 = P( 1 + N*Ts) + I*Ts*(1 + N*Ts) + D*N
    b1 = -( P *(2 + N*Ts) + I*Ts + 2*D*N )
    b2 = P + D*N
    a0 = (1 + N*Ts)
    a1 = -(2 + N*Ts)
    a2 = 1

    u[k]  = -((a1/a0)*u[k-1]) -((a2/a0)*u[k-2]) + ((b0/b1)*e[k]) + ((b1/a0)*e[k-1]) + ((b2/a0)*e[k-2])

    """
    def __init__(self, P, I, D, Ts, N, **kwargs):
        """
        Parameters
        ----------

        """
        self.P = P
        self.I = I
        self.D = D
        self.Ts = Ts
        self.N = N

        self.b0 = P*(1 + N*Ts) + I*Ts*(1 + N*Ts) + D*N
        self.b1 = -( P *(2 + N*Ts) + I*Ts + 2*D*N )
        self.b2 = P + D*N
        self.a0 = (1 + N*Ts)
        self.a1 = -(2 + N*Ts)
        self.a2 = 1.0

        self.ku1 = self.a1/self.a0
        self.ku2 = self.a2/self.a0
        
        self.ke0 = self.b0/self.a0
        self.ke1 = self.b1/self.a0
        self.ke2 = self.b2/self.a0

        self.u = 0.0
        self.u_old = 0.0
        self.u_old_old = 0.0

        # [ e[k-2], e[k-1], e[k] ]
        self.e = 0.0
        self.e_old = 0.0
        self.e_old_old = 0.0


        self.UMAX = 3
        self.UMIN = -3


    def control(self, error):

        """
        Produces the controller output

        """

        self.e = error
        self.e_old_old = self.e_old
        self.e_old = self.e
        self.u_old_old = self.u_old
        self.u_old = self.u
        self.u = - (self.ku1*self.u_old) - (self.ku2*self.u_old_old) + self.ke0*self.e + self.ke1*self.e_old + self.ke2*self.e_old_old

        return self.u
