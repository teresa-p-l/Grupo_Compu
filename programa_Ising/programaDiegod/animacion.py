import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import os

def leer_spins(filename):
    """
    Lee el archivo de spins y organiza los datos en un diccionario.
    Cada clave es una temperatura y su valor asociado es la matriz NxN.
    """
    datos = {}
    with open(filename, 'r') as file:
        lines = file.readlines()
        temperatura = None
        matriz = []

        for line in lines:
            line = line.strip()
            if line.startswith("# T ="):
                # Si ya hay una matriz cargada, guárdala antes de procesar la nueva temperatura
                if temperatura is not None:
                    datos[temperatura] = matriz
                    matriz = []
                # Extraer la temperatura
                temperatura = float(line.split('=')[1].strip())
            elif line:  # Si la línea no está vacía, es parte de la matriz
                matriz.append([int(x) for x in line.split()])
        
        # Guardar la última matriz
        if temperatura is not None:
            datos[temperatura] = matriz

    return datos

def crear_video(datos, output_path):
    """
    Crea un video a partir de las matrices de spins.
    Cada frame corresponde a una temperatura distinta.
    """
    # Crear el directorio de salida si no existe
    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    # Configurar la figura
    fig, ax = plt.subplots()
    ax.axis('off')  # Ocultar los ejes

    def actualizar(frame):
        temp, matriz = frame
        ax.clear()
        ax.axis('off')
        ax.set_title(f"Temperatura: {temp:.2f}", fontsize=16)
        ax.imshow(matriz, cmap='gray', vmin=-1, vmax=1)

    # Crear los frames
    frames = [(temp, np.array(matriz)) for temp, matriz in sorted(datos.items())]

    # Crear la animación
    anim = animation.FuncAnimation(fig, actualizar, frames=frames, repeat=False)

    # Guardar el video
    anim.save(output_path, writer='ffmpeg', fps=2)
    print(f"Video guardado en: {output_path}")

# Ejemplo de uso
if __name__ == "__main__":
    archivo = "c:/Users/diego/Desktop/Fisica_Computacional/Grupo_Compu/programa_Ising/programaDiegod/spins_all_temps.txt"
    salida_video = "c:/Users/diego/Desktop/Fisica_Computacional/Grupo_Compu/programa_Ising/programaDiegod/spins_video.mp4"

    datos = leer_spins(archivo)
    crear_video(datos, salida_video)