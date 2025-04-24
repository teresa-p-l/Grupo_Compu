import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import os

def leer_matrices(filename):
    """
    Lee el archivo de matrices para una sola temperatura.
    Devuelve una lista de matrices.
    """
    matrices = []
    with open(filename, 'r') as file:
        lines = file.readlines()
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

def crear_video_matrices(matrices, output_path, temperatura):
    """
    Crea un video a partir de las matrices de spins para una sola temperatura.
    Cada frame corresponde a un paso temporal.
    """
    # Crear el directorio de salida si no existe
    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    # Configurar la figura
    fig, ax = plt.subplots()
    ax.axis('off')  # Ocultar los ejes

    def actualizar(frame):
        matriz = frame
        ax.clear()
        ax.axis('off')
        ax.set_title(f"Temperatura: {temperatura:.2f}", fontsize=16)
        ax.imshow(matriz, cmap='gray', vmin=-1, vmax=1)

    # Crear los frames
    frames = [np.array(matriz) for matriz in matrices]

    # Crear la animación
    anim = animation.FuncAnimation(fig, actualizar, frames=frames, repeat=False)

    # Guardar el video
    anim.save(output_path, writer='ffmpeg', fps=2)
    print(f"Video guardado en: {output_path}")

# Ejemplo de uso
if __name__ == "__main__":
    archivo = "c:/Users/diego/Desktop/Fisica_Computacional/GrupoCompu/Grupo_Compu/programa_Ising/programaDiegod/spinsSOLOUNA.txt"
    output_path = salida_video = "c:/Users/diego/Desktop/Fisica_Computacional/GrupoCompu/Grupo_Compu/programa_Ising/programaDiegod/spinsSOLOUNA_video.mp4"
    temperatura = 1.0  # Cambia esto según la temperatura correspondiente

    matrices = leer_matrices(archivo)
    crear_video_matrices(matrices, salida_video, temperatura)