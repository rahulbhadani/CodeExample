import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors

# Define the function
def f(t):
    return np.exp(8j*t) + 6*np.exp(-4j*t) + 4*np.exp(5j*t)

# Define the time range
t = np.arange(0, 2*np.pi, 0.001)


# Evaluate the function at each time point
z = f(t)

# Extract real and imaginary parts
real_parts = np.real(z)
imag_parts = np.imag(z)

# Create a custom colormap from blue to red to blue
cmap = mcolors.LinearSegmentedColormap.from_list("", ["black", "gray", "black"])

# Create a figure
fig, ax = plt.subplots(figsize=(50, 50))


# Plot the real and imaginary parts as a line with colormap
for i in range(len(t)-1):
    x = [real_parts[i], real_parts[i+1]]
    y = [imag_parts[i], imag_parts[i+1]]
    color = cmap(i/len(t))  # map segment to color
    plt.plot(x, y, color=color, linewidth=10)

# Fill the area within the curve with a color
#plt.fill_between(real_parts, imag_parts, color='black', alpha=1.0)
plt.fill(np.append(real_parts, real_parts[0]), np.append(imag_parts, imag_parts[0]), color='#ff8fab', alpha=0.2)


# Set axis limits
plt.axis('equal')
plt.axis('off')
plt.xlim(-15, 15)
plt.ylim(-15, 15)

fig.savefig('aarc_lab_logo.png', transparent=True)
fig.savefig('aarc_lab_logo.eps')
fig.savefig('aarc_lab_logo.svg')

# Show the plot
plt.show()