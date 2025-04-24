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
anim = FuncAnimation(fig, update, frames=len(estados), interval=5, repeat=False)

# Guardar el video sin cv2
plt.show()
