function Hd = pass5_stop25
%PASS5_STOP25 Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 8.6 and the Signal Processing Toolbox 7.1.
% Generated on: 06-Dec-2015 18:48:14

% Elliptic Lowpass filter designed using FDESIGN.LOWPASS.

% All frequency values are in Hz.
Fs = 500;  % Sampling Frequency

Fpass = 5;       % Passband Frequency
Fstop = 25;      % Stopband Frequency
Apass = 0.5;     % Passband Ripple (dB)
Astop = 60;      % Stopband Attenuation (dB)
match = 'both';  % Band to match exactly

% Construct an FDESIGN object and call its ELLIP method.
h  = fdesign.lowpass(Fpass, Fstop, Apass, Astop, Fs);
Hd = design(h, 'ellip', 'MatchExactly', match);

% Get the transfer function values.
[b, a] = tf(Hd);
g = prod(Hd.ScaleValues);				% Calculate gain factor

% Convert to a singleton filter.
Hd = dfilt.df2(b, a);
a = Hd.Denominator; b = Hd.Numerator;	% Grab coefficients
z = roots(b); p = roots(a);				% Calculate poles/zeros
zp2biquad(z,p,g,'pass5_stop25.c');		% Dump filter coefs to file

% [EOF]
