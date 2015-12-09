% Elliptic Lowpass filter designed using FDESIGN.LOWPASS.
clear all;
close all;

% All frequency values are in Hz.
Fs = 100;  % Sampling Frequency

Fpass = 12.5;       % Passband Frequency
Fstop = 15;       % Stopband Frequency
Apass = 0.1;     % Passband Ripple (dB)
Astop = 40;      % Stopband Attenuation (dB)
match = 'both';  % Band to match exactly

% Construct an FDESIGN object and call its ELLIP method.
h  = fdesign.lowpass(Fpass, Fstop, Apass, Astop, Fs);
Hd = design(h, 'ellip', 'MatchExactly', match, 'FilterStructure', ...
            'df2tsos');
		
df2tsos_print(Hd);

% [EOF]
