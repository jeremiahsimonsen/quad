gyro_x = gx(4:end,8);
gyro_y = gy(4:end,8);
gyro_z = gz(4:end,8);

DT = 20e-3;
t = [1:length(gyro_x)] * DT;

% Unfiltered
N = 8192;
Hf = fftshift(fft(gyro_y,N));
f = linspace(-0.5,0.5,N);
figure(1);
plot(f,20*log10(abs(Hf)));

% Filter the data
y = filter(Hd,gyro_y);

figure(2);
plot(t,gyro_y,t,y);