import os
import subprocess

from utils import bet_log, bet_err
from constants import PROJECT_DIR, SUBPROCESS_USE_SHELL


"""
Runs the compiled testbed on the given platform (exe/bin)
"""
def tests():
    bet_log('\nStarted running Catch2 tests!')

    bet_log('Running Core testbed...')
    core_test_bin = os.path.join(PROJECT_DIR, 'Build/Bin/Tests/CoreTests/CoreTests')
    core_test_cmd = subprocess.run([core_test_bin], shell=SUBPROCESS_USE_SHELL)

    bet_log('Running Game testbed...')
    game_test_bin = os.path.join(PROJECT_DIR, 'Build/Bin/Tests/GameTests/GameTests')
    game_test_cmd = subprocess.run([game_test_bin], shell=SUBPROCESS_USE_SHELL)

    bet_log('Running Engine testbed...')
    eng_test_bin = os.path.join(PROJECT_DIR, 'Build/Bin/Tests/EngineTests/EngineTests')
    eng_test_cmd = subprocess.run([eng_test_bin], shell=SUBPROCESS_USE_SHELL)

    tests_passed = core_test_cmd.returncode == 0 and game_test_cmd.returncode == 0 and eng_test_cmd.returncode == 0  

    if not tests_passed: 
        bet_err("Uh oh, awkward... either the tests failed or something went wrong while running the testbeds.")

    bet_log("bet! Successfully ran tests.")


"""
Attempts to compile the project using a specified build tool before running tests
"""
def compile_tests(build_tool):
    supported_build_tools = ['msbuild', 'make']
    lowercase_build_tool = str(build_tool).lower()

    # Exit with error if user passes invalid build tool
    if lowercase_build_tool not in supported_build_tools:
        bet_err(f"Uh oh, awkward... {build_tool} was not found in the supported list!")

    compile_cmd = int()

    if lowercase_build_tool == 'msbuild': 
        sln_file = os.path.join(PROJECT_DIR, 'sad.sln')
        compile_cmd = subprocess.run([lowercase_build_tool, sln_file], shell=SUBPROCESS_USE_SHELL)

    if lowercase_build_tool == 'make':
        compile_cmd = subprocess.run([lowercase_build_tool], shell=SUBPROCESS_USE_SHELL)

    # Exit with error if command executes but no build artifacts exist
    if compile_cmd.returncode != 0:
        bet_err('Uh oh, awkward... something went wrong while executing the build command. Is there a solution or Makefile in the current directory?')

    tests()