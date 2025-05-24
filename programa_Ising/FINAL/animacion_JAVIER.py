import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.colors import LinearSegmentedColormap

# === CARGAR DATOS ===

# Cargar spins
with open(r'C:\Users\molin\Escritorio\Universidad\3Fisica\Segundocuatri\Compu\Grupo_Compu\programa_Ising\FINAL\spins_all_temps', 'r') as f:
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
data = np.loadtxt(r'C:\Users\molin\Escritorio\Universidad\3Fisica\Segundocuatri\Compu\Grupo_Compu\programa_Ising\FINAL\ising_data.txt', skiprows=1)
T, Eprom, Mprom = data[:, 0], data[:, 1], data[:, 2]

# === GRAFICAR ===

fig, axs = plt.subplots(2, 2, figsize=(10, 8))
ax_spin = axs[0, 0]
ax_blank = axs[0, 1]
ax_E = axs[1, 0]
ax_M = axs[1, 1]

custom_cmap = LinearSegmentedColormap.from_list("custom_blue", ["#5DADE2", "#154360"])  # Adjusted light and dark blue
# Subplot de espines
im = ax_spin.imshow(frames[0], cmap=custom_cmap, vmin=-1, vmax=1)
ax_spin.set_title(f'Red de {N} spins')
ax_spin.axis('off')

# Subplot en blanco
# Cargar datos del archivo con T, step y medidas
with open(r'C:\Users\molin\Escritorio\Universidad\3Fisica\Segundocuatri\Compu\Grupo_Compu\programa_Ising\FINAL\datos_red.txt', 'r') as f:
    T, step, medidas = map(float, f.readline().strip().split())

ax_blank.axis('off')
ax_blank.text(0.5, 0.5, f'T = {T}, Step = {step}, Medidas = {medidas} \n No thread optimization \n AMD Ryzen 5 1600 Six-Core Processor', fontsize=12, ha='center', va='center', transform=ax_blank.transAxes)

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

max_frames = max(len(frames), len(Eprom), len(Mprom))

# === ANIMACIÓN ===
def update(frame_idx):
    # Para los frames de spins, mantén el último frame si hemos llegado al final
    spin_idx = min(frame_idx, len(frames) - 1)
    im.set_data(frames[spin_idx])
    
    # Para las series temporales, muestra solo hasta el frame actual o el último disponible
    e_idx = min(frame_idx + 1, len(Eprom))
    m_idx = min(frame_idx + 1, len(Mprom))
    
    line_E.set_data(range(e_idx), Eprom[:e_idx])
    line_M.set_data(range(m_idx), Mprom[:m_idx])
    
    # Actualizar el título del subplot en blanco para mostrar el progreso
    progress = f"Frame: {frame_idx+1}/{max_frames}"
    ax_blank.set_title(f'{progress}')
    
    return im, line_E, line_M

ani = FuncAnimation(fig, update, frames=len(frames), interval=0.1, blit=False)

plt.tight_layout()
plt.show()
ani.save(r'C:\Users\molin\Escritorio\Universidad\3Fisica\Segundocuatri\Compu\Grupo_Compu\programa_Ising\FINALanimaciones\J animacion T = {}, Step = {}, Medidas = {}, Spins = {}.mp4'.format(T, step, medidas, N), writer='ffmpeg', fps=30)