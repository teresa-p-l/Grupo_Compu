import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import matplotlib.animation as animation
import re

def parse_data_file(file_path):
    """Parse the data file into temperature values and corresponding matrices."""
    with open("C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/spins_all_temps.txt", 'r') as file:
        content = file.read()
    
    # Split the content by temperature headers - note the space between # and T
    pattern = r'#\s*T\s*=\s*([\d.]+)'  # Updated pattern to match "# T = X.XX"
    
    # Find all sections
    sections = re.split(pattern, content)
    
    # The first section is empty (before the first temperature header)
    sections = sections[1:]  # Skip the first (empty) section
    
    # Extract all temperature values
    temperatures = re.findall(pattern, content)
    
    temperature_data = []
    
    # Process each section with its corresponding temperature
    for i, (temp_str, section) in enumerate(zip(temperatures, sections)):
        # Convert temperature to float
        temperature = float(temp_str)
        
        # Parse the matrix data
        matrix_data = []
        for line in section.strip().split('\n'):
            line = line.strip()
            if not line or line.startswith('#'):
                continue  # Skip empty lines and comments
                
            try:
                row_values = [int(val) for val in line.split()]
                if row_values:  # Only append non-empty rows
                    matrix_data.append(row_values)
            except ValueError:
                continue  # Skip lines that can't be parsed as integers
        
        if matrix_data:  # Ensure we have data
            matrix = np.array(matrix_data)
            temperature_data.append((temperature, matrix))
    
    return temperature_data

def create_animation(data_file, output_file="temperature_animation.gif", interval=1000):
    """Create an animation from the temperature matrix data using only matplotlib."""
    # Parse the data
    temperature_data = parse_data_file(data_file)
    
    if not temperature_data:
        print("No valid data found in the file.")
        return
    
    print(f"Found {len(temperature_data)} temperature matrices to animate.")
    
    # Create figure and axes
    fig, ax = plt.subplots(figsize=(8, 8))
    
    # Fixed colormap setup - ensure number of colors matches number of bins
    # For values -1 and 1, we need only 2 distinct color regions
    cmap = mcolors.ListedColormap(['yellow', 'purple'])
    
    # Create a simpler norm that just checks if value is below or above 0
    norm = mcolors.BoundaryNorm([-2, 0, 2], cmap.N)
    
    # Initialize with first matrix
    temp, matrix = temperature_data[0]
    im = ax.imshow(matrix, cmap=cmap, norm=norm, interpolation='none')
    
    # Add grid lines
    ax.grid(which='both', color='white', linestyle='-', linewidth=0.5)
    ax.tick_params(axis='both', which='both', length=0)
    
    # Remove axis labels
    ax.set_xticks([])
    ax.set_yticks([])
    
    # Create title
    title = ax.set_title(f'Temperature = {temp}', fontsize=14)
    
    def update(frame):
        """Update function for animation."""
        temp, matrix = temperature_data[frame]
        im.set_array(matrix)
        title.set_text(f'Temperature = {temp}')
        return [im, title]
    
    # Create animation
    ani = animation.FuncAnimation(
        fig, update, frames=len(temperature_data), 
        interval=interval, blit=True
    )
    
    # Save animation
    ani.save(output_file, writer='pillow', fps=1000/interval)
    
    print(f"Animation saved as {output_file}")
    plt.close()

# Example usage
if __name__ == "__main__":
    # Replace with your actual file path
    data_file = "temperature_data.txt"
    create_animation(data_file, "C:/Users/Teresa/Desktop/COMPU/Grupo_Compu/programa_Ising/programaTeresa/temperature_animation.gif", interval=1000)  # interval in milliseconds