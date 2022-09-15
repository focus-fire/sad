import os
import subprocess

from utils import bet_log
from constants import PROJECT_DIR


"""
Runs the compiled test package on the given platform (exe/bin)
"""
def tests():
    file_name = os.path.basename(__file__)
    bet_log(file_name, 'Running Catch2 tests...\n')

    # Optional: Run msbuild to compile project before running tests
    # sln_file = os.path.join(PROJECT_DIR, 'sad.sln')
    # msbuild_cmd = subprocess.run(['msbuild', sln_file, '-interactive:False', '-p:Configuration=Debug'], shell=True)

    test_bin = os.path.join(PROJECT_DIR, 'Build/Bin/Tests/Tests')
    test_cmd = subprocess.run([test_bin], shell=True)

    if test_cmd.returncode == 0:
        bet_log(file_name, "bet! Successfully ran tests.")
    else:
        bet_log(file_name, "Uh oh, awkward... something went wrong while executing the tests.")
        bet_log(file_name, "Did you build the platform executable for the 'Tests' project?")
