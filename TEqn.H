// Solve the linearised temperature equation

fvScalarMatrix TEqn
(
    fvm::ddt(T)
    +  fvm::div(phib, T)
    +  fvc::div(phi, Tb)
    ==
    fvm::laplacian(alpha, T)
);

TEqn.solve();
