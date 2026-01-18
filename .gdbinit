# GDB initialization file for 10kingdoms debugging
# This file is automatically loaded by GDB when starting a debugging session

# Get the project root directory using Python
# This assumes .gdbinit is in the project root, or we detect it by looking for 'data' directory
python
import os

# Try to find the project root by looking for the 'data' directory
# Start from current working directory and walk up if needed
cwd = os.getcwd()
project_root = cwd

# If we're already in the data directory, go up one level
if os.path.basename(cwd) == 'data':
    project_root = os.path.dirname(cwd)
# Otherwise, check if 'data' directory exists in current directory
elif not os.path.isdir(os.path.join(cwd, 'data')):
    # Walk up the directory tree to find the project root
    current = cwd
    while current != os.path.dirname(current):  # Stop at filesystem root
        if os.path.isdir(os.path.join(current, 'data')):
            project_root = current
            break
        current = os.path.dirname(current)

data_dir = os.path.join(project_root, 'data')
data_dir = os.path.abspath(data_dir)

# Store in a GDB convenience variable
gdb.execute('set $tkdata_dir = "%s"' % data_dir)
end

# Set the working directory to the data folder
set cwd $tkdata_dir

# Set environment variable for data path
set environment TKDATA=$tkdata_dir

# Enable pretty printing for C++ STL containers
set print pretty on
set print elements 0

# Set history size
set history size 10000

# Enable source code display
set listsize 20

# Set breakpoint on common error functions
# Uncomment as needed:
# break abort
# break exit

# Useful aliases
define bt_full
    set backtrace limit 0
    bt
    set backtrace limit 200
end
document bt_full
Print full backtrace without limit
end

# Print current source location
define whereami
    frame
    list
end
document whereami
Show current frame and source code
end
