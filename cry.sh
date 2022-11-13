#!/bin/sh

##
## cry.sh
## Shortcut script that executes commands to clean, build, and run the project (gmake2)
## Note: Should be run from the root of the repository
##

# Update project submodules and dependencies (this can be commented if it's slowing things down)
python3 ./Scripts/bet.py --update

# Clean previous premake artifacts
python3 ./Scripts/bet.py --clean

# Build the project with premake
python3 ./Scripts/bet.py --build gmake2

# Optional: Compile the project from the generated Makefiles
make

# Optional: Run the unit tests
# python3 ./Scripts/bet.py --tests

# Optional: Run the project
./Build/Bin/Game/Game.app
