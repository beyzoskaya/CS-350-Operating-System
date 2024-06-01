import os
import matplotlib.pyplot as plt

def plot_directory_structure(root_dir, level=0):
    try:
        fig, ax = plt.subplots()

        ax.set_title("Directory Structure Tree")

        ax.text(level*0.5, 0, os.path.basename(root_dir), fontsize=12, ha='center')

        dirs = [d for d in os.listdir(root_dir) if os.path.isdir(os.path.join(root_dir, d))]

        for i, d in enumerate(dirs):
            try:
                ax.text(level*0.5, -(i+1), d, fontsize=10, ha='center')
                plot_directory_structure(os.path.join(root_dir, d), level+1)
            except (PermissionError, OSError) as e:
                print(f"Permission denied or result too large for directory: {os.path.join(root_dir, d)}")

        files = [f for f in os.listdir(root_dir) if os.path.isfile(os.path.join(root_dir, f))]
        for i, f in enumerate(files):
            ax.text(level*0.5 + 0.5, -(i+len(dirs)+1), f, fontsize=10, ha='center', color='blue')

        ax.set_xlim(-1, level*0.5 + 1)
        ax.set_ylim(-(len(dirs)+len(files)), 1)
        ax.axis('off')

        plt.savefig('directory_structure_tree.png')

    except (PermissionError, OSError) as e:
        print(f"Permission denied or result too large for directory: {root_dir}")

root_directory = '/'

plot_directory_structure(root_directory)
