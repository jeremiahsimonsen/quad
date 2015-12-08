acc_x = ax(4:end,8);
acc_y = ay(4:end,8);
acc_z = az(4:end,8);

DT = 20e-3;
t = [1:length(acc_x)] * DT;

N = 8192;
Hf = fftshift(fft(acc_x,N));
f = linspace(-0.5, 0.5, N)*50;
figure(1);
plot(f,20*log10(abs(Hf)));

y = filter(Hd, acc_x);

figure(2);
plot(t,acc_x,t,y);