import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
# Set number of planets
n_planets = 8  # You have 8 planets

# File path
file_path = "C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/sist_solar_teresa/planetas"

# Read the file and strip out blank lines
with open(file_path, "r") as file:
    lines = [line.strip() for line in file if line.strip()]

# Check that the number of lines is correct
if len(lines) % n_planets != 0:
    raise ValueError("Number of lines is not divisible by number of planets!")

# Process blocks of data
planet_pos = []
for i in range(0, len(lines), n_planets):
    group = []
    for line in lines[i:i + n_planets]:
        x, y = map(float, line.replace(",", "").split())
        group.extend([x, y])
    planet_pos.append(group)

# Convert to a NumPy array
planet_pos = np.array(planet_pos)

# Print result
print("planet_pos array:")
print(planet_pos)

n_frames = planet_pos.shape[0]

# === PREPARE DATA ===
# Each planet gets its own trajectory
trajectories_x = [planet_pos[:, 2*i] for i in range(n_planets)]
trajectories_y = [planet_pos[:, 2*i + 1] for i in range(n_planets)]

# === SETUP PLOT ===
fig, ax = plt.subplots(figsize=(8, 8))
ax.set_aspect("equal")
ax.set_xlim(-35, 35)
ax.set_ylim(-35, 35)
ax.set_facecolor("black")

# Unique colors per planet
colors = plt.cm.tab10(np.linspace(0, 1, n_planets))

# Create plot elements
planet_dots = [ax.plot([], [], 'o', color=colors[i], markersize=6)[0] for i in range(n_planets)]
planet_trails = [ax.plot([], [], '-', color=colors[i], linewidth=1)[0] for i in range(n_planets)]

# === INIT FUNCTION ===
def init():
    for dot, trail in zip(planet_dots, planet_trails):
        dot.set_data([], [])
        trail.set_data([], [])
    return planet_dots + planet_trails

# === ANIMATION FUNCTION ===
def update(frame):
    for i in range(n_planets):
        x = trajectories_x[i][frame]
        y = trajectories_y[i][frame]
        planet_dots[i].set_data(x, y)
        planet_trails[i].set_data(trajectories_x[i][:frame+1], trajectories_y[i][:frame+1])
    return planet_dots + planet_trails

# === ANIMATE ===
ani = animation.FuncAnimation(fig, update, frames=n_frames, init_func=init, blit=True, interval=300)

# === SAVE VIDEO ===
ani.save("planet_animation.mp4", writer='ffmpeg', fps=1)
# Or save as gif:
# ani.save("planet_animation.gif", writer='pillow')

plt.close()