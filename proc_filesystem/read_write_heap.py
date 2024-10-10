#!/usr/bin/python3
"""
Finds a string in the heap and replaces it
"""

import sys

def read_write_heap(pid, oldstr, newstr):
    """Finds and replaces a string in the heap of a process."""
    # Ensure the new string is not longer than the old string
    assert len(newstr) <= len(oldstr),\
        "Error: The new string ('{}') cannot be longer than the old string ('{}').".format(newstr, oldstr)

    # Pad the new string with spaces if it is shorter than the old string
    if len(newstr) < len(oldstr):
        newstr = newstr.ljust(len(oldstr))

    # Open and read the process' heap memory map
    with open("/proc/{}/maps".format(pid), 'r') as maps:
        # Find the line corresponding to the heap
        line = next((x for x in maps if '[heap]' in x), None)
        assert line, "Error: Could not find the heap in the process memory map."

        # Extract the start and end addresses of the heap
        start, end = line.split(maxsplit=1)[0].split("-")
        start, end = int(start, 16), int(end, 16)

    # Open and read the process' memory
    with open("/proc/{}/mem".format(pid), 'rb+') as mem:
        mem.seek(start)
        heap = mem.read(end - start)

        # Find the first occurrence of the old string in the heap
        offset = heap.find(bytes(oldstr, "ascii"))
        assert offset != -1, "Error: The old string ('{}') was not found in the heap.".format(oldstr)

        # Replace all occurrences of the old string in the heap
        while offset != -1:
            mem.seek(start + offset)
            mem.write(bytes(newstr, "ascii"))
            offset = heap.find(bytes(oldstr, "ascii"), offset + len(oldstr))

if __name__ == '__main__':
    try:
        # Ensure we have the correct number of command-line arguments
        if len(sys.argv) != 4:
            raise ValueError("Usage: python read_write_heap.py <pid> <old-string> <new-string>")

        _, pid, oldstr, newstr = sys.argv
        read_write_heap(pid, oldstr, newstr)

    except AssertionError as e:
        print(f"Error: {e}")
        exit(1)

    except ValueError as e:
        print(e)
        exit(1)

    except Exception as e:
        print("An unexpected error occurred.",
              "Please report this issue with the error message below:",
              f"Error type: {type(e).__name__}\nDetails: {e}",
              sep='\n', file=sys.stderr)
        exit(1)
