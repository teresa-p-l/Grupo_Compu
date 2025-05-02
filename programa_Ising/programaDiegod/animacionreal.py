import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from matplotlib.colors import LinearSegmentedColormap

def leer_matrices(filename):
    """
    Lee el archivo de matrices para una sola temperatura.
    Devuelve una lista de matrices.
    """
    matrices = []
    with open(filename, 'r') as file:
        lines = file.readlines()[1:]  # Ignorar la primera línea
        matriz = []

        for line in lines:
            line = line.strip()
            if line:  # Si la línea no está vacía, es parte de la matriz
                matriz.append([int(x) for x in line.split()])
            else:  # Si la línea está vacía, significa que termina una matriz
                if matriz:
                    matrices.append(matriz)
                    matriz = []

        # Guardar la última matriz si no está vacía
        if matriz:
            matrices.append(matriz)

    return matrices

def mostrar_animacion_tiempo_real(matrices, temperatura, fps=5):
    """
    Muestra una animación en tiempo real a partir de las matrices de spins.
    Cada frame corresponde a un paso temporal.
    """
    # Crear un mapa de colores personalizado (azul claro y azul oscuro)
    colors = ["#00008B", "#ADD8E6"]  # Azul oscuro y azul claro
    cmap = LinearSegmentedColormap.from_list("custom_blue", colors, N=2)

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
    frames = [np.array(matriz, dtype=np.int8) for matriz in matrices]

    # Crear la animación
    anim = animation.FuncAnimation(fig, actualizar, frames=frames, interval=1000 // fps, repeat=False)

    # Mostrar la animación en tiempo real
    plt.show()

# Ejemplo de uso
if __name__ == "__main__":
    archivo = "c:/Users/diego/Desktop/Fisica_Computacional/GrupoCompu/Grupo_Compu/programa_Ising/programaDiegod/spinsSOLOUNA.txt"
    temperatura = 2.26  # Cambia esto según la temperatura correspondiente
    fps = 24  # Cambia este valor para ajustar la velocidad de la animación

    matrices = leer_matrices(archivo)
    mostrar_animacion_tiempo_real(matrices, temperatura, fps)