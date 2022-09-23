import os
import subprocess

from utils import bet_log, bet_err
from constants import SCRIPT_DIR, PROJECT_DIR, SUBPROCESS_USE_SHELL


"""
Uses premake to build the project for a particular toolset
Configured toolsets include vs2022, vs2019, gmake2, or xcode
"""
def build(toolset):
    # Premake Paths 
    premake = os.path.abspath(os.path.join(SCRIPT_DIR, 'premake5/premake5'))
    premake_config = os.path.abspath(os.path.join(PROJECT_DIR, 'premake5.lua'))

    # Add or include more premake toolsets here
    # Refer to premake5 documentation for available toolsets
    # https://premake.github.io/docs/Using-Premake/#using-premake-to-generate-project-files
    available_toolsets = ["vs2022", "vs2019", "gmake2", "xcode4"]

    if toolset not in available_toolsets:
        bet_err(f"Uh oh, awkward... {toolset} isn't a valid toolset, please try again.")
        return

    # Execute OS subprocess for premake with the desired toolset
    bet_log(f"Building for {toolset} in {PROJECT_DIR}")
    premake_cmd = subprocess.run([premake, toolset, f"--file={premake_config}"], shell=SUBPROCESS_USE_SHELL)

    if premake_cmd.returncode == 0:
        bet_log("bet! Successfully built project files.")
    else:
        bet_err("Uh oh, awkward... something went wrong while executing the build.")