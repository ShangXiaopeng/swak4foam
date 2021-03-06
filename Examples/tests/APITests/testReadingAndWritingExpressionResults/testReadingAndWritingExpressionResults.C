/*---------------------------------------------------------------------------*\
 ##   ####  ######     |
 ##  ##     ##         | Copyright: ICE Stroemungsfoschungs GmbH
 ##  ##     ####       |
 ##  ##     ##         | http://www.ice-sf.at
 ##   ####  ######     |
-------------------------------------------------------------------------------
 =========                   |
 \\      /   F ield          | OpenFOAM: The Open Source CFD Toolbox
  \\    /    O peration      |
   \\  /     A nd            | Copyright (C) 1991-2005 OpenCFD Ltd.
    \\/      M anipulation   |
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
    testReadingAndWritingExpressionResults

Description

Contributors/Copyright:
    2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

#include "CommonValueExpressionDriver.H"

#include "timeSelector.H"

#include "IFstream.H"
#include "OFstream.H"

#include "printSwakVersion.H"

#include "ExpressionResult.H"
#include "DelayedExpressionResult.H"
#include "StackExpressionResult.H"
#include "StoredStackExpressionResult.H"
#include "StoredExpressionResult.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    Foam::argList::validArgs.append("fileToWrite");

#   include "setRootCase.H"
#   include "createTime.H"
    runTime.functionObjects().off();
#   include "createMesh.H"

    printSwakVersion();

    fileName theFile(args.args()[1]);

    {
        Info << "Writing to " << theFile << endl;

        scalarField sField(42);
        vectorField vField(42);
        forAll(sField,i) {
            sField[i]=i*i;
            vField[i]=vector(i,i*i,i*i*i);
        }
        ExpressionResult sResult(sField);
        ExpressionResult vResult(vField);
        ExpressionResult uResult(1.2);

        StackExpressionResult stackResult;
        StoredStackExpressionResult storedStackResult;
        ExpressionResult &ssResult=storedStackResult;
        DelayedExpressionResult dResult;
        StoredExpressionResult storedResult;
        storedResult=sResult;

        stackResult=uResult;
        stackResult=uResult;
        ssResult=uResult;
        dResult=vResult;
        dResult.storeValue(runTime.time().value());

        runTime++;
        // stackResult.reset();
        // storedStackResult.reset();
        stackResult=uResult;
        stackResult=uResult;
        ssResult=uResult;
        dResult=1.5*vResult;
        dResult.storeValue(runTime.time().value());

        runTime++;
        // stackResult.reset();
        // storedStackResult.reset();
        stackResult=uResult;
        stackResult=uResult;
        ssResult=uResult;
        dResult=2*vResult;
        dResult.storeValue(runTime.time().value());

        IOdictionary data(
            IOobject(
                theFile,
                mesh.time().timeName(),
                mesh
            )
        );

        data.add("sResult",sResult);
        data.add("stackResult",stackResult);
        data.add("storedStackResult",storedStackResult);
        data.add("vResult",vResult);
        data.add("uResult",uResult);
        data.add("dResult",dResult);
        data.add("storedResult",storedResult);

        data.write();
    }

    {
        Info << "Reading from " << theFile << endl;

        IOdictionary data(
            IOobject(
                theFile,
                mesh.time().timeName(),
                mesh,
                IOobject::MUST_READ
            )
        );

        ExpressionResult sResult(data.subDict("sResult"));
        ExpressionResult vResult(data.subDict("vResult"));
        ExpressionResult uResult(data.subDict("uResult"));
        StackExpressionResult stackResult(data.subDict("stackResult"));
        StoredStackExpressionResult storedStackResult(data.subDict("storedStackResult"));
        DelayedExpressionResult dResult(data.subDict("dResult"));
        StoredExpressionResult storedResult(data.subDict("storedResult"));
    }

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
