#!/bin/bash
#
# Script to automatically set up symlinks for existing hooks.
#
# This code has been taken from BaHaMAS and adapted by
# Alessandro Sciarra <sciarra@th.physik.uni-frankfurt.de>
#
# Copyright (c) 2017,2018 Alessandro Sciarra
#
# This file is part of CL2QCD.
#
# CL2QCD is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CL2QCD is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CL2QCD. If not, see <http://www.gnu.org/licenses/>.
#-----------------------------------------------------------------------#

readonly CL2QCD_repositoryTopLevelPath="$(git rev-parse --show-toplevel)"
readonly hookGitFolder="${CL2QCD_repositoryTopLevelPath}/.git/hooks"

# Get full path to folder with hooks and then cut off the path to git
# repository. This makes the command to link the hooks stable in future,
# even if folders in the repository are renamed (assuming this script
# remains in the hook folder).
readonly CL2QCD_hooksFolderFromTopLevel=$(sed "s@${CL2QCD_repositoryTopLevelPath}/@@" <<< "$(dirname "$(readlink -f "$BASH_SOURCE")")")
readonly CL2QCD_hooksFolder="${CL2QCD_repositoryTopLevelPath}/${CL2QCD_hooksFolderFromTopLevel}"
readonly CL2QCD_failureExitCode=1
source ${CL2QCD_hooksFolder}/auxiliaryFunctions.bash || exit $CL2QCD_failureExitCode

cd ${hookGitFolder}
# Here we rely on the fact that in the "hooks" folder the executable files are only this
# script together with all the hooks that will then be used. It sounds reasonable.
errecho '\n'
for hookBash in $(find ${CL2QCD_hooksFolder} -maxdepth 1 -perm -111 -type f -printf "%f\n"); do
    #We have to skip this executable file
    if [ ${hookBash} != $(basename ${BASH_SOURCE}) ]; then
        hook="${hookBash%.bash}"
        if [ -e ${hook} ]; then #Here we are in the git folder (see cd above)
            if [ -L ${hook} ] && [ $(realpath ${hook}) = ${CL2QCD_hooksFolder}/${hookBash} ]; then
                errecho "Hook \"${hook}\" already correctly symlinked!\n" 10
                continue
            else
                errecho "Hook \"${hook}\" already existing, symlink not created!\n" 9
                continue
            fi
        else
            commandToBeRun="ln -s -f ../../${CL2QCD_hooksFolderFromTopLevel}/${hookBash} ${hook}"
            errecho "Symlinking hook \"${hook}\"" 13
            errecho "${commandToBeRun}"
            ${commandToBeRun}
            if [ ! -e ${hook} ]; then
                errecho "...failed!\n" 9
            else
                errecho "...done!\n" 10
            fi
        fi
    fi
done
errecho '\n'

#Check and in case warn the user about the absence of "clang-format"
if ! builtin type -P clang-format >/dev/null; then
    errecho '\e[1mWARNING:\e[21m The program "clang-format" was not found but it is needed by the pre-commit hook.\n\n' 11
fi

#Symlink clang-format options file to top directory so that it is found by clang-format
cd ${CL2QCD_repositoryTopLevelPath}
readonly clangFormatOptionFilesDestination="${CL2QCD_repositoryTopLevelPath}/_clang-format"
readonly clangFormatOptionFilesSource="${CL2QCD_hooksFolder}/_clang-format"
if [ -e ${clangFormatOptionFilesDestination} ]; then
    if [ -L ${clangFormatOptionFilesDestination} ] && [ $(realpath ${clangFormatOptionFilesDestination}) = ${clangFormatOptionFilesSource} ]; then
        errecho "File with clang-format options already correctly symlinked!\n" 10
    else
        errecho "File with clang-format options already existing, symlink not created!\n" 9
    fi
else
    commandToBeRun="ln -s -f ${CL2QCD_hooksFolderFromTopLevel}/$(basename ${clangFormatOptionFilesSource}) $(basename ${clangFormatOptionFilesSource})"
    errecho "Symlinking file with clang-format options" 13
    errecho "${commandToBeRun}"
    ${commandToBeRun}
    if [ ! -e ${clangFormatOptionFilesDestination} ]; then
        errecho "...failed!\n" 9
    else
        errecho "...done!\n" 10
    fi
fi
errecho '\n'
