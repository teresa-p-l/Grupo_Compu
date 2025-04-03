import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Ruta del archivo de datos
ruta_archivo = "C:/Users/Turkipollito/Escritorio/Universidad/3Fisica/Segundocuatri/Computacional/REPOSITORIO/Grupo_Compu/DiegodPruebas/orbitas.txt"

# Lectura del archivo
datos = {}
with open(ruta_archivo, 'r') as archivo:
    lineas = archivo.readlines()
    encabezados = lineas[0].strip().split('\t')
    for encabezado in encabezados:
        datos[encabezado] = []
    for linea in lineas[1:]:
        valores = linea.strip().split('\t')
        for i, valor in enumerate(valores):
            datos[encabezados[i]].append(float(valor))

# Crear gráfica
fig, ax = plt.subplots(figsize=(10, 10))
ax.set_xlim(-35, 35)
ax.set_ylim(-35, 35)
ax.set_xlabel('Posición X (UA)')
ax.set_ylabel('Posición Y (UA)')
ax.set_title('Órbitas del Sistema Solar Simulado')
ax.grid(True)
ax.scatter(0, 0, color='yellow', s=300, label='Sol', edgecolor='orange', zorder=5)

# Planetas y colores
planetas = ['Mercurio', 'Venus', 'Tierra', 'Marte', 'Jupiter', 'Saturno', 'Urano', 'Neptuno']
colores = ['gray', 'orange', 'blue', 'red', 'brown', 'goldenrod', 'lightblue', 'darkblue']

# Inicializar líneas para las órbitas
lineas = []
for i, planeta in enumerate(planetas):
    linea, = ax.plot([], [], '-', color=colores[i], lw=1, label=planeta)
    lineas.append(linea)

ax.legend()

# Función de inicialización
def init():
    for linea in lineas:
        linea.set_data([], [])
    return lineas

# Función de actualización para la animación
def actualizar(frame):
    for i, planeta in enumerate(planetas):
        lineas[i].set_data(
            datos[f'{planeta}_x'][:frame],
            datos[f'{planeta}_y'][:frame]
        )
    return lineas

# Crear la animación
frames_totales = len(datos['t'])
intervalo = 10000 / frames_totales  # 60 segundos en total
ani = animation.FuncAnimation(fig, actualizar, frames=frames_totales,
                              init_func=init, interval=intervalo, blit=True)

# Guardar la animación como un archivo de video opcional (descomentar si lo necesitas)
# ani.save("sistema_solar_orbitas.mp4", fps=30, extra_args=['-vcodec', 'libx264'])

plt.show()