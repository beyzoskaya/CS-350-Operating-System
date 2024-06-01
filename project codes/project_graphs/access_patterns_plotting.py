import os
import time
import matplotlib.pyplot as plt
import numpy as np

def get_file_access_patterns(directory):
    access_times = {}  # Dictionary to store file access times
    modification_times = {}  # Dictionary to store file modification times

    for dirpath, dirnames, filenames in os.walk(directory):
        for filename in filenames:
            file_path = os.path.join(dirpath, filename)
            try:
                access_time = os.path.getatime(file_path)
                modification_time = os.path.getmtime(file_path)
                access_times[file_path] = access_time
                modification_times[file_path] = modification_time
            except (PermissionError, FileNotFoundError):
                pass

    return access_times, modification_times

# Specify the directory to analyze
directory_to_analyze = '/users/beyzakaya'

# Get access patterns for the specified directory
access_times, modification_times = get_file_access_patterns(directory_to_analyze)

# Line graph of access times over modification times
plt.figure(figsize=(10, 6))
plt.plot(modification_times.values(), access_times.values(), 'o', markersize=3)
plt.xlabel('Modification Time')
plt.ylabel('Access Time')
plt.title('Access Patterns: Access Time vs Modification Time')
plt.grid(True)
plt.savefig('access_time_vs_modification_time_line.png')  # Save the image with a descriptive filename
plt.show()

# Scatter plot of access times versus modification times
plt.figure(figsize=(10, 6))
plt.scatter(modification_times.values(), access_times.values(), s=5)
plt.xlabel('Modification Time')
plt.ylabel('Access Time')
plt.title('Access Patterns: Access Time vs Modification Time')
plt.grid(True)
plt.savefig('access_time_vs_modification_time_scatter.png')  # Save the image with a descriptive filename
plt.show()

# Heatmap of access patterns (frequency of file accesses over time)
access_array = np.array(list(access_times.values()))
modification_array = np.array(list(modification_times.values()))

plt.figure(figsize=(10, 6))
plt.hist2d(modification_array, access_array, bins=50, cmap='viridis')
plt.colorbar(label='Frequency')
plt.xlabel('Modification Time')
plt.ylabel('Access Time')
plt.title('Access Patterns Heatmap')
plt.savefig('access_patterns_heatmap.png')  # Save the image with a descriptive filename
plt.show()
