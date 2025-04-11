import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Read the output file
positions = {}
with open("c:/Users/diego/Desktop/Fisica_Computacional/GrupoCompu/Grupo_Compu/DiegodPruebas/output.txt", "r") as file:
    for line in file:
        if line.startswith("Body"):
            parts = line.split()
            body_id = int(parts[1][:-1])  # Extract body ID
            x, y, z = map(float, parts[2:])
            if body_id not in positions:
                positions[body_id] = {"x": [], "y": []}
            positions[body_id]["x"].append(x)
            positions[body_id]["y"].append(y)

# Prepare the figure and axis
fig, ax = plt.subplots(figsize=(8, 8))
ax.set_xlim(-0.5e13, 0.5e13)  # Adjust limits based on your simulation
ax.set_ylim(-0.5e13, 0.5e13)
ax.set_xlabel("X Position (m)")
ax.set_ylabel("Y Position (m)")
ax.set_title("Animated Trajectories of Celestial Bodies")
ax.grid()

# Create scatter plots for each body
scatters = []
for body_id in positions:
    scatter, = ax.plot([], [], 'o', label=f"Body {body_id}")
    scatters.append(scatter)

# Create a legend
ax.legend()

# Update function for animation
def update(frame):
    for body_id, scatter in enumerate(scatters):
        scatter.set_data(positions[body_id]["x"][:frame], positions[body_id]["y"][:frame])
    return scatters

# Create the animation
num_frames = len(next(iter(positions.values()))["x"])  # Number of steps in the simulation
ani = FuncAnimation(fig, update, frames=num_frames, interval=50, blit=True)

# Show the animation
plt.show()