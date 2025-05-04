import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# === CARGAR DATOS ===

# Cargar spins
with open('C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/spins_all_temps.txt', 'r') as f:
    lines = f.readlines()

frames = []
frame = []
for line in lines:
    if line.startswith("# T ="):
        if frame:
            frames.append(np.array(frame))
            frame = []
    elif line.strip():
        frame.append([int(i) for i in line.strip().split()])
if frame:
    frames.append(np.array(frame))  # añadir el último frame

# Inferir tamaño cuadrado de la matriz
N = int(np.sqrt(len(frames[0].flatten())))

# Cargar datos de Eprom y Mprom
data = np.loadtxt('C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/ising_data.txt', skiprows=1)
T, Eprom, Mprom = data[:, 0], data[:, 1], data[:, 2]

# === GRAFICAR ===

fig, axs = plt.subplots(2, 2, figsize=(10, 8))
ax_spin = axs[0, 0]
ax_blank = axs[0, 1]
ax_E = axs[1, 0]
ax_M = axs[1, 1]

# Subplot de espines
im = ax_spin.imshow(frames[0], cmap='gray', vmin=-1, vmax=1)
ax_spin.set_title('Spins (-1 negro, 1 blanco)')
ax_spin.axis('off')

# Subplot en blanco
ax_blank.axis('off')
ax_blank.set_title('Subplot en blanco')

# Subplot Eprom
line_E, = ax_E.plot([], [], 'r-')
ax_E.set_xlim(0, len(Eprom))
ax_E.set_ylim(min(Eprom), max(Eprom))
ax_E.set_title('Energía Promedio')
ax_E.set_xlabel('Paso')
ax_E.set_ylabel('E')

# Subplot Mprom
line_M, = ax_M.plot([], [], 'b-')
ax_M.set_xlim(0, len(Mprom))
ax_M.set_ylim(min(Mprom), max(Mprom))
ax_M.set_title('Magnetización Promedio')
ax_M.set_xlabel('Paso')
ax_M.set_ylabel('M')

# === ANIMACIÓN ===
def update(frame_idx):
    im.set_data(frames[frame_idx])
    line_E.set_data(range(frame_idx + 1), Eprom[:frame_idx + 1])
    line_M.set_data(range(frame_idx + 1), Mprom[:frame_idx + 1])
    return im, line_E, line_M

ani = FuncAnimation(fig, update, frames=len(frames), interval=300, blit=False)

plt.tight_layout()
plt.show()
