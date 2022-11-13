import os
import subprocess

from utils import bet_log, bet_err
from constants import SUBPROCESS_USE_SHELL

"""
Performs a recursive update on project and its submodules
Ideally this gets run on project setup

TODO: Research git hooks for cloning? Or anstract this into a setup.py script
"""
def update():
    bet_log(f"Recursively updating project and its submodules")
    update_cmd = subprocess.run(["git", "submodule", "update", "--init", "--recursive"], shell=SUBPROCESS_USE_SHELL)

    if update_cmd.returncode == 0:
        bet_log("bet! Successfully updated project project dependencies and submodules.")
    else:
        bet_err("Uh oh, awkward... something went wrong while updating the project submodules.")
