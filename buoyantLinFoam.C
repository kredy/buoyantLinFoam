/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | 
   \\    /   O peration     | Copyright (C) 2019 Krishna Reddy Maryada
    \\  /    A nd           | 
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This is a free software: you can redistribute it and/or modify it 
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.
    This software is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warrant of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public Licese for
    more details.
    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
Application
    buoyantLinFoam
Description
    Linear Stability solver for buoyancy-driven flows. Uses the Boussinessq
    approximation in its original form.
\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "pisoControl.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"

    pisoControl piso(mesh);

    #include "createFields.H"
    #include "initContinuityErrs.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

        #include "CourantNo.H"

        #include "UEqn.H"

        while (piso.correct())
        {
            #include "pEqn.H"
        }

	#include "TEqn.H"

	#include "continuityErrs.H"

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
