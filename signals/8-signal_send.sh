#!/usr/bin/bash

# Check if the number of arguments is correct
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <pid>"
    exit 1
fi

# Send the SIGQUIT signal to the process with the provided PID
kill -SIGQUIT "$1"
