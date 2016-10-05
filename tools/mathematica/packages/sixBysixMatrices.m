(* ::Package:: *)

BeginPackage["SixBySixMatrices`"]
cold6x6mat::usage =
	"cold6x6mat gives the 6-by-6 Identity matrix"
nonTrivial6x6mat::usage = 
	"nonTrivial6x6mat gives a non trivial 6-by-6 matrix"

Begin["Private`"]

cold6x6mat:=
	Module[{cold6x6mat=IdentityMatrix[6]},
	cold6x6mat
	]

nonTrivial6x6mat:=
	Module[{nonTrivial6x6mat=Table[x*y,{x,6},{y,6}]},
	nonTrivial6x6mat
	]

End[]

EndPackage[]


