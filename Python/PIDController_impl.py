#!/usr/bin/env python
# coding: utf-8
# author: Rahul Bhadani

import control as ct
from control.matlab import *

import numpy as np
from DigitalPID import DigitalPID
import matplotlib.pyplot as plt
from Ztransform import Ztransform

P = 100
I = 1
D = 1
N = 20
Ts = 0.01
T_final = 1

T = np.arange(0, T_final, Ts)
reference = np.full( T.size , 0.0)
digitalpid = DigitalPID(P, I, D, Ts, N)
phi = 0
#F = np.sin(T)
F = np.full( T.size , 0.0)
F[5] = 1.0
y_array2 = np.zeros( T.size)

M = 0.5;
m = 0.2;
b = 0.1;
I = 0.06;
g = 9.8;
l = 0.3;
q = (M+m)*(I+m*(l**2)) - ((m*l)**2);

s = ct.tf('s')
P_pend = (m*l*s/q)/((s**3) + (b*(I + m*(l**2)))*(s**2)/q - ((M + m)*m*g*l)*s/q - b*m*g*l/q);

Pz = c2d(P_pend, Ts)

Z_Plant = Ztransform( list(Pz.num[0][0]) , list(Pz.den[0][0]))


u_old = 0
for i, t in enumerate(T):
    
    v = F[i]   
    error = reference[i] - phi
    x = digitalpid.control(error)
    
    #print(type(v.item()))

    u = v.item() + x.item()

    #T4, phi_arr = ct.forced_response(P_pend, [0, Ts], [u_old, u], phi)
    
    phi = Z_Plant.control(u)

    #phi = phi_arr[1]
    y_array2[i] = phi
    v_old = v


plt.plot(T, F,'r')
plt.plot(T, y_array2, 'b')
plt.legend(['reference', 'phi'])
plt.show()
