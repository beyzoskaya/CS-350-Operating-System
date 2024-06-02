import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import gaussian_kde

def collect_file_sizes(directory):
    file_sizes = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            try:
                file_size = os.path.getsize(file_path)
                file_sizes.append(file_size)
            except FileNotFoundError:
                pass  
    return file_sizes

def plot_file_size_distribution(directory, save_path=None):
    file_sizes = collect_file_sizes(directory)

    kde = gaussian_kde(file_sizes)
    sizes = np.linspace(min(file_sizes), max(file_sizes), 1000)
    pdf = kde(sizes)

    plt.plot(sizes, pdf, color='skyblue')
    plt.xlabel('File Size (bytes)')
    plt.ylabel('Probability Density')
    plt.title('File Size Distribution (PDF)')
    plt.grid(True)

    if save_path:
        plt.savefig(save_path)
        print(f"PDF plot saved as {save_path}")
    else:
        plt.show()

root_directory = '/'
plot_file_size_distribution(root_directory, save_path='file_size_distribution_pdf.png')
