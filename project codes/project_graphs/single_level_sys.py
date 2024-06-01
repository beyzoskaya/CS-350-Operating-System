import os
import matplotlib.pyplot as plt
import networkx as nx

def plot_single_level_tree(root_dir, output_image):
    G = nx.Graph()

    # Add root node
    G.add_node(root_dir)

    # Get first-level children of root directory
    first_level_children = [os.path.join(root_dir, d) for d in os.listdir(root_dir) if os.path.isdir(os.path.join(root_dir, d))]

    # Add first-level children as nodes and edges
    for child in first_level_children:
        G.add_node(child)
        G.add_edge(root_dir, child)

    # Plot the tree
    pos = nx.spring_layout(G)
    plt.figure(figsize=(10, 6))
    nx.draw(G, pos, with_labels=True, node_size=3000, node_color='skyblue', font_size=8, font_weight='bold')
    plt.title('Single-Level Directory Tree')
    plt.tight_layout()
    plt.savefig(output_image)  # Save the image with a specific filename
    plt.show()

# Specify the root directory to search for single-level directories
root_directory = '/'  # Update this with the desired root directory
home_directory = '/home'
output_image = 'single_level_directory_tree.png'  # Output image filename
output_image_2 = 'single_level_dir_home_tree.png' 
plot_single_level_tree(root_directory, output_image)
