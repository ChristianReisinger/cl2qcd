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

BeginPackage["SixBySixMatrices`"]
cold6x6mat::usage =
	"cold6x6mat gives the 6-by-6 Identity matrix"
nonTrivial6x6mat::usage =
	"nonTrivial6x6mat gives a non trivial 6-by-6 matrix"
nonTrivial6x6mat2::usage =
	"nonTrivial6x6mat2 gives a non trivial 6-by-6 matrix with ascending real filling"

Begin["Private`"]

cold6x6mat:=
	Module[{cold6x6mat=IdentityMatrix[6]},
	cold6x6mat
	]

nonTrivial6x6mat:=
	Module[{nonTrivial6x6mat=Table[x*y,{x,6},{y,6}]},
	nonTrivial6x6mat
	]

nonTrivial6x6mat2:=
	Module[{nonTrivial6x6mat2=Table[6*(x-1)+y,{x,6},{y,6}]},
	nonTrivial6x6mat2
	]

End[]

EndPackage[]
