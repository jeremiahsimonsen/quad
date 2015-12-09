function Hd = pass1_stop2
%PASS1_STOP2 Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 8.6 and the Signal Processing Toolbox 7.1.
% Generated on: 08-Dec-2015 09:52:28

% Elliptic Lowpass filter designed using FDESIGN.LOWPASS.

% All frequency values are in Hz.
Fs = 100;  % Sampling Frequency

Fpass = 1;       % Passband Frequency
Fstop = 2;       % Stopband Frequency
Apass = 0.1;     % Passband Ripple (dB)
Astop = 80;      % Stopband Attenuation (dB)
match = 'both';  % Band to match exactly

% Construct an FDESIGN object and call its ELLIP method.
h  = fdesign.lowpass(Fpass, Fstop, Apass, Astop, Fs);
Hd = design(h, 'ellip', 'MatchExactly', match, 'FilterStructure', ...
            'df2tsos');
		
df2tsos_print(Hd);

% [EOF]
