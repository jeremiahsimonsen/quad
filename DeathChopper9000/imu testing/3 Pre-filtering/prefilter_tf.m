clear all;
close all;

f = linspace(-.5,.5,1024);	% normalized frequency
z = exp(1i*2*pi.*f);
tau = 100000;

H = (1+z.^(-1)) ./ (tau+1+(1-tau).*z.^(-1));

figure(1);
plot(f, 20*log10(abs(H)));