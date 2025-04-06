import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Cargar datos
data = np.loadtxt("c:/Users/User/Documents/Fisica_compu/Compu/Grupo_Compu/SistemaSolar/output.txt")
t = data[:, 0]  # Tiempo
x = data[:, 1]  # Posición en X
y = data[:, 2]  # Posición en Y



# Configurar figura y ejes
fig, ax = plt.subplots()
ax.set_xlim(-1.2, 1.2)
ax.set_ylim(-1.2, 1.2)
ax.set_xlabel("x (UA)")
ax.set_ylabel("y (UA)")
ax.set_title("Órbita del planeta")
ax.plot(0, 0, 'yo', markersize=10, label="Sol")  # Representar el Sol

# Inicializar gráfico del planeta
planet, = ax.plot([], [], 'bo', markersize=5, label="Planeta")

# Función de actualización para la animación
def update(frame):
    planet.set_data([x[frame]], [y[frame]])
    return planet,

# Crear animación
ani = animation.FuncAnimation(fig, update, frames=len(t), interval=20, blit=True)

plt.legend()
plt.show()
