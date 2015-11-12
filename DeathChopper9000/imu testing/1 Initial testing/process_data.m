clear all;
close all;

dataFile = 'acc_gyro_data_flat_motors_50HzABW.txt';
fd = fopen(dataFile);

data = [];
tline = fgetl(fd);
while ischar(tline)
	data = [data; sscanf(tline, '%f %f %f %f %f %f')'];
	tline = fgetl(fd);
end

ax = data(2:end,1);
ay = data(2:end,2);
az = data(2:end,3);
gx = data(2:end,4);
gy = data(2:end,5);
gz = data(2:end,6);

DT = 20e-3;
t = [1:length(ax)].*DT;

roll = zeros(length(ax),1);
for k = 1:length(ax)
	roll(k) = atan2(ax(k), sqrt(ay(k)^2+az(k)^2)) * 180.0 / pi;
end

figure(1);
N = 1024;
k = 0:N-1;
AX = fft(ax,N);
stem(k,abs(AX));

figure(2);
plot(t,ax,t,ay,t,az);
ylabel('Accelerometer reading (g)');
xlabel('Time (s)');
grid on;

figure(3);
plot(t,gx,t,gy,t,gz);
ylabel('Gyroscope reading (^\circ/s)');
xlabel('Time (s)');
grid on;

% angle_x = 0.0;
% angle_x_arr = [];
% AA = 0.99;
% 
% for k=1:length(data(:,1))
% 	rate_pitch = data(k,4);
% 	angle_x = AA * (angle_x + rate_pitch * DT);
% 	angle_x = angle_x + (1.0-AA)*data(k,1);
% 	angle_x_arr = [angle_x_arr, angle_x];
% end
% 
% plot(angle_x_arr)

s = tf('s');
tau = 0.5;

Ha = 1 / (tau*s + 1);
Hg = tau*s / (tau*s + 1);

figure(4);
bode(Ha);

% figure(5);
% lsim(Ha, ax, t);
% 
% figure(6);
% lsim(Ha, ay, t);
% 
% figure(7);
% lsim(Ha, az, t);



fclose(fd);







