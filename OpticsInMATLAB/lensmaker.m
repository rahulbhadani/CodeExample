function [S, fnum, phi, f] = lensmaker(R1, R2, n, D, t, lambda)
% LENSMAKER: Calculate the focal length, power , f-number and sport diameter of the
% lens.
%
% USAGE: [S, fnum, phi, f] = lensmaker(R1, R2, n, D)
%
% INPUTS:
% R1: Radius of the right surface of the lens
% R2: Radius of the left surface of the lens
% n: Refractive index of the material for the given wavelength
% D: Diameter of the lens
% t: thickness of the lens along the optical axis
% lambda: Wavelength of the light
%
% OUTPUTS:
% S: Spot diameter
% fnum: f-number of the lens
% phi = optical power of the lens
% f = focal length of the lens
%
% (C) RAHUL BHADANI, May 15, 2019
% This program implements lens maker equations for paraxial optis
% \phi = 1/f = (n-1)[ 1/R1 - 1/R2 + (n-1)t/(nR1R2) ]
% Here phi is the optical power of the lens, R1 and R2 are radii of
% curvature of the lens surfaces and t is the thickness of the lens along
% the optical axis.

% The diameter of the focused spot of light S at the focal point cannot be
% infinitesimally small. The diameter of the focused spot according to the
% Rayleigh criterion is given by S= 2.44*lambda*f/D where lambda is the
% wavelength of the light, D is the diameter of the lens and f is the focal
% length obtained fron the lens maker equations.

% f# = f/D where f$ is the f-number. So Sport diameter can be written in
% terms of the f-number as follows:

% S = 2.44*lambda*f#



    phi = (n-1).*(1/R1 - 1/R2 + (n-1).*(t/(n.*R1.*R2)) );
    f = 1./phi;
    fnum = f./D;
    S = 2.44.*lambda.*fnum;
end