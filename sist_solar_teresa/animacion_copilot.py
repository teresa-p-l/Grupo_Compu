import numpy as np
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Circle
import matplotlib.cm as cm

# Parámetros
file_in = "C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/sist_solar_teresa/planetas"  # Ruta del fichero de datos
file_out = "planetas"  # Nombre del fichero de salida (sin extensión)
x_min, x_max = -50, 50  # Límites de los ejes X
y_min, y_max = -50, 50  # Límites de los ejes Y
interval = 100  # Tiempo entre fotogramas en milisegundos
show_trail = True  # Mostrar estelas
trail_width = 1  # Ancho de las estelas
save_to_file = True  # Guardar en fichero
dpi = 150  # Calidad del vídeo
planet_radius = 1  # Radio de los planetas

# Lectura del fichero de datos
with open(file_in, "r") as f:
    data_str = f.read()

# Procesar los datos
frames_data = []
for frame_data_str in data_str.strip().split("\n\n"):
    frame_data = []
    for planet_pos_str in frame_data_str.split("\n"):
        planet_pos = np.fromstring(planet_pos_str, sep=",")
        if planet_pos.size > 0:
            frame_data.append(planet_pos)
    frames_data.append(frame_data)

nplanets = len(frames_data[0])  # Número de planetas
nframes = len(frames_data)  # Número de fotogramas

# Crear colores únicos para cada planeta
colors = cm.get_cmap("hsv", nplanets)

# Crear la figura y los ejes
fig, ax = plt.subplots()
ax.axis("equal")
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)

# Convertir el radio a una lista si es un único valor
if not hasattr(planet_radius, "__iter__"):
    planet_radius = planet_radius * np.ones(nplanets)

# Inicializar los planetas y sus estelas
planet_points = []
planet_trails = []
for i, (planet_pos, radius) in enumerate(zip(frames_data[0], planet_radius)):
    x, y = planet_pos
    planet_point = Circle((x, y), radius, color=colors(i))
    ax.add_artist(planet_point)
    planet_points.append(planet_point)

    if show_trail:
        planet_trail, = ax.plot(x, y, "-", linewidth=trail_width, color=colors(i))
        planet_trails.append(planet_trail)

# Función para actualizar la animación
def update(j_frame):
    for j_planet, planet_pos in enumerate(frames_data[j_frame]):
        x, y = planet_pos
        planet_points[j_planet].center = (x, y)

        if show_trail:
            xs_old, ys_old = planet_trails[j_planet].get_data()
            xs_new = np.append(xs_old, x)
            ys_new = np.append(ys_old, y)
            planet_trails[j_planet].set_data(xs_new, ys_new)

    return planet_points + planet_trails

# Función para inicializar la animación
def init_anim():
    if show_trail:
        for trail in planet_trails:
            trail.set_data([], [])
    return planet_points + planet_trails

# Crear la animación
animation = FuncAnimation(
    fig, update, init_func=init_anim, frames=nframes, blit=True, interval=interval
)

# Guardar o mostrar la animación
if save_to_file:
    animation.save(f"{file_out}.mp4", dpi=dpi)
else:
    plt.show()