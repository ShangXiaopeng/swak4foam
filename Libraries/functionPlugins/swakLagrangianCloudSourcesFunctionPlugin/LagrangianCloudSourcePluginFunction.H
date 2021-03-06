/*---------------------------------------------------------------------------*\
 ##   ####  ######     |
 ##  ##     ##         | Copyright: ICE Stroemungsfoschungs GmbH
 ##  ##     ####       |
 ##  ##     ##         | http://www.ice-sf.at
 ##   ####  ######     |
-------------------------------------------------------------------------------
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2008 OpenCFD Ltd.
     \\/     M anipulation  |
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

Class
    Foam::LagrangianCloudSourcePluginFunction

Description

SourceFiles
    LagrangianCloudSourcePluginFunction.C

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#ifndef LagrangianCloudSourcePluginFunction_H
#define LagrangianCloudSourcePluginFunction_H

#include "FieldValuePluginFunction.H"

#include "runTimeSelectionTables.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class LagrangianCloudSourcePluginFunction Declaration
\*---------------------------------------------------------------------------*/

    // template<class CloudType>
class LagrangianCloudSourcePluginFunction
:
    public FieldValuePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const LagrangianCloudSourcePluginFunction &);

    LagrangianCloudSourcePluginFunction(const LagrangianCloudSourcePluginFunction &);

    word cloudName_;

protected:
    //    typedef CloudType cloudType;

    template<class LookupCloudType>
    bool hasCloudAs();

    template<class OtherCloudType,class LookupCloudType>
    const OtherCloudType &getCloudAs();

    const word &cloudName() const { return cloudName_; }

public:
    LagrangianCloudSourcePluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name,
        const word &returnValueType,
        const string &additionalArgs=""
    );

    virtual ~LagrangianCloudSourcePluginFunction() {}

    //    TypeName("LagrangianCloudSourcePluginFunction");

    void setArgument(label index,const word &);

};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "LagrangianCloudSourcePluginFunctionI.H"
#endif

// set a autoPtr to the needed cloud. This is required because
// "similar" clouds don't really inherit (because of the template paramter)
#define castAndCall(rVar,rType,neededType,usedType,theCall)    \
    if(!rVar.valid() && hasCloudAs<neededType>()) { \
        rVar.set( \
            new rType( \
                getCloudAs<neededType,usedType>().theCall \
            ) \
        );\
    }\

// Fail if no cloud data was found
#define noCloudFound(pVar) \
    if(!pVar.valid()) { \
    FatalErrorIn("Plugin function "+this->name()) \
    << "No matching cloud " << this->cloudName() << "found" \
        << endl \
    << exit(FatalError); \
    }


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
