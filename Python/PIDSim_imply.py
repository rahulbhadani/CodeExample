#!/usr/bin/env python
# coding: utf-8

from PIDsim import PID
from scipy.integrate import odeint

import numpy as np
import matplotlib.pyplot as plt
# reactor temperature setpoint
Tsp = 390
Ea  = 72750     # activation energy J/gmol
R   = 8.314     # gas constant J/gmol/K
k0  = 7.2e10    # Arrhenius rate constant 1/min
V   = 100.0     # Volume [L]
rho = 1000.0    # Density [g/L]
Cp  = 0.239     # Heat capacity [J/g/K]
dHr = -5.0e4    # Enthalpy of reaction [J/mol]
UA  = 5.0e4     # Heat transfer [J/min/K]
q = 100.0       # Flowrate [L/min]
Cf = 1.0        # Inlet feed concentration [mol/L]
Tf  = 300.0     # Inlet feed temperature [K]
C0 = 0.5        # Initial concentration [mol/L]
T0  = 350.0;    # Initial temperature [K]
Tcf = 300.0     # Coolant feed temperature [K]
qc = 50.0       # Nominal coolant flowrate [L/min]
Vc = 20.0       # Cooling jacket volume

# do simulation at fixed time steps dt
dt = 0.05
ti = 0.0
tf = 8.0

# set initial conditions and cooling flow
IC = [C0,T0,Tcf]

# do simulation at fixed time steps dt
tstart = 0
tstop = 8
tstep = 0.05

# Arrhenius rate expression
def k(T):
    return k0*np.exp(-Ea/R/T)

def deriv(X,t):
    C,T,Tc = X
    dC = (q/V)*(Cf - C) - k(T)*C
    dT = (q/V)*(Tf - T) + (-dHr/rho/Cp)*k(T)*C + (UA/V/rho/Cp)*(Tc - T)
    dTc = (qc/Vc)*(Tcf - Tc) + (UA/Vc/rho/Cp)*(T - Tc)
    return [dC,dT,dTc]

# configure controller. Creates a PID object.
reactorPID = PID(Kp=8,Ki=30,Kd=5,MVrange=(0,300),DirectAction=True)

c,T,Tc = IC                                          # reactor initial conditions
qc = 150                                             # initial condition of the MV
for t in np.arange(tstart,tstop,tstep):              # simulate from tstart to tstop
    qc = reactorPID.update(t,Tsp,T,qc)               # update manipulated variable
    c,T,Tc = odeint(deriv,[c,T,Tc],[t,t+dt])[-1]     # start at t, find state at t + dt

reactorPID.manual()                                  # switch to manual model
T -= 10                                              # change process variable by -10 deg
for t in np.arange(t,t+1,tstep):                     # simulate for 1 minute
    qc = reactorPID.update(t,Tsp,T,qc)               # continue to update, SP tracks PV
    c,T,Tc = odeint(deriv,[c,T,Tc],[t,t+dt])[-1]     # start at t, find state at t + dt

reactorPID.auto()                                    # switch back to auto mode
for t in np.arange(t,t+tstop,tstep):                 # integrate another tstop minutes
    qc = reactorPID.update(t,Tsp,T,qc)               # update MV
    c,T,Tc = odeint(deriv,[c,T,Tc],[t,t+dt])[-1]     # start at t, find state at t + dt

# plot controller log
reactorPID.plot()

plt.show()
