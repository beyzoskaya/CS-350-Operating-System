import os
import matplotlib.pyplot as plt
import networkx as nx

def create_directory_tree(root_directory, depth, max_children=5):
    G = nx.DiGraph()
    G.add_node(root_directory, subset=0) 
    _add_children(G, root_directory, depth, subset=1, max_children=max_children)
    return G

def _add_children(G, parent_dir, depth, subset, max_children):
    if depth == 0:
        return
    try:
        children = [d for d in os.listdir(parent_dir) if os.path.isdir(os.path.join(parent_dir, d))]
        num_children = len(children)
        num_added_children = min(num_children, max_children)
        child_spacing = 500
        for i, child in enumerate(children[:num_added_children]):
            child_path = os.path.join(parent_dir, child)
            G.add_node(child_path, subset=subset)
            G.add_edge(parent_dir, child_path)
            pos_x = (i - (num_added_children - 1) / 2) * child_spacing  
            G.nodes[child_path]['pos'] = (pos_x, -subset)
            _add_children(G, child_path, depth-1, subset+1, max_children)
    except PermissionError:
        print(f"Permission denied for directory: {parent_dir}")

def plot_directory_tree(root_directory, depth, max_children=5):
    G = create_directory_tree(root_directory, depth, max_children)
    pos = {node: data.get('pos', (0, 0)) for node, data in G.nodes(data=True)}
    plt.figure(figsize=(12, 8))
    nx.draw(G, pos, with_labels=True, node_size=3000, node_color='lightblue', font_size=8)
    plt.title(f'Directory Tree (Depth={depth}, Max Children={max_children})')
    plt.savefig('directory_tree_max_children.png')
    plt.show()

# Change the number of children and depth for your purpose 
root_directory = '/'  
depth = 2 
max_children = 5 

plot_directory_tree(root_directory, depth, max_children)
