import os
import sys
import shutil

from constants import PENSIVE_COWBOY


"""
BETter print()
"""
def bet_log(msg):
    print(f"[bet] {msg}")


"""
BETter error()
"""
def bet_err(msg):
    print(PENSIVE_COWBOY, file=sys.stderr)
    print(f"[bet] {msg}", file=sys.stderr)


"""
rdf - remove directory or file

Note: Use with caution, a bit of a savage little function
ie: Will nuke directories/files that are passed without errors or warnings
"""
def rdf(path):
    if not os.path.exists(path):
        return

    if os.path.isdir(path):
        shutil.rmtree(path)
        return

    os.remove(path)


"""
rdfe - remove directory or file with extension

Searches passed directory for all file matching a particular extension and removes them
"""
def rdfe(dir, ext):
    for item in os.listdir(dir):
        if item.endswith(ext):
            rdf(os.path.join(dir, item))

