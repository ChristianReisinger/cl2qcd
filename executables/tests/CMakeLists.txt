# Copyright (c) 2011-2013 Matthias Bach
# Copyright (c) 2012-2014 Christopher Pinke
# Copyright (c) 2013,2014,2018 Alessandro Sciarra
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

message(STATUS "Building executables tests")

find_package(PythonInterp REQUIRED)

# default random number seed
configure_file( ${CMAKE_SOURCE_DIR}/rand_seeds rand_seeds COPYONLY )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/conf.00200 conf.00200 COPYONLY )

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# ignore some tests
configure_file(CTestCustom.cmake ${CMAKE_BINARY_DIR}/CTestCustom.cmake @ONLY)

# Test the inverter for correct inversion
## even testnumbers correspond to tests using even-odd-precond.,
## odd numbers to those who don`t
## tests on a cold config
add_test(executables/Inverter1_CPU_TM ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1            --useGPU=0 --useEO=false  --measureCorrelator=1)
add_test(executables/Inverter2_CPU_TM_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1         --useGPU=0 --useEO=true   --measureCorrelator=1)
add_test(executables/Inverter1_CPU_TM_CG ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1         --useGPU=0 --useEO=false  --measureCorrelator=1 --solver=cg)
add_test(executables/Inverter2_CPU_TM_CG_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1      --useGPU=0 --useEO=true   --measureCorrelator=1 --solver=cg)
add_test(executables/Inverter1_CPU_TM_SAVE ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1       --useGPU=0 --useEO=false  --measureCorrelator=1 --solver=bicgstab_save)
add_test(executables/Inverter2_CPU_TM_SAVE_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1    --useGPU=0 --useEO=true   --measureCorrelator=1 --solver=bicgstab_save)
add_test(executables/Inverter5_GPU_TM ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1            --useGPU=1 --useEO=false  --measureCorrelator=1 --useCPU=false)
add_test(executables/Inverter6_GPU_TM_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1         --useGPU=1 --useEO=true   --measureCorrelator=1 --useCPU=false)
add_test(executables/Inverter5_GPU_TM_CG ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1         --useGPU=1 --useEO=false  --measureCorrelator=1 --useCPU=false --solver=cg)
add_test(executables/Inverter6_GPU_TM_CG_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1      --useGPU=1 --useEO=true   --measureCorrelator=1 --useCPU=false --solver=cg)
add_test(executables/Inverter5_GPU_TM_SAVE ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1       --useGPU=1 --useEO=false  --measureCorrelator=1 --useCPU=false --solver=bicgstab_save)
add_test(executables/Inverter6_GPU_TM_SAVE_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1    --useGPU=1 --useEO=true   --measureCorrelator=1 --useCPU=false --solver=bicgstab_save)
## Tests with rec12 active left out since options not available in executable, because broken at the moment
#add_test(executables/Inverter3_CPU_TM_REC12 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1      --useGPU=0 --useEO=false  --useReconstruct12=true --measureCorrelator=1)
#add_test(executables/Inverter4_CPU_TM_EO_REC12 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1   --useGPU=0 --useEO=true   --useReconstruct12=true --measureCorrelator=1)
#add_test(executables/Inverter7_GPU_TM_REC12 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1      --useGPU=1 --useEO=false  --useReconstruct12=true --measureCorrelator=1 --useCPU=false)
#add_test(executables/Inverter8_GPU_TM_EO_REC12 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1   --useGPU=1 --useEO=true   --useReconstruct12=true --measureCorrelator=1 --useCPU=false)

## tests on a given config
## the config does not contain a valid checksum, the resulting error will be ignored
add_test(executables/Inverter9_CPU_TM ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2             --useGPU=0 --useEO=false --measureCorrelator=1 --ignoreChecksumErrors=true)
add_test(executables/Inverter10_CPU_TM_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2         --useGPU=0 --useEO=true  --measureCorrelator=1 --ignoreChecksumErrors=true)
add_test(executables/Inverter9_CPU_TM_CG ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2          --useGPU=0 --useEO=false --measureCorrelator=1 --ignoreChecksumErrors=true --solver=cg)
add_test(executables/Inverter10_CPU_TM_CG_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2      --useGPU=0 --useEO=true  --measureCorrelator=1 --ignoreChecksumErrors=true --solver=cg)
add_test(executables/Inverter9_CPU_TM_SAVE ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2        --useGPU=0 --useEO=false --measureCorrelator=1 --ignoreChecksumErrors=true --solver=bicgstab_save)
add_test(executables/Inverter10_CPU_TM_SAVE_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2    --useGPU=0 --useEO=true  --measureCorrelator=1 --ignoreChecksumErrors=true --solver=bicgstab_save)
add_test(executables/Inverter13_GPU_TM ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2            --useGPU=1 --useEO=false --measureCorrelator=1 --ignoreChecksumErrors=true --useCPU=false)
add_test(executables/Inverter14_GPU_TM_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2         --useGPU=1 --useEO=true  --measureCorrelator=1 --ignoreChecksumErrors=true --useCPU=false )
add_test(executables/Inverter13_GPU_TM_CG ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2         --useGPU=1 --useEO=false --measureCorrelator=1 --ignoreChecksumErrors=true --useCPU=false --solver=cg)
add_test(executables/Inverter14_GPU_TM_CG_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2      --useGPU=1 --useEO=true  --measureCorrelator=1 --ignoreChecksumErrors=true --useCPU=false --solver=cg)
add_test(executables/Inverter13_GPU_TM_SAVE ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2       --useGPU=1 --useEO=false --measureCorrelator=1 --ignoreChecksumErrors=true --useCPU=false --solver=bicgstab_save)
add_test(executables/Inverter14_GPU_TM_SAVE_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2    --useGPU=1 --useEO=true  --measureCorrelator=1 --ignoreChecksumErrors=true --useCPU=false --solver=bicgstab_save)
## Tests with rec12 active left out since options not available in executable, because broken at the moment
#add_test(executables/Inverter11_CPU_TM_REC12 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2      --useGPU=0 --useEO=false --useReconstruct12=true --measureCorrelator=1 --ignoreChecksumErrors=true)
#add_test(executables/Inverter12_CPU_TM_EO_REC12 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2   --useGPU=0 --useEO=true  --useReconstruct12=true --measureCorrelator=1 --ignoreChecksumErrors=true)
#add_test(executables/Inverter15_GPU_TM_REC12 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2      --useGPU=1 --useEO=false --useReconstruct12=true --measureCorrelator=1 --ignoreChecksumErrors=true --useCPU=false )
#add_test(executables/Inverter16_GPU_TM_EO_REC12 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2   --useGPU=1 --useEO=true  --useReconstruct12=true --measureCorrelator=1 --ignoreChecksumErrors=true --useCPU=false )

# Test the correlators
## the inverter test above have tested also the z-dir correlators, the t-dir correlators are pending
add_test(executables/Inverter17_CPU_TM ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1     --useGPU=0 --useEO=false --measureCorrelator=1 --correlatorDirection=0)
add_test(executables/Inverter18_CPU_TM ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1     --useGPU=1 --useEO=false --measureCorrelator=1 --correlatorDirection=0)
add_test(executables/Inverter19_CPU_TM ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref3 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2    --useGPU=0 --useEO=false --measureCorrelator=1 --correlatorDirection=0 --ignoreChecksumErrors=true )
add_test(executables/Inverter20_CPU_TM ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref3 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2    --useGPU=1 --useEO=false --measureCorrelator=1 --correlatorDirection=0 --ignoreChecksumErrors=true )

# Tests for pure Wilson fermions
## compared to the inverter tests above, only the fermionmatrix is different
#in that it needs less matrix kernels.
#thus only redo tests 1,2,5,6,9,10,13,14 from above
add_test(executables/Inverter21_CPU_W ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1          --useGPU=0 --useEO=false --fermionAction=wilson --measureCorrelator=1 )
add_test(executables/Inverter22_CPU_W_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1       --useGPU=0 --useEO=true  --fermionAction=wilson --measureCorrelator=1 )
add_test(executables/Inverter23_GPU_W ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1          --useGPU=1 --useEO=false --fermionAction=wilson --measureCorrelator=1 --useCPU=false )
add_test(executables/Inverter24_GPU_W_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1       --useGPU=1 --useEO=true  --fermionAction=wilson --measureCorrelator=1 --useCPU=false )
add_test(executables/Inverter21_CPU_W_CG ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1       --useGPU=0 --useEO=false --fermionAction=wilson --measureCorrelator=1 --solver=cg)
add_test(executables/Inverter22_CPU_W_CG_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1    --useGPU=0 --useEO=true  --fermionAction=wilson --measureCorrelator=1 --solver=cg)
add_test(executables/Inverter23_GPU_W_CG ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1       --useGPU=1 --useEO=false --fermionAction=wilson --measureCorrelator=1 --solver=cg --useCPU=false)
add_test(executables/Inverter24_GPU_W_CG_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1    --useGPU=1 --useEO=true  --fermionAction=wilson --measureCorrelator=1 --solver=cg --useCPU=false)
add_test(executables/Inverter21_CPU_W_SAVE ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1     --useGPU=0 --useEO=false --fermionAction=wilson --measureCorrelator=1 --solver=bicgstab_save)
add_test(executables/Inverter22_CPU_W_SAVE_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1  --useGPU=0 --useEO=true  --fermionAction=wilson --measureCorrelator=1 --solver=bicgstab_save)
add_test(executables/Inverter23_GPU_W_SAVE ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1     --useGPU=1 --useEO=false --fermionAction=wilson --measureCorrelator=1 --solver=bicgstab_save --useCPU=false)
add_test(executables/Inverter24_GPU_W_SAVE_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_1  --useGPU=1 --useEO=true  --fermionAction=wilson --measureCorrelator=1 --solver=bicgstab_save --useCPU=false)
## tests on a given config
## the config does not contain a valid checksum, the resulting error will be ignored
add_test(executables/Inverter25_CPU_W ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2           --useGPU=0 --useEO=false --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1)
add_test(executables/Inverter26_CPU_W_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2        --useGPU=0 --useEO=true  --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1)
add_test(executables/Inverter27_GPU_W ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2           --useGPU=1 --useEO=false --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --useCPU=false )
add_test(executables/Inverter28_GPU_W_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2        --useGPU=1 --useEO=true  --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --useCPU=false )
add_test(executables/Inverter25_CPU_W_CG ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2        --useGPU=0 --useEO=false --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --solver=cg)
add_test(executables/Inverter26_CPU_W_CG_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2     --useGPU=0 --useEO=true  --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --solver=cg)
add_test(executables/Inverter27_GPU_W_CG ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2        --useGPU=1 --useEO=false --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --solver=cg --useCPU=false)
add_test(executables/Inverter28_GPU_W_CG_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2     --useGPU=1 --useEO=true  --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --solver=cg --useCPU=false)
add_test(executables/Inverter25_CPU_W_SAVE ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2      --useGPU=0 --useEO=false --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --solver=bicgstab_save)
add_test(executables/Inverter26_CPU_W_SAVE_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2   --useGPU=0 --useEO=true  --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --solver=bicgstab_save)
add_test(executables/Inverter27_GPU_W_SAVE ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2      --useGPU=1 --useEO=false --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --solver=bicgstab_save --useCPU=false)
add_test(executables/Inverter28_GPU_W_SAVE_EO ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testInverter.py ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_ref5 ${CMAKE_CURRENT_SOURCE_DIR}/inverter_test_input_2   --useGPU=1 --useEO=true  --ignoreChecksumErrors=true --fermionAction=wilson --measureCorrelator=1 --solver=bicgstab_save --useCPU=false)

# Test the heatbath
add_test(executables/Heatbath1_CPU_DP ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHeatbath.py ${CMAKE_CURRENT_SOURCE_DIR}/heatbath_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/heatbath_test_input_1)
add_test(executables/Heatbath3_GPU_DP ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHeatbath.py ${CMAKE_CURRENT_SOURCE_DIR}/heatbath_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/heatbath_test_input_3 --useCPU=false)
## Tests in single precision not done at the moment since single precision broken at the moment -> see USEDOUBLEPREC if-else in main CMakeLists.txt file
#add_test(executables/Heatbath5_CPU_SP ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHeatbath.py ${CMAKE_CURRENT_SOURCE_DIR}/heatbath_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/heatbath_test_input_5)
#add_test(executables/Heatbath7_GPU_SP ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHeatbath.py ${CMAKE_CURRENT_SOURCE_DIR}/heatbath_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/heatbath_test_input_7 --useCPU=false)

# Test the HMC
## odd testnumbers correspond to tests using even-odd-precond.,
## even numbers to those who don`t
## LF: Leapfrog
## 2MN: 2mn
## XTS: X timescales
## CONF: starting from config
## ANTI: antiperiodic boundary conditions
add_test(executables/HMC1_CPU_TM_EO_LF_1TS ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_1)
add_test(executables/HMC2_CPU_TM_NONEO_LF_1TS ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_2)
add_test(executables/HMC3_CPU_TM_EO_2MN_1TS ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_3)
add_test(executables/HMC5_CPU_TM_EO_LF_CG_1TS ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_5)
add_test(executables/HMC7_CPU_TM_EO_2MN_2TS ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_7)
add_test(executables/HMC9_CPU_TM_EO_2MN_3TS ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_9)
add_test(executables/HMC11_CPU_TM_EO_LF_1TS_CONF ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref2  ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_11)
add_test(executables/HMC12_CPU_TM_NONEO_LF_1TS_CONF ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref2 ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_12)
add_test(executables/HMC13_CPU_PGT_NONEO_LF_1TS_COLD ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref3 ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_13)
add_test(executables/HMC14_CPU_PGT_RECT_NONEO_LF_1TS_COLD ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref4 ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_14)
add_test(executables/HMC15_CPU_TM_EO_2MN_2TS_ANTI ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_15)
add_test(executables/HMC17_CPU_TM_EO_2MN_3TS_ANTI ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testHmc.py ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_ref ${CMAKE_CURRENT_SOURCE_DIR}/hmc_test_input_17)

# Test the RHMC
## LF: Leapfrog
## 2MN: 2min-norm
## XTS: X timescales
## CONF: starting from config COLD: starting from cold config
## ANTI: antiperiodic boundary conditions
### All these tests are with: EO preconditioning, fermact=rooted_stagg
## The options for the Rational Approximation files are given here to have a global path!!
set(APPROX_FILES "--rationalApproxFileHB=${CMAKE_CURRENT_SOURCE_DIR}/Approx_Heatbath" "--rationalApproxFileMD=${CMAKE_CURRENT_SOURCE_DIR}/Approx_MD" "--rationalApproxFileMetropolis=${CMAKE_CURRENT_SOURCE_DIR}/Approx_Metropolis")
add_test(executables/RHMC_01_CPU_LF_1TS_COLD        ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_1 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_01 ${APPROX_FILES})
add_test(executables/RHMC_02_CPU_LF_1TS_COLD_ANTI   ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_2 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_02 ${APPROX_FILES})
add_test(executables/RHMC_03_CPU_LF_1TS_CONF        ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_3 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_03 ${APPROX_FILES})
add_test(executables/RHMC_04_CPU_LF_1TS_CONF_ANTI   ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_4 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_04 ${APPROX_FILES})
add_test(executables/RHMC_05_CPU_LF_2TS_COLD        ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_1 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_05 ${APPROX_FILES})
add_test(executables/RHMC_06_CPU_LF_2TS_COLD_ANTI   ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_2 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_06 ${APPROX_FILES})
add_test(executables/RHMC_07_CPU_LF_2TS_CONF        ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_3 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_07 ${APPROX_FILES})
add_test(executables/RHMC_08_CPU_LF_2TS_CONF_ANTI   ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_4 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_08 ${APPROX_FILES})
add_test(executables/RHMC_09_CPU_2MN_1TS_COLD       ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_1 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_09 ${APPROX_FILES})
add_test(executables/RHMC_10_CPU_2MN_1TS_COLD_ANTI  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_2 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_10 ${APPROX_FILES})
add_test(executables/RHMC_11_CPU_2MN_1TS_CONF       ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_3 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_11 ${APPROX_FILES})
add_test(executables/RHMC_12_CPU_2MN_1TS_CONF_ANTI  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_4 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_12 ${APPROX_FILES})
add_test(executables/RHMC_13_CPU_2MN_2TS_COLD       ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_1 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_13 ${APPROX_FILES})
add_test(executables/RHMC_14_CPU_2MN_2TS_COLD_ANTI  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_2 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_14 ${APPROX_FILES})
add_test(executables/RHMC_15_CPU_2MN_2TS_CONF       ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_3 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_15 ${APPROX_FILES})
add_test(executables/RHMC_16_CPU_2MN_2TS_CONF_ANTI  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testRhmc.py ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_ref_4 ${CMAKE_CURRENT_SOURCE_DIR}/rhmc_test_input_16 ${APPROX_FILES})
