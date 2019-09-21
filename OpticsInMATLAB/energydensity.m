function U = energydensity(E, lambda, fnum)
% ENERGYDENSITY: Calculate the energy density of the focused light
% USAGE: U = energydensity(E, lambda, fnum)
%
%INPUTS:
% E = The amount of the energy in the light beam
% lambda = wavelength of the light
% fnum = fnumber of the lens
%
%OUTPUTS: The energy density U of the focused light.
%
% (C) RAHUL BHADANI
%
% The energy density U of the focused light can be estimated by knowing
% the amount of energy E in the light beam and dividing by the area A of the
% spot U = E/(pi/4*(2.44*lambda*fnum)^2)

    U = E./(pi/4.*(2.44.*lambda.*fnum).^2);
end

