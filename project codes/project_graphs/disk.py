import os
import matplotlib.pyplot as plt

def get_disk_usage(directory):
    total_size = 0
    for root, dirs, files in os.walk(directory):
        for f in files:
            file_path = os.path.join(root, f)
            try:
                total_size += os.path.getsize(file_path)
            except FileNotFoundError:
                print(f"File {file_path} not found.")
    return total_size

def plot_disk_usage(directory):
    disk_usage = {}
    for root, dirs, files in os.walk(directory):
        for d in dirs:
            dir_path = os.path.join(root, d)
            try:
                disk_usage[dir_path] = get_disk_usage(dir_path)
            except FileNotFoundError:
                print(f"Directory {dir_path} not found.")
    
    sorted_disk_usage = sorted(disk_usage.items(), key=lambda x: x[1], reverse=True)
    top_directories = sorted_disk_usage[:10]
    
    directories, sizes = zip(*top_directories)
    
    plt.figure(figsize=(10, 6))
    plt.barh(directories, sizes, color='skyblue')
    plt.xlabel('Disk Space Usage (bytes)')
    plt.ylabel('Directory')
    plt.title('Top 10 Directories by Disk Space Usage')
    plt.gca().invert_yaxis() 
    plt.tight_layout()

    plt.savefig('disk_usage_chart.png')

    plt.show()

root_directory = '/'

plot_disk_usage(root_directory)
