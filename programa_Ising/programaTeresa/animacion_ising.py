#("c:/Users/User/Documents/Fisica_compu/Compu/Grupo_Compu/Grupo_Compu/programa_Ising/programaMiguel/spins_T2.00.txt")

import numpy as np
import matplotlib.pyplot as plt
import re
from matplotlib.animation import FuncAnimation
from PIL import Image

# Ruta del archivo con todos los estados
archivo = "C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/spins_all_temps.txt"

# Leer y procesar el archivo
with open(archivo, "r") as f:
    bloques = f.read().strip().split("\n\n")

# Extraer la información de temperaturas y matrices
estados = []
temperaturas = []

for bloque in bloques:
    lineas = bloque.strip().split("\n")
    if not lineas or not lineas[0].startswith("#"):
        continue

    # Extraer temperatura desde el encabezado
    temp_match = re.search(r"T\s*=\s*([\d\.]+)", lineas[0])
    T = float(temp_match.group(1)) if temp_match else 0.0
    temperaturas.append(T)

    # Leer la matriz de espines
    matriz = np.array([[int(x) for x in fila.strip().split()] for fila in lineas[1:]])
    estados.append(matriz)

# Crear la figura para la animación
fig, ax = plt.subplots(figsize=(5, 5))
ax.axis('off')


cmap_usado = 'viridis'


# Función de actualización para la animación
def update(frame):
    ax.clear()  # Limpiar el eje
    ax.axis('off')  # Quitar los ejes
    img = (estados[frame] + 1) / 2  # Convertir de [-1,1] a [0,1]
    ax.imshow(img, cmap=cmap_usado, interpolation='nearest')
    ax.set_title(f"T = {temperaturas[frame]:.2f}")
    return ax,

# Crear la animación
anim = FuncAnimation(fig, update, frames=len(estados), interval=20, repeat=False)

# Guardar el video sin cv2
plt.show()




# Cambia esto por la ruta de tu archivo
ruta_archivo = 'C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/ising_data.txt'

# Leer datos del archivo
with open(ruta_archivo, 'r') as f:
    lineas = f.readlines()

# Ignorar la línea de encabezado
datos = [linea.strip().split() for linea in lineas if not linea.startswith('#')]

# Extraer columnas
Eprom = [float(d[1]) for d in datos]
Mprom = [float(d[2]) for d in datos]

# Crear figuras y ejes para los dos plots
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
fig.suptitle('Visualización en tiempo real de Eprom y Mprom')

# Inicializar líneas vacías
line1, = ax1.plot([], [], 'b-o', label='Eprom')
line2, = ax2.plot([], [], 'r-o', label='Mprom')

ax1.set_ylabel('Eprom')
ax2.set_ylabel('Mprom')
ax2.set_xlabel('Paso')

ax1.grid(True)
ax2.grid(True)
ax1.legend()
ax2.legend()

# Inicializar listas de datos que se irán mostrando
xdata, ydata1, ydata2 = [], [], []

# Mostrar en tiempo real
plt.ion()
plt.show()

for i in range(len(Eprom)):
    xdata.append(i)
    ydata1.append(Eprom[i])
    ydata2.append(Mprom[i])
    
    line1.set_data(xdata, ydata1)
    line2.set_data(xdata, ydata2)
    
    ax1.relim()
    ax1.autoscale_view()
    
    ax2.relim()
    ax2.autoscale_view()
    
    plt.pause(0.2)  # Velocidad de actualización (en segundos)

# Mantener la ventana abierta al final
plt.ioff()
plt.show()