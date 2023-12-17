import matplotlib.pyplot as plt

# Define the x values and corresponding y values based on the given switch points
switch_points = [100, 300]
x_max = 400  # Maximum x value for the plot

x = list(range(0, x_max + 1))  # x values from 0 to 400
y = []

current_y = 800  # Starting value
for point in x:
    if point in switch_points:
        current_y = 400 if current_y == 800 else 800  # Switch between 800 and 400
    y.append(current_y)

# Create the plot
plt.plot(x, y)
plt.xlabel('Simulation time (t)')
plt.ylabel('Link capacity (kB/s)')
plt.title('Link capacity over simulation time')
plt.grid(True)

# Set the axis limits
plt.xlim(0, x_max)  # Set x-axis limits
plt.ylim(0, 1000)  # Set y-axis limits

# Show the plot
plt.show()
