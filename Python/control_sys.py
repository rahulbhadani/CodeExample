#!/usr/bin/env python
# coding: utf-8
# author: Rahul Bhadani


import control as ct
import numpy as np
import matplotlib.pyplot as plt

num = [-0.06, 0.4]
denom = [1, -1.6, 0.78]
Ts = 0.01
sys = ct.tf(num, denom, Ts)

T_final = 1
T1 = np.arange(0, T_final, Ts)
X0 = 0.0
T2, yout = ct.impulse_response(sys, T1, X0)
plt.step(T2, yout)
plt.show()
T3, yout2 = ct.step_response(sys, T1, X0)
plt.step(T3, yout2)
plt.show()

M = 0.5;
m = 0.2;
b = 0.1;
I = 0.06;
g = 9.8;
l = 0.3;
q = (M+m)*(I+m*(l**2)) - ((m*l)**2);

s = ct.tf('s')
P_pend = (m*l*s/q)/((s**3) + (b*(I + m*(l**2)))*(s**2)/q - ((M + m)*m*g*l)*s/q - b*m*g*l/q);
T_final = 1
T1 = np.arange(0, T_final, Ts)
X0 = 0.0
T4, yout4 = ct.impulse_response(P_pend, T1, X0)
MM = ct.forced_response(P_pend, [0.4, 0.45], [3.0, 3.5], X0)

print(MM)
print(MM.t)
print(MM.y)
print(MM.x)

