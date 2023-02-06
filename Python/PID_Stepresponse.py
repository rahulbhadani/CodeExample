import control
import numpy
import matplotlib.pyplot as plt

s = control.tf([1, 0], 1)
ts = numpy.linspace(0, 5)

def plot_step_response(G):
    t, y = control.step_response(G, ts)
    # Add some action before time zero so that the initial step is visible
    t = numpy.concatenate([[-1, 0], t])
    y = numpy.concatenate([[0, 0], y])
    plt.plot(t, y)
    plt.show()

K_C = 1

tau_I = 1

alpha = 0.1
tau_D = 1

Gc = K_C*(1 + 1/(tau_I*s) + 1*s/(alpha*tau_D*s + 1))


plot_step_response(Gc)

