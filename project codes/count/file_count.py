import json
import matplotlib.pyplot as plt

def read_json(filepath):
    with open(filepath, 'r') as f:
        data =  json.loads(f.read())
        # print(data)
    return data
    

def visualize_by_file_count(json):
    """
    visualize top 10 filetype by count bar graph
    """
    types_size = json['types_size']
    # print(types_size)

    sorted_types_size = sorted(types_size.items(), key=lambda item: item[1], reverse=True)
    # print(sorted_types_size)
    top_10_types_size = sorted_types_size[:10]
    print(top_10_types_size)
    types = []
    sizes = []
    for type, size in top_10_types_size:
        print(type, size)
        types.append(type)
        sizes.append(size)

    plt.bar(types, sizes)
    plt.xlabel('File Type')
    plt.xticks(rotation=45)
    plt.ylabel('Count')
    #log scale
    plt.yscale('log')
    plt.title('Top 10 File Types by Count')
    plt.show()


def visualize_by_file_size(json):
    """
    visualize top 10 filetype by size bar graph
    """
    types_size = json['types_size']
    # print(types_size)

    sorted_types_size = sorted(types_size.items(), key=lambda item: item[1], reverse=True)
    # print(sorted_types_size)
    top_10_types_size = sorted_types_size[:10]
    print(top_10_types_size)
    types = []
    sizes = []
    for type, size in top_10_types_size:
        # print(type, size)
        if type == '':
            type = 'No Extension'
        types.append(type)
        sizes.append(size)

    plt.bar(types, sizes)
    plt.xlabel('File Type')
    plt.xticks(rotation=45)
    plt.ylabel('Size')
    #log scale
    plt.yscale('log')
    plt.title('Top 10 File Types by Size')
    plt.show()



def find_mean(json):
    """
    find mean of file size
    """
    total_file_size = json['total_size']
    total_file_count = json['total_count']

    mean = total_file_size / total_file_count
    return mean

def visualize_by_size_per_file(json):
    """
    visualize top 10 filetype by size per file bar graph
    """
    types_size = json['types_size']
    types_count = json['types_count']
    means = {}
    for type in types_size:
        means[type] = types_size[type] / types_count[type]
    
    sorted_types_size = sorted(means.items(), key=lambda item: item[1], reverse=True)
    # print(top_10_types_size)
    types = []
    sizes = []
    for type, size in sorted_types_size[:10]:
        sizes.append(size)
        if type == '':
            type = 'No Extension'
        types.append(type)
    plt.bar(types, sizes)
    plt.xlabel('File Type')
    plt.xticks(rotation=45)
    plt.ylabel('Size')
    #log scale
    plt.yscale('log')
    plt.title('Top 10 File Types by Size per File')
    plt.show()


def main():
    analysis = read_json("./output.json")
    # print(analysis)
    visualize_by_file_count(analysis)
    visualize_by_file_size(analysis)
    # histogram_by_file_size(analysis)
    print(find_mean(analysis))
    visualize_by_size_per_file(analysis)


if __name__ == "__main__":
    main()