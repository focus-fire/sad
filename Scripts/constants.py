import os


"""
Project Directories

SCRIPT_DIR - Path to the 'Scripts' directory
PROJECT_DIR - Path to the root directory of the project

CODE_DIR - Path to the 'Code' directory of the project
TEST_DIR - Path to the 'Tests' directory of the proejct
VENDOR_DIR - Path to the 'Vendor' directory of the project
"""
SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
PROJECT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, os.pardir))

CODE_DIR = os.path.join(PROJECT_DIR, "Code")
TEST_DIR = os.path.join(PROJECT_DIR, "Tests")
VENDOR_DIR = os.path.join(PROJECT_DIR, "Vendor")
