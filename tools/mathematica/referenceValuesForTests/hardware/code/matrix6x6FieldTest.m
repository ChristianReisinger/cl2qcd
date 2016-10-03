(* ::Package:: *)

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






