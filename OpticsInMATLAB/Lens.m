classdef Lens
    % A class for describing Simple symmetric lens
    % (C) Rahul Bhadani May 15, 2019
    
    %% Properties
    % Static Constants
    properties(Constant=true)
        NBK7 = 1.5168;
        NAir = 1.0;
    end
    
    % Public properties
    properties(Access='public')
        n = Lens.NBK7; % Refractive Index of the lens
        D;
        R1;
        R2;
        t;
        lambda;
        lensMatrix = [];
    end
    
    properties(GetAccess = 'public', SetAccess = 'protected')
        phi;
        f;
        fnum;
        S;
    end
    %% Methods
    methods
        function obj = Lens(n, lambda, D, R1, R2, thin, t)
            
            % This is the Lens constructor.
            % A simple symmetric lens can be specified by 5 parameters: its refractive
            % index, diameter, raii of curvature, thickness along the
            % optical axis
            % If no argument is passed, the constructor will construct a
            % unit lens with refractive index of matrial N-BK7 at
            % wavelength of 587.6 nm
            if(nargin == 0)
                obj.n = Lens.NBK7;
                obj.D = 1.0;
                obj.R1 = 1.0;
                obj.R2 = -1.0;
                obj.lambda = 587.6e-9;
                obj.t = 1e-6; %If the thickness of the thin lens is much smaller than diameter of the lens, we call it thick lens
            elseif(nargin >= 1 && nargin < 6)
                error('Wrong number of input arguments. Required: Lens(refractive_index, Diameter, Radius1, Radius2, thickness');
                clear obj;
            else
                obj.n = n;
                obj.D = D;
                obj.R1 = R1;
                obj.R2 = R2;
                obj.lambda = lambda;
                if(nargin == 6 && thin == true)
                    obj.t = 1e-6;
                elseif(nargin == 6 && thin == false)
                    error('Requesting thick lens but thickness of the lens is missing');
                    clear obj;
                else
                    obj.t = t;
                end
            end
            obj.phi = (n-1).*(1/R1 - 1/R2 + (n-1).*(t/(n.*R1.*R2)) );
            obj.f = 1./obj.phi;
            obj.fnum = (obj.f)./D;
            obj.S = 2.44.*lambda.*(obj.fnum);
        end
        
        function [F, MLens, MTotal] = calcLensMatrix(obj)
           % We will assume for the time that, ray is comming parallel to
           % the optical axis at a height y0, so it intersects at the focal length of the
           % lens
           %
           
            % Configuring translation and refractive matrices
            P1 = (Lens.NAir - obj.n)./(-obj.R1); %First Surface Lens Power
            P2 = (obj.n - Lens.NAir)./(-obj.R2); % Second Surface Lens Power
            tempf = 0;
            deviation = 10;
            MLens = zeros(2);
            MTotal= zeros(2);
            while abs(deviation) > 0.0005
                tempf = tempf+0.001;
                % Left Most Tx Matrix
                TM3 = [1, 0; 0, 1]; %Rays are coming parallel to optical axis, so tx matrix is identity
                % Refraction Matrix at left surface
                RM2 = [1 -P2; 0 1];
                % Tranmission within the lens
                TM2 = [1 , 0;obj.t/obj.n, 1];
                % Refraction matrix at right surface
                RM1 = [1 -P1;0  1];
                %Tranmission matrix in the air, at the point it converges
                TM1 = [1 0; tempf, 1];

                % Calculate Lens transfer matrix
                MLens = RM1*TM2*RM2;
                MTotal = TM1*MLens*TM3;
                deviation = MTotal(2,2);
            end
            F = tempf;
        end
    end
end