#Esto es una prueba de ploteo para ver si funciona

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

def leer_datos(filename):
    """
    Lee el archivo output.txt y devuelve una lista de pasos.
    Cada paso es una lista de posiciones [(rx, ry), ...] para cada planeta.
    """
    pasos = []
    with open(filename, 'r') as file:
        paso_actual = []
        for line in file:
            line = line.strip()
            if line == "":  # Línea en blanco indica un nuevo paso
                if paso_actual:
                    pasos.append(paso_actual)
                    paso_actual = []
            else:
                datos = line.split(",")
                rx, ry = float(datos[0]), float(datos[1])
                paso_actual.append((rx, ry))
        if paso_actual:  # Agregar el último paso si no está vacío
            pasos.append(paso_actual)
    return pasos

def animar_orbitas(pasos):
    """
    Crea una animación de las órbitas de los planetas.
    """
    fig, ax = plt.subplots()
    ax.set_aspect('equal')
    ax.set_xlim(-5e12, 5e12)  # Ajusta los límites según tus datos
    ax.set_ylim(-5e12, 5e12)
    ax.set_xlabel("x (m)")
    ax.set_ylabel("y (m)")
    ax.set_title("Órbitas de los planetas")

    # Crear un scatter plot para los planetas
    scatter = ax.scatter([], [], s=50)

    # Crear líneas para las trayectorias
    colores = plt.cm.jet(np.linspace(0, 1, len(pasos[0])))  # Colores para cada planeta
    trayectorias = [ax.plot([], [], color=colores[i], alpha=0.5)[0] for i in range(len(pasos[0]))]

    # Datos acumulados para las trayectorias
    trayectoria_x = [[] for _ in range(len(pasos[0]))]
    trayectoria_y = [[] for _ in range(len(pasos[0]))]

    def actualizar(frame):
        """
        Actualiza las posiciones de los planetas para cada paso.
        """
        posiciones = pasos[frame]
        rx = [pos[0] for pos in posiciones]
        ry = [pos[1] for pos in posiciones]

        # Actualizar las posiciones de los planetas
        scatter.set_offsets(list(zip(rx, ry)))

        # Actualizar las trayectorias
        for i in range(len(posiciones)):
            trayectoria_x[i].append(rx[i])
            trayectoria_y[i].append(ry[i])
            trayectorias[i].set_data(trayectoria_x[i], trayectoria_y[i])

        return scatter, *trayectorias

    # Crear la animación
    ani = animation.FuncAnimation(fig, actualizar, frames=len(pasos), interval=20, blit=True)

    plt.show()

# Leer los datos del archivo output.txt
filename = "c:/Users/diego/Desktop/Fisica_Computacional/GrupoCompu/Grupo_Compu/SISTEMAFINAL/output.txt"
pasos = leer_datos(filename)

# Crear la animación
animar_orbitas(pasos)