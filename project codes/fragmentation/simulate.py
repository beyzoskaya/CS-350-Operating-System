# import matplotlib.pyplot as plt
# import random

# def simulate_fragmentation(num_files):
#     file_names = [f'file_{i}' for i in range(num_files)]
#     file_sizes = [random.randint(1, 1000) for _ in range(num_files)]
#     fragmentation_levels = [random.random() for _ in range(num_files)] 
#     return file_names, file_sizes, fragmentation_levels

# def plot_fragmentation_map(file_names, file_sizes, fragmentation_levels, save_path=None):
#     plt.figure(figsize=(10, 6))
#     plt.scatter(file_sizes, fragmentation_levels, alpha=0.5)
#     plt.xlabel('File Size (bytes)')
#     plt.ylabel('Fragmentation Level')
#     plt.title('Fragmentation Map')
#     plt.grid(True)
    
#     if save_path:
#         plt.savefig(save_path)
#         print(f"Fragmentation map saved as {save_path}")
#     else:
#         plt.show()

# num_files = 10000 
# file_names, file_sizes, fragmentation_levels = simulate_fragmentation(num_files)
# plot_fragmentation_map(file_names, file_sizes, fragmentation_levels, save_path="fragmentation_map_10000.png")


import matplotlib.pyplot as plt
import random

def simulate_fragmentation_and_locality(num_files):
    file_names = [f'file_{i}' for i in range(num_files)]
    file_sizes = [random.randint(1, 1000) for _ in range(num_files)]
    fragmentation_levels = [random.random() for _ in range(num_files)]
  
    locality = [random.choice([0, 1]) for _ in range(num_files)] 
    return file_names, file_sizes, fragmentation_levels, locality

def plot_fragmentation_map(file_names, file_sizes, fragmentation_levels, locality, save_path=None):
    plt.figure(figsize=(10, 6))
    plt.scatter(file_sizes, fragmentation_levels, c=locality, cmap='coolwarm', alpha=0.5, s=50)
    plt.xlabel('File Size (bytes)')
    plt.ylabel('Fragmentation Level')
    plt.title('Fragmentation Map with Locality')
    plt.colorbar(label='Locality')
    plt.grid(True)
    
    if save_path:
        plt.savefig(save_path)
        print(f"Fragmentation map with locality saved as {save_path}")
    else:
        plt.show()

num_files = 1000 
file_names, file_sizes, fragmentation_levels, locality = simulate_fragmentation_and_locality(num_files)
plot_fragmentation_map(file_names, file_sizes, fragmentation_levels, locality, save_path="fragmentation_map_with_locality_1000.png")
