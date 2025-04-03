import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Read the output file
positions = {}
with open("c:/Users/diego/Desktop/Fisica_Computacional/GrupoCompu/Grupo_Compu/DiegodPruebas/output.txt", "r") as file:
    for line in file:
        if line.startswith("Body"):
            parts = line.split()
            body_id = int(parts[1][:-1])  # Extract body ID
            x, y = map(float, parts[2:])  # Only unpack x and y
            if body_id not in positions:
                positions[body_id] = {"x": [], "y": []}
            positions[body_id]["x"].append(x)
            positions[body_id]["y"].append(y)

# Prepare the figure and axis for the animation
fig1, ax1 = plt.subplots(figsize=(8, 8))
ax1.set_xlim(-2e11, 2e11)  # Adjust limits based on your simulation
ax1.set_ylim(-2e11, 2e11)
ax1.set_xlabel("X Position (m)")
ax1.set_ylabel("Y Position (m)")
ax1.set_title("Animated Trajectories of Celestial Bodies")
ax1.grid()

# Create scatter plots for each body
scatters = []
for body_id in positions:
    scatter, = ax1.plot([], [], 'o', label=f"Body {body_id}")
    scatters.append(scatter)

# Create a legend
ax1.legend()

# Update function for animation
def update(frame):
    for body_id, scatter in enumerate(scatters):
        scatter.set_data(positions[body_id]["x"][:frame], positions[body_id]["y"][:frame])
    return scatters

# Create the animation
num_frames = len(next(iter(positions.values()))["x"])  # Number of steps in the simulation
ani = FuncAnimation(fig1, update, frames=range(0, num_frames, 2), interval=25, blit=True)  # Faster animation

# Show the animation
plt.show()

# Read the energy file
steps = []
energies = []
with open("c:/Users/diego/Desktop/Fisica_Computacional/GrupoCompu/Grupo_Compu/DiegodPruebas/energy.txt", "r") as file:
    for line in file:
        step, energy = map(float, line.split())
        steps.append(step)
        energies.append(energy)

# Plot the energy in a separate figure
fig2, ax2 = plt.subplots(figsize=(8, 6))
ax2.plot(steps, energies, label="Total Energy")
ax2.set_xlabel("Simulation Step")
ax2.set_ylabel("Total Energy (Joules)")
ax2.set_title("Total Energy Over Time")
ax2.legend()
ax2.grid()

# Show the energy plot
plt.show()