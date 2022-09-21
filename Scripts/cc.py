import os
import subprocess

from utils import bet_log
from constants import SCRIPT_DIR, SUBPROCESS_USE_SHELL


"""
Generates compile commands for the project using `premake-export-compile-commands`
Will install and configure the repository if not found on the system

https://github.com/tarruda/premake-export-compile-commands
"""
def gen_cc():
    bet_log('Generating compile commands for project...')
    bet_log('Checking if tarruda/premake-export-compile-commands is installed...')

    # Ensure .premake config directory exists
    home_dir = os.path.expanduser('~')
    premake_home_dir = os.path.join(home_dir, '.premake')
    if not os.path.isdir(premake_home_dir):
        bet_log('Creating ~/.premake directory to add premake extensions')
        os.mkdir(premake_home_dir)

    # Check if 'export-compile-commands' repository is cloned and enabled in premake
    cc_tool_dir =  os.path.join(premake_home_dir, 'export-compile-commands')
    cc_setup_file = os.path.join(premake_home_dir, 'premake-system.lua')

    # Clone the compile commands extension repository if it isn't located
    if not os.path.isdir(cc_tool_dir):
        bet_log('Cloning `tarruda/premake-export-compile-commands`')
        clone_cmd = subprocess.run(['git', 'clone', 'https://github.com/tarruda/premake-export-compile-commands', cc_tool_dir], shell=SUBPROCESS_USE_SHELL)
        if clone_cmd.returncode != 0:
            bet_log('Uh oh, awkward... failed to clone the `export-compile-commands` repository')
            return

    # Create the premake extension setup file if it isn't found
    if not os.path.isfile(cc_setup_file):
        bet_log('Creating setup file for premake extensions')
        with open(cc_setup_file, 'w') as file:
            file.write('require \"export-compile-commands\"')
            file.close()
    
    # Use premake extension to export compile commands 
    premake = os.path.join(SCRIPT_DIR, 'premake5/premake5')
    cc_cmd = subprocess.run([premake, 'export-compile-commands'], shell=SUBPROCESS_USE_SHELL)
    if cc_cmd.returncode == 0:
        bet_log('bet! Successfully generated compile commands for the project.')
    else:
        bet_log('Uh oh, awkward... something went wrong while exporting the compile commands')

