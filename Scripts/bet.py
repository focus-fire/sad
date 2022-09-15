import sys
import argparse

from build import build
from clean import clean
from tests import tests


"""
bet - the sad engine scripting pipeline
small cli tool that automates cleaning, building, and testing of the sad engine
"""
def main():
    parser = argparse.ArgumentParser(description='bet - the sad engine scripting pipeline')

    # Initialize command line arguments
    parser.add_argument('--clean', action=argparse.BooleanOptionalAction, help='cleans the project of auto-generated premake files')
    parser.add_argument('--build', metavar='toolset', type=str, help='build the premake project (toolsets: vs2022, vs2019, gmake2, xcode, etc.)')
    parser.add_argument('--tests', action=argparse.BooleanOptionalAction, help='tries to build and run the sad testbed')
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


if __name__ == '__main__':
    main()