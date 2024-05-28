import os
import matplotlib.pyplot as plt
import networkx as nx

def build_file_tree(root_dir):
    G = nx.DiGraph()

    # Add root node
    G.add_node(root_dir)

    # Get immediate subdirectories of root directory
    subdirectories = [os.path.join(root_dir, d) for d in os.listdir(root_dir) if os.path.isdir(os.path.join(root_dir, d))]

    # Add subdirectories as root nodes
    for subdir in subdirectories:
        G.add_node(subdir)
        G.add_edge(root_dir, subdir)

        # Recursively add directories and files as nodes and edges
        for dirpath, dirnames, filenames in os.walk(subdir):
            for dirname in dirnames:
                dir_full_path = os.path.join(dirpath, dirname)
                G.add_node(dir_full_path)
                G.add_edge(dirpath, dir_full_path)
            for filename in filenames:
                file_full_path = os.path.join(dirpath, filename)
                G.add_node(file_full_path)
                G.add_edge(dirpath, file_full_path)

    return G

def plot_shared_files(file_tree, output_image):
    plt.figure(figsize=(14, 8))
    pos = nx.spring_layout(file_tree, seed=42)
    nx.draw(file_tree, pos, with_labels=True, node_size=3000, node_color='skyblue', font_size=8, font_weight='bold')
    plt.title('Shared Files in File System')
    plt.tight_layout()
    plt.savefig(output_image)  # Save the image with a specific filename
    plt.show()

# Specify the root directory to visualize
root_directory = '/users/beyzakaya/Desktop/bk/Akademik'  # Update this with the path to the parent directory containing multiple subdirectories
output_image = 'shared_files_tree.png'  # Output image filename

# Build the file tree
file_tree = build_file_tree(root_directory)

# Plot the shared files tree and save the image
plot_shared_files(file_tree, output_image)
