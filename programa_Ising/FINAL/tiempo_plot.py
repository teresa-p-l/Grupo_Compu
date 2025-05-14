#
# ESTE PROGRAMA SIRVE PARA PLOTEAR EL TIEMPO DE TIME.TXT
# HAY QUE MODIFICAR AL FINAL LOS LÍMITES DEL EJE X Y DEL EJE Y
# 
# TE MUESTRA EL PLOT EN PLOTTIME.PNG Y LO GUARDA EN EL MISMO LUGAR
#



import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit




#Parámetro del eje X, inferior y superior
a=19
b=41

#Parámetro del eje Y, inferior y superior
c=0.0
d=4.5



#RUTAS:

#ruta de diego:
#c:/Users/diego/Desktop/Fisica_Computacional/Grupo_Compu/programa_Ising/FINAL/time.txt
#c:/Users/diego/Desktop/Fisica_Computacional/Grupo_Compu/programa_Ising/FINAL/plottime.png







def read_time_file(filename):
    """
    Reads the file 'time.txt' and returns two lists: x (integers) and y (floats).
    """
    x = []
    y = []
    with open(filename, 'r') as file:
        for line in file:
            data = line.strip().split()
            if len(data) == 2:
                x.append(int(data[0]))  # First value is x (integer)
                y.append(float(data[1]))  # Second value is y (float)
    return x, y

def fit_function(x, a,b,c):
    """
    Example function to fit the data: a quadratic function.
    Modify this function as needed.
    """
    return a * x**2 + b * x + c

def plot_time_data_with_fit(x, y, xlim=None, ylim=None, output_file="c:/Users/diego/Desktop/Fisica_Computacional/Grupo_Compu/programa_Ising/FINAL/plottime.png"):
    """
    Plots the data from the file, fits it to a function, and saves the plot as a PNG.
    """
    # Fit the data
    popt, _ = curve_fit(fit_function, x, y)
    a, b, c = popt

    # Generate fitted y values
    x_fit = np.linspace(min(x), max(x), 500)  # Smooth x values for the fit curve
    y_fit = fit_function(x_fit, a, b, c)

    # Plot the data
    plt.figure(figsize=(8, 6))
    plt.plot(x, y, marker='o', linestyle='', color='b', label='Data')  # Original data
    plt.plot(x_fit, y_fit, linestyle='-', color='r', label=f'Fit: {a:.5f}x² + {b:.5f}x + {c:.5f}')  # Fit curve
    plt.xlabel('N', fontsize=12)
    plt.ylabel('Time it took (s)', fontsize=12)
    plt.title('Time Data Plot with Fit', fontsize=14)
    plt.legend()
    plt.grid(True)

    # Set axis limits if provided
    if xlim:
        plt.xlim(xlim)
    if ylim:
        plt.ylim(ylim)

    # Save the plot as a PNG file
    plt.savefig(output_file)
    print(f"Plot saved as {output_file}")

    # Show the plot
    plt.show()

# Example usage
if __name__ == "__main__":
    filename = "c:/Users/diego/Desktop/Fisica_Computacional/Grupo_Compu/programa_Ising/FINAL/time.txt"
    output_file = "c:/Users/diego/Desktop/Fisica_Computacional/Grupo_Compu/programa_Ising/FINAL/plottime.png"
    x, y = read_time_file(filename)

    # Set axis limits (change these values as needed)
    x_limits = (a, b)  # Example: x-axis limits
    y_limits = (c, d)  # Example: y-axis limits

    # Plot the data with fit and save it
    plot_time_data_with_fit(x, y, xlim=x_limits, ylim=y_limits, output_file=output_file)