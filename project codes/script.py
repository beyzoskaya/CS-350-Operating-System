import os
import mimetypes
import platform

def analyze_directory(directory):
    total_size = 0
    total_count = 0
    types_count = {}
    types_size = {}
    error_files = []

    for root, dirs, files in os.walk(directory):
        for file in files:
            try:
                filepath = os.path.join(root, file)
                filesize = os.path.getsize(filepath)
                
                # Handling different kind of file types
                file_type, _ = mimetypes.guess_type(file)
                if file_type is None:
                    file_type = "Unknown"
                
                total_size += filesize
                total_count += 1
                
                if file_type not in types_count:
                    types_count[file_type] = 1
                    types_size[file_type] = filesize
                else:
                    types_count[file_type] += 1
                    types_size[file_type] += filesize
            
            # Some files not allowed to access 
            except FileNotFoundError:
                error_files.append(filepath)
            # Some files not allowed to process depending on a extension of a file
            except Exception as e:
                error_files.append(filepath)
                print(f"Error processing file {filepath}: {e}")

    return {
        "total_size": total_size,
        "total_count": total_count,
        "types_count": types_count,
        "types_size": types_size,
        "error_files": error_files
    }

def print_analysis(analysis):
    print(f"Total file count: {analysis['total_count']}")
    print(f"Total size: {analysis['total_size']} bytes")

    if analysis['error_files']:
        print("\nError processing the following files:")
        for file_path in analysis['error_files']:
            print(file_path)

    # Prints the file count based on number of files
    #print("\nFiles count by type (sorted):")
    #for file_type, count in sorted(analysis['types_count'].items(), key=lambda item: item[1], reverse=True):
    #    print(f"{file_type}: {count} files")

    # Prints the file count based on file size
    print("\nFiles size by type (sorted):")
    for file_type, size in sorted(analysis['types_size'].items(), key=lambda item: item[1], reverse=True):
        print(f"{file_type}: {size} bytes")

if __name__ == "__main__":
    os_type = platform.system()
    
    # For Linux and Mac operating system understanding
    if os_type == "Linux":
        directory = input("Enter the Linux directory to analyze: ")
    elif os_type == "Darwin": 
        directory = input("Enter the macOS directory to analyze: ")
    else:
        print("Unsupported operating system")
        exit()

    analysis = analyze_directory(directory)
    print_analysis(analysis)
