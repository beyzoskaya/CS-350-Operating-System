import os
import csv
from datetime import datetime

def collect_metadata(directory):
    metadata = []
    for root, dirs, files in os.walk(directory):
        for name in dirs + files:
            path = os.path.join(root, name)
            try:
                stat = os.stat(path)
                metadata.append({
                    'name': name,
                    'path': path,
                    'mode': stat.st_mode,
                    'uid': stat.st_uid,
                    'gid': stat.st_gid,
                    'size': stat.st_size,
                    'mtime': stat.st_mtime,
                })
            except FileNotFoundError:
                print(f"FileNotFoundError: Skipping {path}")
    return metadata

def save_metadata(metadata, output_file):
    with open(output_file, 'w', newline='') as csvfile:
        fieldnames = ['name', 'path', 'mode', 'uid', 'gid', 'size', 'mtime']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        
        writer.writeheader()
        for data in metadata:
            writer.writerow(data)

def main():
    root_directory = '/Users/beyzakaya'  
    output_file = 'metadata.csv'

    metadata = collect_metadata(root_directory)
    save_metadata(metadata, output_file)
    print(f"Metadata saved to {output_file}")

if __name__ == "__main__":
    main()
