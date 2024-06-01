import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def visualize_file_system(root_dir, output_image):
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 10)
    ax.axis('off')

    # Function to recursively draw directory structure
    def draw_directory(dir_path, x, y, width, height):
        nonlocal ax
        try:
            ax.add_patch(patches.Rectangle((x, y), width, height, edgecolor='black', facecolor='none'))
            ax.text(x + 0.5 * width, y + 0.5 * height, os.path.basename(dir_path), ha='center', va='center', fontsize=8)
            subdirs = [d for d in os.listdir(dir_path) if os.path.isdir(os.path.join(dir_path, d))]
            num_subdirs = len(subdirs)
            if num_subdirs > 0:
                sub_width = width / num_subdirs
                for i, subdir in enumerate(subdirs):
                    draw_directory(os.path.join(dir_path, subdir), x + i * sub_width, y - 1.5, sub_width, height - 1)
        except (PermissionError, FileNotFoundError, OSError):
            pass

    draw_directory(root_dir, 1, 9, 8, 1)
    plt.savefig(output_image)  # Save the image with a specific filename
    plt.show()

# Specify the root directory to visualize
root_directory = '/'  # Update this with the desired directory
output_image = 'file_system_layout.png'  # Output image filename
visualize_file_system(root_directory, output_image)
