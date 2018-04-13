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

BeginPackage["PauliMatricesTensorColorIdentity`"]

  Pauli1::usage =
  	"Pauli1[x] gives the first Pauli matrix, tensor multiplied
  	with the 3-by-3 x matrix in color space."

 Pauli2::usage =
  	"Pauli2[x] gives the secound Pauli matrix, tensor multiplied
  	with the 3-by-3 x matrix in color space."

  Pauli3::usage =
  	"Pauli3[x] gives the third Pauli matrix, tensor multiplied
  	with the 3-by-3 x matrix in color space."

UnitMatrixx::usage =
	"UnitMatrixx[x] gives the Identity 2-by-2 matrix, tensor multiplied
	with the 3-by-3 x matrix in color space."

Begin["Private`"]

  Pauli1[x_] :=
  	Module[{pauli1=ArrayFlatten[{{0, 1*x}, {1*x, 0}}]},
  	pauli1
  	]

  Pauli2[x_] :=
  	Module[{pauli2=ArrayFlatten[{{0, -I*x}, {I*x, 0}}]},
  	pauli2
  	]

  Pauli3[x_] :=
  	Module[{pauli3=ArrayFlatten[{{x, 0}, {0, -x}}]},
  	pauli3
  	]

  UnitMatrixx[x_] :=
  	Module[{unitMatrixx=ArrayFlatten[{{x, 0},{0, x}}]},
  	unitMatrixx
  	]

  End[]

  EndPackage[]
