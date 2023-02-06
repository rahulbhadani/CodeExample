#!/usr/bin/env python
# coding: utf-8

import matlab.engine
eng = matlab.engine.start_matlab()

from simple_pid import PID

#import control
#from control.matlab import *

P = 100.0
I = 1.0
D = 1.0

mPID = eng.pid(100,1,1,0.5)

theta=eng.lsim(mPID, matlab.double([0.3, 0.1, 0.4, 0.5, 0.223, 0.244, 20.4]), matlab.double([0, 0.01]));

#num = [D, P, I]
#denom = [1, 0]

#sys1 = tf(num, denom)
#U = [0.3, 0.1]
#T= [0, 0.01]
#X0 = 0

#yout, T, xout = lsim(sys1, U, T, X0)


print(theta)
#print(yout)


pid = PID(P, I, D, setpoint=0)

output = pid(0.1)

print(output)

