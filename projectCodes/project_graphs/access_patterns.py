import os
import time

def get_file_access_patterns(root_dir):
    
    access_times = {}  
    modification_times = {}  

    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            file_path = os.path.join(dirpath, filename)
            try:
                access_time = os.path.getatime(file_path)
                modification_time = os.path.getmtime(file_path)
                access_times[file_path] = access_time
                modification_times[file_path] = modification_time
            except (PermissionError, FileNotFoundError):
                pass

    return access_times, modification_times

root_directory = '/'  

access_times, modification_times = get_file_access_patterns(root_directory)

print("File Access Patterns:")
for file_path, access_time in access_times.items():
    print("File:", file_path)
    print("Access Time:", time.ctime(access_time))
    print("Modification Time:", time.ctime(modification_times[file_path]))
    print()
