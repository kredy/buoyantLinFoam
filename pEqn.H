// Solve for pressure and correct the velocity terms


// OpenFoam's version of Rhie-Chow interpolation
volScalarField rAU("rAU", 1.0/UEqn.A());
surfaceScalarField rAUf("rAUf", fvc::interpolate(rAU));
volVectorField HbyA("HbyA", constrainHbyA(rAU*UEqn.H(), U, p));

surfaceScalarField phig
(
    "phig",
    -rAUf*fvc::interpolate(beta*((T-TRef)))
    *(g & mesh.Sf())
);

surfaceScalarField phiHbyA
(
    "phiHbyA",
    fvc::flux(HbyA)
    + rAUf*fvc::ddtCorr(U,phi)
);

// Adjust the face velocities at the boundaries to conserve mass
adjustPhi(phiHbyA, U, p);
phiHbyA = phiHbyA + phig;

// Adjust the pressure BCs to conserve mass
constrainPressure(p, U, phiHbyA, rAUf);

// PISO stuff
while (piso.correctNonOrthogonal())
{

    fvScalarMatrix pEqn
    {
        fvm::laplacian(rAUf, p) == fvc::div(phiHbyA)
    };
    
    pEqn.setReference(pRefCell, pRefValue);
    pEqn.solve();

    if (piso.finalNonOrthogonalIter())
    {
        phi = phiHbyA - pEqn.flux();
    }
}


// Correct velocity
U = HbyA + rAU*(fvc::reconstruct(phig/rAUf) - fvc::grad(p));
U.correctBoundaryConditions();



