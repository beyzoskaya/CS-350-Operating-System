import os

def get_file_info(directory):
    file_info = {}
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            file_size = os.path.getsize(file_path)
            file_type = file.split('.')[-1] if '.' in file else 'Unknown'
            file_info[file_path] = {'size': file_size, 'type': file_type}
    return file_info

def display_file_info(file_info):
    total_files = len(file_info)
    total_size = sum(info['size'] for info in file_info.values())
    print(f"Total number of files: {total_files}")
    print(f"Total size of all files: {total_size} bytes")
    print("\nFile information:")
    for file_path, info in file_info.items():
        print(f"File: {file_path} | Size: {info['size']} bytes | Type: {info['type']}")

if __name__ == "__main__":
    directory = input("Enter the directory path: ")
    file_info = get_file_info(directory)
    display_file_info(file_info)