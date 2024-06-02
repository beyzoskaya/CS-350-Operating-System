import os
import numpy as np
import matplotlib.pyplot as plt

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

def plot_file_size_distribution(directory, plot_type='histogram', save_path=None):
    file_sizes = collect_file_sizes(directory)

    if plot_type == 'histogram':
        plt.hist(file_sizes, bins=30, color='skyblue', edgecolor='black')
        plt.xlabel('File Size (bytes)')
        plt.ylabel('Frequency')
        plt.title('File Size Distribution (Histogram)')
        plt.grid(True)
    elif plot_type == 'cdf':
        sorted_sizes = np.sort(file_sizes)
        y = np.arange(1, len(sorted_sizes) + 1) / len(sorted_sizes)
        plt.plot(sorted_sizes, y, marker='.', linestyle='none')
        plt.xscale('log')
        plt.xlabel('File Size (bytes)')
        plt.ylabel('Cumulative Probability')
        plt.title('File Size Distribution (CDF)')
        plt.grid(True)
    else:
        print("Invalid plot type. Please choose 'histogram' or 'cdf'.")

    if save_path:
        plt.savefig(save_path)
    else:
        plt.show()

root_directory = '/'
plot_file_size_distribution(root_directory, plot_type='cdf', save_path='file_size_distribution_cdf.png')  
