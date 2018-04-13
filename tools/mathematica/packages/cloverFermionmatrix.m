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
Get["cloverPackage.m",Path->{NotebookDirectory[]}]


(*Clover Fermionmatrix for a specific site, for constant gauge- and \
spinorfield*)


BeginPackage["clover`"]

Clover::usage =
	"compute Clover Matrix 1+T taking kappa, csw,  a spinor filling and a 3x3matrix filling."
CloverInverse::usage =
	"compute inverse Clover Matrix (1+T)^(-1) taking kappa, csw,  a spinor filling and a 3x3matrix filling."


Begin["Private`"]
Needs["cloverPackage`"]
Needs["PauliMatricesTensorColorIdentity`"]

Clover[k_, csw_, s_, u_, v_] :=
	Module[{clover = ArrayFlatten[{{UpperLeftBlock[k,csw,u,v],0},{0,LowerRightBlock[k,csw,u,v]}}].s},
	clover
	]

CloverInverse[k_, csw_, s_, u_, v_] :=
	Module[{cloverInv = ArrayFlatten[{{Inverse[UpperLeftBlock[k,csw,u,v], Method -> "CofactorExpansion"],0},{0,Inverse[LowerRightBlock[k,csw,u,v], Method -> "CofactorExpansion"]}}].s},
	cloverInv
	]

End[]

EndPackage[]
