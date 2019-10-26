Info<< "Reading transportProperties\n" << endl;

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);


// Read Prandtl number
dimensionedScalar Pr
(
    "Pr",
    dimless,
    transportProperties.lookup("Pr")

);

// Read Rayleigh number
dimensionedScalar Ra
(
    "Ra",
    dimless,
    transportProperties.lookup("Ra")
);

// Read reference temperature
dimensionedScalar TRef
(
    "TRef",
    dimTemperature,
    transportProperties.lookup("TRef")
);

// Temporary variable to match dimensions of beta
dimensionedScalar betatmp
(
    "betatmp",
    dimless/dimTemperature,
    scalar(1.0)
);

// Temporary variable to match dimensions of alpha and nu
dimensionedScalar alphatmp
(
    "alphatmp",
    dimViscosity,
    scalar(1.0)
    );

// Calculate beta
dimensionedScalar beta
(
    "beta",
    dimless/dimTemperature,
    1.0
);
beta = Pr * betatmp;

// Calculate alpha
dimensionedScalar alpha
(
    "alpha",
    dimViscosity,
    1.0
);
alpha = alphatmp * (1/sqrt(Ra));

// Calculate nu
dimensionedScalar nu
(
    "nu",
    dimViscosity,
    1.0
);
nu = alpha * Pr;

Info<< "Reading thermophysical properties\n" << endl;
Info<< "Reading field p\n" << endl;
volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field T\n" << endl;
volScalarField T
(
    IOobject
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
        ),
    mesh
);

/* Read the base flow fields */

Info<< "Reading field Ub\n" << endl;
volVectorField Ub
(
    IOobject
    (
        "Ub",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

Info<< "Reading field Tb\n" << endl;
volScalarField Tb
(
    IOobject
    (
        "Tb",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    ),
    mesh
);


// Pertub temperature field
Random obj(314);
forAll (mesh.C(), id)
{
    const scalar randNumber(obj.scalar01());;
    T.primitiveFieldRef()[id] = 0.001 * randNumber;
}


// Create phi
Info<< "Calculating face flux field phi of the perturbation\n" << endl;
surfaceScalarField phi
(
    IOobject
    (
        "phi",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    fvc::flux(U)
);


// Create phib
Info<< "Calculating face flux field phib of the base flow\n" << endl;
surfaceScalarField phib
(
    IOobject
    (
        "phib",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE
    ),
    fvc::flux(Ub)
);

// Read acceleration due to gravity
#include "readGravitationalAcceleration.H"

// Set reference value for pressure to solve for certain boundary conditions
label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, mesh.solutionDict().subDict("PISO"), pRefCell, pRefValue);
mesh.setFluxRequired(p.name());
