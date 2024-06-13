#!/usr/bin/python3
"""
This script searches the heap of a process for a given string and replaces it
with another string.

Usage: read_write_heap.py pid search_string replace_string

where:
    pid - is the process ID of the running process
    search_string - is the ASCII string to search for in the heap
    replace_string - is the ASCII string to replace the search_string with
"""

import os
from sys import argv, exit

def read_write_heap(pid, read_str, write_str):
    """
    Search the heap of process corresponding to `pid` for `read_str` and replace
    it with `write_str`.

    Args:
        pid (int): Process ID of the running process whose heap is to be searched
        read_str (str): The string to search for in the heap
        write_str (str): The string to replace `read_str` with
    """
    
    # Validate PID
    try:
        pid_n = int(pid)
        if pid_n <= 0:
            raise ValueError
    except (TypeError, ValueError):
        print("Missing or invalid pid!")
        print("Usage: read_write_heap(pid, read_str, write_str)")
        exit(1)

    # Validate read_str
    if not isinstance(read_str, str) or read_str == "":
        print("Missing or invalid read string!")
        print("Usage: read_write_heap(pid, read_str, write_str)")
        exit(1)

    # Validate write_str
    if not isinstance(write_str, str):
        print("Invalid write string!")
        print("Usage: read_write_heap(pid, read_str, write_str)")
        exit(1)

    # Ensure read_str and write_str have the same length
    if len(read_str) != len(write_str):
        print("read_str and write_str must be of the same length!")
        exit(1)

    try:
        maps_path = f"/proc/{pid}/maps"
        with open(maps_path, 'r') as maps:
            print(f"[*] maps: {maps_path}")
            print(f"[*] mem: /proc/{pid}/mem")

            heap_data = None
            for line in maps:
                if "heap" in line:
                    heap_data = line.split()
                    break

    except OSError as error:
        print(f"Can't open file /proc/{pid}/maps: OSError: {error}")
        exit(1)

    if heap_data is None:
        print("No heap found!")
        exit(1)
    else:
        print('\n'.join(("[*] Found: {}:".format(heap_data[-1]),
                         "\tpathname = {}".format(heap_data[-1]),
                         "\taddress range = {}".format(heap_data[0]),
                         "\tpermissions = {}".format(heap_data[1]),
                         "\toffset (in bytes) = {}".format(heap_data[2]),
                         "\tinode = {}".format(heap_data[4]))))

    addr = heap_data[0].split('-')
    start_address = int(addr[0], 16)
    end_address = int(addr[1], 16)
    print(f"[*] Addresses start [{addr[0]}] | end [{addr[1]}]")

    perms = heap_data[1]
    if 'r' not in perms or 'w' not in perms:
        print("Heap does not have read and write permissions")
        exit(1)

    try:
        mem_path = f"/proc/{pid}/mem"
        with open(mem_path, 'rb+') as mem:
            mem.seek(start_address)
            heap = mem.read(end_address - start_address)
            str_offset = heap.find(bytes(read_str, "ASCII"))
            if str_offset == -1:
                print(f"Can't find '{read_str}' in /proc/{pid}/mem")
                exit(1)
            else:
                print(f"[*] Found '{read_str}' at {hex(str_offset)}")

            mem.seek(start_address + str_offset)
            mem.write(bytes(write_str, "ASCII"))
            print(f"[*] Writing '{write_str}' at {hex(start_address + str_offset)}")

    except OSError as error:
        print(f"Can't open file /proc/{pid}/mem: OSError: {error}")
        exit(1)

if __name__ == "__main__":
    if len(argv) != 4:
        print("Usage: {} <pid> <string to read> <string to write>".format(argv[0]))
        exit(1)
    else:
        pid = argv[1]
        target_str = argv[2]
        new_str = argv[3]
        read_write_heap(pid, target_str, new_str)
