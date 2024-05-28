import os
import pydot
from collections import defaultdict

def generate_file_tree(root_dir, parent_node, graph):
    """
    Recursively generate the file tree structure as a hierarchical graph.
    """
    try:
        items = os.listdir(root_dir)
    except PermissionError:
        return
    
    for item in sorted(items):
        item_path = os.path.join(root_dir, item)
        if os.path.isdir(item_path):
            dir_node = pydot.Node(item, shape="folder")
            graph.add_node(dir_node)
            if parent_node is not None:
                graph.add_edge(pydot.Edge(parent_node, dir_node))
            generate_file_tree(item_path, dir_node, graph)
        else:
            file_node = pydot.Node(item, shape="file")
            graph.add_node(file_node)
            if parent_node is not None:
                graph.add_edge(pydot.Edge(parent_node, file_node))

root_directory = '/users/beyzakaya' 
output_image = 'file_system_tree.png'  

file_tree_graph = pydot.Dot(graph_type='graph')
generate_file_tree(root_directory, None, file_tree_graph)

file_tree_graph.write_png(output_image)
print("File System Tree saved as", output_image)
