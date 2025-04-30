import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import matplotlib.colors as mcolors

def leermatriz(filename):
    """
    Lee el archivo de matrices para una sola temperatura.
    Devuelve una lista de matrices y la temperatura.
    """
    matrices = []
    temperatura = None  # Inicializar la temperatura como None
    with open(filename, 'r') as file:
        lines = file.readlines()
        matriz = []

        for line in lines:
            line = line.strip()
            if line.startswith("#"):  # Leer la temperatura del encabezado
                if "T =" in line and temperatura is None:  # Leer solo la primera vez
                    temperatura = float(line.split("=")[1].strip())
                continue
            if line:  # Si la línea no está vacía, es parte de la matriz
                matriz.append([int(x) for x in line.split()])
            else:  # Si la línea está vacía, significa que termina una matriz
                if matriz:
                    matrices.append(matriz)
                    matriz = []

        # Guardar la última matriz si no está vacía
        if matriz:
            matrices.append(matriz)

    return matrices, temperatura

def mostraranimacion(matrices, temperatura, fps):
    """
    Muestra una animación en tiempo real a partir de las matrices de spins.
    Cada frame corresponde a un paso temporal.
    """
    # Crear un mapa de colores personalizado (azul claro y azul oscuro)
    colors = ["#00008B", "#ADD8E6"]  # Azul oscuro y azul claro
    cmap = mcolors.LinearSegmentedColormap.from_list("custom_blue", colors, N=2)

    # Configurar la figura
    fig, ax = plt.subplots()
    ax.axis('off')  # Ocultar los ejes

    def actualizar(frame):
        matriz = frame
        ax.clear()
        ax.axis('off')
        ax.set_title(f"Temperatura: {temperatura:.2f}", fontsize=16)
        ax.imshow(matriz, cmap=cmap, vmin=-1, vmax=1)

    # Crear los frames
    frames = [np.array(matriz) for matriz in matrices]

    # Crear la animación
    anim = animation.FuncAnimation(fig, actualizar, frames=frames, interval=1000 // fps, repeat=False)

    # Mostrar la animación en tiempo real
    plt.show()

# Leer el archivo espines.txt
matrices, temperatura = leermatriz("C:/Users/Turkipollito/Escritorio/Universidad/3Fisica/Segundocuatri/Computacional/REPOSITORIO/Grupo_Compu/programa_Ising/programaJavier/espines.txt")

# Mostrar la animación
mostraranimacion(matrices, temperatura=temperatura, fps=50)