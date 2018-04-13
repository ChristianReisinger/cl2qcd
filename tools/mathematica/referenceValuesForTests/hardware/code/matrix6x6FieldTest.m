(* ::Package:: *)

(*
 * Copyright (c)
 *
 * This file is part of CL2QCD.
 *
 * CL2QCD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CL2QCD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CL2QCD. If not, see <http://www.gnu.org/licenses/>.
*)

SetOptions[SelectedNotebook[], PrintPrecision -> 16]

(*Get the needed packages for these tests*)
Get[FileNameJoin[{ParentDirectory[ParentDirectory[ParentDirectory[NotebookDirectory[]]]], "packages/cloverPackage.m"}], Path -> {NotebookDirectory[]}]
Get[FileNameJoin[{ParentDirectory[ParentDirectory[ParentDirectory[NotebookDirectory[]]]], "packages/threeBythreeMatrices.m"}], Path -> {NotebookDirectory[]}]
Get[FileNameJoin[{ParentDirectory[ParentDirectory[ParentDirectory[NotebookDirectory[]]]], "packages/real.m"}], Path -> {NotebookDirectory[]}]


(*add up all matrix entries*)
countMatrixField[a_] := Total[a, 2];


(*uniform filling*)
countMatrixField[Inverse[UpperLeftBlock[nonTrivialRealPar,nonTrivialRealPar,cold3x3mat,cold3x3mat], Method -> "CofactorExpansion"]]
countMatrixField[Inverse[LowerRightBlock[nonTrivialRealPar,nonTrivialRealPar,cold3x3mat,cold3x3mat], Method -> "CofactorExpansion"]]
(*ascending + nonTrivial filling*)
countMatrixField[Inverse[UpperLeftBlock[nonTrivialRealPar,nonTrivialRealPar,ascending3x3mat,nonTrivial3x3mat], Method -> "CofactorExpansion"]]
countMatrixField[Inverse[LowerRightBlock[nonTrivialRealPar,nonTrivialRealPar,ascending3x3mat,nonTrivial3x3mat], Method -> "CofactorExpansion"]]


(* calculation of S_det = Log(Det[(1+T_ee)^2]) *)
tmp1=Log[Re[Det[UpperLeftBlock[nonTrivialRealPar,nonTrivialRealPar,ascending3x3mat,nonTrivial3x3mat].UpperLeftBlock[nonTrivialRealPar,nonTrivialRealPar,ascending3x3mat,nonTrivial3x3mat]]]];
tmp2=Log[Re[Det[LowerRightBlock[nonTrivialRealPar,nonTrivialRealPar,ascending3x3mat,nonTrivial3x3mat].UpperLeftBlock[nonTrivialRealPar,nonTrivialRealPar,ascending3x3mat,nonTrivial3x3mat]]]];
tmp1+tmp2
