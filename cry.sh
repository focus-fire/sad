#!/bin/sh

##
## cry.sh
## Shortcut script that executes commands to clean, build, and run the project
## Using the 'gmake2' toolchain
## 
## Note: Should be run from the root of the repository
##

# Clean and build the project using the 'bet' pipeline
python3 ./Scripts/bet.py --clean
python3 ./Scripts/bet.py --build gmake2

# Make the project from the generated Makefiles
make

# Run the project 
./Build/Bin/Game/Game

# Optional: Run the test suites
# python3 ./Scripts/bet.py --tests
