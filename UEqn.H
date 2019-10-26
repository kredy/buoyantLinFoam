// Solve the linearised momentum equation with the Boussinessq approximation


fvVectorMatrix UEqn
(
    fvm::ddt(U)
    +  fvm::div(phib, U)
    +  fvc::div(phi, Ub) 
    -  fvm::laplacian(nu, U)
);

if (piso.momentumPredictor())
{
    solve
    (
        UEqn
        ==
        - fvc::grad(p)
        + fvc::reconstruct
        (
            -fvc::interpolate(beta*(T-TRef))*(g & mesh.Sf())
        )
    );
}
