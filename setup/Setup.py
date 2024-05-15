
import os
import sys
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements

# Make sure everything we need for the setup is installed
PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
# os.chdir(os.path.dikrname(__file__)+'\\..') # Change from devtools/scripts directory to root

premakeInstalled = PremakeRequirements.Validate()

# print("\nUpdating submodules...")
# subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning premake...")
        if (len(sys.argv) > 1): # First argument is 'Setup.py'
            commands = []
            commands.append(os.path.dirname(__file__) + "\\Win-GenProjects.bat")
            commands.append("nopause")
            for command in sys.argv:
                commands.append(command)
            subprocess.call(commands)

    print("\nSetup completed!")
else:
    print("Cober Engi(ne requires Premake to generate project files.")

