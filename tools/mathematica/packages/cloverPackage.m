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
Get["pauliMatricesTensorColorIdentity.m",Path->{NotebookDirectory[]}]


BeginPackage["cloverPackage`"]

FieldStrengthTensor::usage =
	"FieldStrengtTensor[u] for a given uniform gaugefield filling."

FieldStrengthTensor0i::usage =
	"FieldStrengtTensor[u,v] for a different filling for temporal and spatial direction."

Sigma::usage =
	"Sigma[x,y] gives the gamma-matrices defined via the commuator."

EplusB::usage =
	"E+B"

EminusB::usage =
	"E-B"

UpperLeftBlock::usage =
	"upper-left block of 1+T"

LowerRightBlock::usage =
	"lower-right block of 1+T"


Begin["Private`"]
Needs["PauliMatricesTensorColorIdentity`"]

FieldStrengthTensor[u_] :=
	Module[{fst=1/8*(u.u.ConjugateTranspose[u].ConjugateTranspose[u]+u.ConjugateTranspose[u].ConjugateTranspose[u].u+ConjugateTranspose[u].ConjugateTranspose[u].u.u+ConjugateTranspose[u].u.u.ConjugateTranspose[u]-u.u.ConjugateTranspose[u].ConjugateTranspose[u]-ConjugateTranspose[u].u.u.ConjugateTranspose[u]-ConjugateTranspose[u].ConjugateTranspose[u].u.u-u.ConjugateTranspose[u].ConjugateTranspose[u].u)},
	fst
	]

FieldStrengthTensor0i[u_, v_] :=
	Module[{fst0i=1/8*(u.v.ConjugateTranspose[u].ConjugateTranspose[v]+v.ConjugateTranspose[u].ConjugateTranspose[v].u+ConjugateTranspose[u].ConjugateTranspose[v].u.v+ConjugateTranspose[v].u.v.ConjugateTranspose[u]-v.u.ConjugateTranspose[v].ConjugateTranspose[u]-ConjugateTranspose[u].v.u.ConjugateTranspose[v]-ConjugateTranspose[v].ConjugateTranspose[u].v.u-u.ConjugateTranspose[v].ConjugateTranspose[u].v)},
	fst0i
	]

Sigma[x_,y_] :=
	Module[{sigma = I/2*(x.y-y.x)},
	sigma
	]

EplusB[u_, v_] :=
	Module[{epb = 8 * FieldStrengthTensor0i[u, v](* + (4 * FieldStrengthTensor[u] - 4 * FieldStrengthTensor[u])*)},
	epb
	]

EminusB[u_, v_] :=
	Module[{emb = 8 * FieldStrengthTensor0i[u, v](* - (4 * FieldStrengthTensor[u] - 4 * FieldStrengthTensor[u])*)},
	emb
	]

UpperLeftBlock[k_, csw_, u_, v_] :=
	Module[{UpperLeftBlock = IdentityMatrix[6] + 1/8 * k *  csw * I * (Pauli1[EminusB[u,v]] + Pauli2[EminusB[u,v]] + Pauli3[EminusB[u,v]])},
	UpperLeftBlock
	]

LowerRightBlock[k_, csw_, u_, v_] :=
	Module[{LowerRightBlock = IdentityMatrix[6] - 1/8 * k * csw * I * (Pauli1[EplusB[u,v]] + Pauli2[EplusB[u,v]] + Pauli3[EplusB[u,v]])},
	LowerRightBlock
	]

End[]

EndPackage[]
