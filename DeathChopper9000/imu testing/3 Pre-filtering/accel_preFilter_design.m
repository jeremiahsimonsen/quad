% Elliptic Lowpass filter designed using FDESIGN.LOWPASS.
clear all;
close all;

% All frequency values are in Hz.
Fs = 100;  % Sampling Frequency

Fpass = 2;       % Passband Frequency
Fstop = 2.5;       % Stopband Frequency
Apass = 0.1;     % Passband Ripple (dB)
Astop = 100;      % Stopband Attenuation (dB)
match = 'both';  % Band to match exactly

% Construct an FDESIGN object and call its ELLIP method.
h  = fdesign.lowpass(Fpass, Fstop, Apass, Astop, Fs);
Hd = design(h, 'ellip', 'MatchExactly', match, 'FilterStructure', ...
            'df2tsos');
		
df2tsos_print(Hd);

% [EOF]
