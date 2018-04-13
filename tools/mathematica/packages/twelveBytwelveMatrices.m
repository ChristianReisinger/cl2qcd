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

BeginPackage["TwelveByTwelveMatrices`"]
cold12x12mat::usage =
	"cold12x12mat gives the 12-by-12 block diagonal matrix with two 6-by-6 identity matrices as diagonal blocks"
nonTrivial12x12mat::usage =
	"nonTrivial12x12mat gives a non trivial 12-by-12 block diagonal matrix with two 6-by-6 non trivial matrices as diagonal blocks"

Begin["Private`"]

cold12x12mat:=
	Module[{cold6x6mat=ArrayFlatten[{{IdentityMatrix[6],ConstantArray[0, {6, 6}]},{ConstantArray[0, {6, 6}],IdentityMatrix[6]}}]},
	cold6x6mat
	]

nonTrivial12x12mat:=
	Module[{nonTrivial6x6mat=ArrayFlatten[{{Table[6*(x-1)+y,{x,6},{y,6}],ConstantArray[0, {6, 6}]},{ConstantArray[0, {6, 6}],Table[6*(x-1)+y,{x,6},{y,6}]}}]},
	nonTrivial6x6mat
	]

End[]

EndPackage[]
