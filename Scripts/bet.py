import sys
import argparse

from build import build
from clean import clean
from tests import tests, compile_tests
from cc import gen_cc
from utils import bet_log
from constants import PEEPO_BET


"""
bet (before even trying) - the sad engine scripting pipeline
small cli tool that automates cleaning, building, and testing of the sad engine
"""
def main():
    bet_log(PEEPO_BET)

    parser = argparse.ArgumentParser(description='bet (before even trying) - the sad engine scripting pipeline')

    # Initialize command line arguments
    parser.add_argument('--clean', action=argparse.BooleanOptionalAction, help='cleans the project of auto-generated premake files')
    parser.add_argument('--build', nargs="?", const="vs2022", metavar='toolset', type=str, help='build the premake project (toolsets: vs2022 [default], vs2019, or gmake2)')
    parser.add_argument('--tests', action=argparse.BooleanOptionalAction, help='tries to run the sad testbed')
    parser.add_argument('--compile-tests', metavar='build tool', type=str, help='tries to compile and run the sad testbed with the build tool specified')
    parser.add_argument('--gen-cc', action=argparse.BooleanOptionalAction, help='generates compile commands for intellisense using premake-export-compile-commands (ex: for vscode)')
    args = parser.parse_args()

    # Print help menu if script is executed without arguments
    if len(sys.argv) == 1:
        parser.print_help(sys.stderr)
        return

    # Execute operations for passed arguments
    if args.clean is not None:
        clean()
    if args.build is not None:
        build(args.build)
    if args.tests is not None:
        tests()
    if args.compile_tests is not None:
        compile_tests(args.compile_tests)
    if args.gen_cc is not None:
        gen_cc()


if __name__ == '__main__':
    main()