import os

from utils import bet_log, rdf, rdfe
from constants import PROJECT_DIR, CODE_DIR, TEST_DIR, VENDOR_DIR


"""
Cleans all premake build files from the project directory
"""
def clean():
    bet_log("Cleaning premake build files and metadata.")

    # Intermediate and build directories that may build regardless of toolset 
    bet_log("Checking for intermediate directories...")
    intermediate_dir = os.path.join(PROJECT_DIR, "Build")
    rdf(intermediate_dir)

    # Remove toolset specific files and directories
    rm_vs_files()
    rm_makefiles()
    rm_xcode_files()

    bet_log("bet! Successfully cleaned premake build files and output.")


"""
Cleans all Visual Studio files generated by premake
"""
def rm_vs_files():
    bet_log("Cleaning auto-generated solution files...")
    sln_file = os.path.join(PROJECT_DIR, "sad.sln")
    rdf(sln_file)

    bet_log("Cleaning Visual Studio project files...")
    proj_ext = ".vcxproj"
    proj_filters_ext = ".vcxproj.filters"
    proj_user_ext = ".vcxproj.user"

    # sad/Code/*.vcxproj
    # sad/Code/*.vcxproj.filters 
    # sad/Code/*.vcxproj.user
    rdfe(CODE_DIR, proj_ext)
    rdfe(CODE_DIR, proj_filters_ext)
    rdfe(CODE_DIR, proj_user_ext)

    # sad/Tests/*.vcxproj
    # sad/Tests/*.vcxproj.filters 
    # sad/Tests/*.vcxproj.user
    rdfe(TEST_DIR, proj_ext)
    rdfe(TEST_DIR, proj_filters_ext)
    rdfe(TEST_DIR, proj_user_ext)

    # sad/Vendor/*.vcxproj
    # sad/Vendor/*.vcxproj.filters 
    # sad/Vendor/*.vcxproj.user
    rdfe(VENDOR_DIR, proj_ext)
    rdfe(VENDOR_DIR, proj_filters_ext)
    rdfe(VENDOR_DIR, proj_user_ext)


"""
Cleans all project Makefiles generated by premake
"""
def rm_makefiles():
    bet_log("Cleaning auto-generated Makefiles...")
    makefile = "Makefile"
    makefile_ext = ".make"

    # sad/Makefile
    root_makefile = os.path.join(PROJECT_DIR, makefile)
    rdf(root_makefile)

    # sad/Code/Makefile
    # sad/Code/*.make
    code_makefile = os.path.join(CODE_DIR, makefile)
    rdf(code_makefile)
    rdfe(CODE_DIR, makefile_ext)

    # sad/Tests/Makefile
    # sad/Tests/*make
    test_makefile = os.path.join(TEST_DIR, makefile)
    rdf(test_makefile)
    rdfe(TEST_DIR, makefile_ext)

    # sad/Vendor/Makefile
    # sad/Vendor/*.make
    vendor_makefile = os.path.join(VENDOR_DIR, makefile)
    rdf(vendor_makefile)
    rdfe(VENDOR_DIR, makefile_ext)


"""
Cleans all xcode project files generated by premake
"""
def rm_xcode_files():
    bet_log("Removing xcode workspace file...")
    sad_xcworkspace = os.path.join(PROJECT_DIR, "sad.xcworkspace")
    rdf(sad_xcworkspace)

    bet_log("Removing xcode project files...")
    xcodeproj_ext = "xcodeproj"

    # sad/Code/*.xcodeproj
    rdfe(CODE_DIR, xcodeproj_ext)

    # sad/Tests/*.xcodeproj
    rdfe(TEST_DIR, xcodeproj_ext)

    # sad/Vendor/*.xcodeproj
    rdfe(VENDOR_DIR, xcodeproj_ext)