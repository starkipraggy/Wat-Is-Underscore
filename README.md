# SPA-Program-Assesssment
A piece of software designed to evaluate a program written in a fictional language SIMPLE and return queries based on the structure of the program

# Setup

    # Clone Repository
    git clone https://github.com/starkipraggy/SPA-Program-Assesssment.git

    # Clone Dependencies Repository (for use later)
    git submodule update --init --recursive


# Compilation and running

The recommended environment for building and testing the solution is Microsoft Visual Studio 2015 (any version), in the Debug configuration. After building the solution in Visual Studio, navigate to the Debug/Release folder generated during the build using a Command Prompt. There you should find the autotester.exe file and its associated dependencies. Run the autotester.exe in the folder, providing three arguments:

    1. The path of the desired SIMPLE Program file
    2. The path to the desired PQL Query file
    3. The path of the desired output file.
    Example: autotester.exe Test2-SIMPLE.txt Test2-Query.txt output.xml

You can find the XSL file for viewing the output.xml file in the root directory of this repository as analysis.xsl.

# Branch Naming:

    # Specific Feature or Functionality
    feature/<feature-label>
    
    # Semantic, Confiuration, Formatting, Code Cleanup, etc...
    enhance/<enhancement>
    
    # Bug, Error Fix, Etc...
    fix/<fix-label>
    
# External libraries
    ## Doxygen (documentation, install to run only)
    http://www.stack.nl/~dimitri/doxygen/index.html
    
    ## How to use Doxygen
    Simply run doxygen from the command line in the root of the project. 
    The documentation will be found in the SPAdocs folder in same folder as the project folder.
