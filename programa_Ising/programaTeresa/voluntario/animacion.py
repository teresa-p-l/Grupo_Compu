import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Parámetros a adaptar
filename    = "C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/voluntario/posiciones.txt"   # nombre de tu fichero
filename_energia = "C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/voluntario/energia.txt"   # nombre de tu fichero
N           = 20                 # número de partículas por “frame”
interval_ms = 100                # tiempo entre frames en ms
output_mp4  = "C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/voluntario/simulacion.mp4"   # nombre del fichero de salida

# --- 1) Leer y estructurar datos ---
# Cada bloque de N líneas, dos columnas x,y
data = np.loadtxt(filename)
n_frames = data.shape[0] // N

# reshaped: (n_frames, N, 2)
frames = data.reshape((n_frames, N, 2))

# --- 2) Preparar figura ---
fig, axs = plt.subplots(2, 2, figsize=(10, 8))
ax_pos = axs[0, 0]
ax_ekin = axs[0, 1]
ax_epot = axs[1, 0]
ax_etot = axs[1, 1]

scat = ax_pos.scatter([], [], s=50)

# Ajusta límites en función de tus datos
ax_pos.set_xlim( 0, 10)
ax_pos.set_ylim(0, 10)
ax_pos.set_xlabel('x'); ax_pos.set_ylabel('y')
ax_pos.set_title('Simulación de partículas')

# Función de inicialización
def init():
    scat.set_offsets(np.empty((0,2)))
    return scat,


# Leemos el archivo de energía y lo animamos

energia = np.loadtxt(filename_energia)

line_ekin, = ax_ekin.plot([], [], 'r-', label='Cinética')
line_epot, = ax_epot.plot([], [], 'g-', label='Potencial')
line_etot, = ax_etot.plot([], [], 'b-', label='Total')

for ax, title in zip([ax_ekin, ax_epot, ax_etot],
                     ['Energía Cinética', 'Energía Potencial', 'Energía Total']):
    ax.set_xlim(0, n_frames)
    ax.set_ylim(0, 2e+04)
    ax.set_title(title)
    ax.set_xlabel('Frame')
    ax.set_ylabel('Energía')
    ax.legend()





xdata = []
ekin_data = []
epot_data = []
etot_data = []

def update(frame_idx):
    x, y = data[frame_idx]
    scat.set_data(x, y)

    xdata.append(frame_idx)
    ekin_data.append(energia[frame_idx, 0])
    epot_data.append(energia[frame_idx, 1])
    etot_data.append(energia[frame_idx, 2])
    line_ekin.set_data(xdata, ekin_data)
    line_epot.set_data(xdata, epot_data)
    line_etot.set_data(xdata, etot_data)

    return scat, line_ekin, line_epot, line_etot

def update(frame_idx):
    xy = frames[frame_idx]
    scat.set_offsets(xy)
    ax_pos.set_title(f'Tiempo: frame {frame_idx+1}/{n_frames}')
    return scat,


# --- 3) Construir animación ---
anim = animation.FuncAnimation(fig, update,
                               frames=range(n_frames),
                               init_func=init,
                               interval=interval_ms,
                               blit=True)

# Mostrar por pantalla
plt.show()

# Guardar como vídeo mp4 (requiere ffmpeg instalado)
anim.save(output_mp4, writer='ffmpeg', dpi=150)
print(f'Vídeo guardado en {output_mp4}')
