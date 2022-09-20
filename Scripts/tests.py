import os
import subprocess

from utils import bet_log, bet_err
from constants import PROJECT_DIR, SUBPROCESS_USE_SHELL


"""
Runs the compiled testbed on the given platform (exe/bin)
"""
def tests():
    bet_log('Running Catch2 tests...\n')

    test_bin = os.path.join(PROJECT_DIR, 'Build/Bin/Tests/Tests')
    test_cmd = subprocess.run([test_bin], shell=True)

    if test_cmd.returncode == 0:
        bet_log("bet! Successfully ran tests.")
    else:
        bet_err("Uh oh, awkward... something went wrong while executing the tests. Did you build the platform executable for the 'Tests' project?")


"""
Attempts to compile the project using a specified build tool before running tests
"""
def compile_tests(build_tool):
    supported_build_tools = ['msbuild', 'make']
    lowercase_build_tool = str(build_tool).lower()

    if lowercase_build_tool not in supported_build_tools:
        bet_err(f"Uh oh, awkward... {build_tool} was not found in the supported list!")
        return

    compile_cmd = int()

    if lowercase_build_tool == 'msbuild': 
        sln_file = os.path.join(PROJECT_DIR, 'sad.sln')
        compile_cmd = subprocess.run([lowercase_build_tool, sln_file], shell=SUBPROCESS_USE_SHELL)

    if lowercase_build_tool == 'make':
        compile_cmd = subprocess.run([lowercase_build_tool], shell=SUBPROCESS_USE_SHELL)

    if compile_cmd.returncode != 0:
        bet_err('Uh oh, awkward... something went wrong while executing the build command. Is there a solution or Makefile in the current directory?')
        return

    tests()