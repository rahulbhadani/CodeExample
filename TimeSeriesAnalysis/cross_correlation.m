function [autocorrelation, lags]  =  cross_correlation(data_src_1, data_src_2)
% CROSS_CORRELATION Determine the cross-correlation of two timeseries data
%
% [autocorrelation, lags]  = CROSS_CORRELATION(data_src_1, data_src_2) provides a
% cross-correlation metric between two timeseries data data_src_1, and data_src_2
%
 % Author: Rahul Bhadani
% Maintainer Email: rahulbhadani@email.arizona.edu
% License: MIT License 
% Copyright 2019-2020 Rahul Bhadani
% Initial Date: Dec 19, 2019

% Permission is hereby granted, free of charge, to any person obtaining 
% a copy of this software and associated documentation files 
% (the "Software"), to deal in the Software without restriction, including
% without limitation the rights to use, copy, modify, merge, publish,
% distribute, sublicense, and/or sell copies of the Software, and to 
% permit persons to whom the Software is furnished to do so, subject 
% to the following conditions:

% The above copyright notice and this permission notice shall be 
% included in all copies or substantial portions of the Software.

% THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF 
% ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
% TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
% PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT 
% SHALL THE AUTHORS, COPYRIGHT HOLDERS OR ARIZONA BOARD OF REGENTS
% BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN 
% AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
% OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
% OR OTHER DEALINGS IN THE SOFTWARE.


delimiter = ',';
dataStartLine = 2;
extraColRule = 'ignore';

opts = delimitedTextImportOptions(...
                                'Delimiter',delimiter,...
                                'DataLines', dataStartLine,...
                                'ExtraColumnsRule',extraColRule); 
                            
src_1 = readmatrix(data_src_1);
src_1= src_1(:, 6) ;
src_2 = readmatrix(data_src_2);
src_2 = src_2(:, 6);

[autocorrelation, lags] = xcorr(src_1, src_2);

figure;
stem(lags,autocorrelation);


end