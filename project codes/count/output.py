import os
import json
import collections

def analyze_directory(directory):
    """
    Analyze file count, size, and type in the given directory, including subdirectories.

    :param directory: The path to the directory to analyze
    :return: A dictionary with total size, total count, and counts by type
    """
    total_size = 0
    total_count = 0
    types_count = collections.defaultdict(int)
    types_size = collections.defaultdict(int)

    for root, dirs, files in os.walk(directory):
        for file in files:
            try:
                filepath = os.path.join(root, file)
                filesize = os.path.getsize(filepath)
                _, ext = os.path.splitext(file)

                total_size += filesize
                total_count += 1
                types_count[ext] += 1
                types_size[ext] += filesize
            except Exception as e:
                print(f"Error processing file {filepath}: {e}")

    return {
        "total_size": total_size,
        "total_count": total_count,
        "types_count": dict(types_count),
        "types_size": dict(types_size)
    }

def pretty_print_analysis(analysis):
    """
    Pretty print the analysis result, sorted by file count and file size.

    :param analysis: The analysis result from analyze_directory
    """
    print(f"Total file count: {analysis['total_count']}")
    print(f"Total size: {analysis['total_size']} bytes")

    # Sort by file count
    print("\nFiles count by type (sorted):")
    for ext, count in sorted(analysis['types_count'].items(), key=lambda item: item[1], reverse=True):
        print(f"{ext if ext else 'No Extension'}: {count} files")

    # Sort by file size
    print("\nFiles size by type (sorted):")
    for ext, size in sorted(analysis['types_size'].items(), key=lambda item: item[1], reverse=True):
        print(f"{ext if ext else 'No Extension'}: {size} bytes")

def output_analysis_as_json(analysis, output_file):
    """
    Output the analysis result as a JSON file.

    :param analysis: The analysis result from analyze_directory
    :param output_file: The path to the output JSON file
    """
    with open(output_file, 'w') as f:
        f.write(json.dumps(analysis, indent=4))
    


if __name__ == "__main__":
    directory = input("Enter the directory to analyze: ")
    analysis = analyze_directory(directory)
    pretty_print_analysis(analysis)
    output_analysis_as_json(analysis, "output.json")