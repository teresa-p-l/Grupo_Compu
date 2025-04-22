import numpy as np
import matplotlib.pyplot as plt

# Cargar datos
data = np.loadtxt("c:/Users/User/Documents/Fisica_compu/Compu/Grupo_Compu/Grupo_Compu/programa_Ising/programaMiguel/ising_data.txt", comments="#")
T, E, M = data[:,0], data[:,1], data[:,2]

# Graficar
plt.figure(figsize=(10, 5))
plt.subplot(1, 2, 1)
plt.plot(T, E, 'o-', label="Energía")
plt.xlabel("Temperatura")
plt.ylabel("Energía promedio por espín")
plt.grid(True)

plt.subplot(1, 2, 2)
plt.plot(T, M, 'o-', color='red', label="Magnetización")
plt.xlabel("Temperatura")
plt.ylabel("Magnetización promedio")
plt.grid(True)

plt.tight_layout()
plt.suptitle("Modelo de Ising 2D", fontsize=14, y=1.05)
plt.show()
