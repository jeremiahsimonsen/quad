clear all;
close all;

% N = length(ax(:,2));
% 
% y_ax = zeros(N,1);
% y_ay = zeros(N,1);
% y_az = zeros(N,1);
% 
% y_gx = zeros(N,1);
% y_gy = zeros(N,1);
% y_gz = zeros(N,1);

tau = 15;
tau2 = 0.8;

%% Transfer functions
f = linspace(-.5,.5,1024);	% normalized frequency
z = exp(1i*2*pi.*f);
% Ha = (1 + z.^(-1)) ./ (tau2+1 + (1-tau2).*z.^(-1));
% Hg = (tau2 + tau2*z.^(-1)) ./ (tau2+1 + (1-tau2).*z.^(-1));
Hg = (tau2*.02) ./ (1-tau2.*z.^(-1));
Ha = (1-tau2) ./ (1-tau2.*z.^(-1));

figure(1);
plot(f, 20*log10(abs(Hg)),f,20*log10(abs(Ha)));
ylim([-60 30]);

%% Pre-filter the raw accelerometer/gyroscope data
for n = 1:N
	if n == 1
		y_ax(n) = (ax(n,2) + 0 - 0) / (tau + 1);
		y_ay(n) = (ay(n,2) + 0 - 0) / (tau + 1);
		y_az(n) = (az(n,2) + 0 - 0) / (tau + 1);
		
		y_gx(n) = (gx(n,2) + 0 - 0) / (tau + 1);
		y_gy(n) = (gy(n,2) + 0 - 0) / (tau + 1);
		y_gz(n) = (gz(n,2) + 0 - 0) / (tau + 1);
	else
		y_ax(n) = (ax(n,2) + ax(n-1,2) - y_ax(n-1)*(1-tau)) / (tau + 1);
		y_ay(n) = (ay(n,2) + ay(n-1,2) - y_ay(n-1)*(1-tau)) / (tau + 1);
		y_az(n) = (az(n,2) + az(n-1,2) - y_az(n-1)*(1-tau)) / (tau + 1);
		
		y_gx(n) = (gx(n,2) + gx(n-1,2) - y_gx(n-1)*(1-tau)) / (tau + 1);
		y_gy(n) = (gy(n,2) + gy(n-1,2) - y_gy(n-1)*(1-tau)) / (tau + 1);
		y_gz(n) = (gz(n,2) + gz(n-1,2) - y_gz(n-1)*(1-tau)) / (tau + 1);
	end
end
% Round to 0
y_ax(abs(y_ax) < 1e-6) = 0.0;
y_ay(abs(y_ay) < 1e-6) = 0.0;
y_az(abs(y_az) < 1e-6) = 0.0;
y_gx(abs(y_gx) < 1e-6) = 0.0;
y_gy(abs(y_gy) < 1e-6) = 0.0;
y_gz(abs(y_gz) < 1e-6) = 0.0;

%% Graph unfiltered and filtered raw accelerometer data
% figure(2);
% plot(ax(:,2));
% hold on;
% plot(y_ax,'r');
% xlabel('Sample number');
% ylabel('Accelerometer X output (g)');
% 
% figure(3);
% plot(ay(:,2));
% hold on;
% plot(y_ay,'r');
% xlabel('Sample number');
% ylabel('Accelerometer Y output (g)');
% 
% figure(4);
% plot(az(:,2));
% hold on;
% plot(y_az,'r');
% xlabel('Sample number');
% ylabel('Accelerometer Z output (g)');

% figure(5);
% plot(gx(:,2));
% hold on;
% plot(y_gx,'r');
% xlabel('Sample number');
% ylabel('Gyroscope X output (deg/s');

%% Angles using only accelerometer (unfiltered vs filtered)
acc_ang_x = zeros(N,1); acc_ang_y = zeros(N,1);
acc_ang_x_f = zeros(N,1); acc_ang_y_f = zeros(N,1);
for n = 1:N
	acc_ang_x(n) = atan2(ax(n,2), sqrt(ay(n,2)^2+az(n,2)^2));
	acc_ang_y(n) = atan2(ay(n,2), sqrt(ax(n,2)^2+az(n,2)^2));
	
	acc_ang_x_f(n) = atan2(y_ax(n), sqrt(y_ay(n)^2+y_az(n)^2));
	acc_ang_y_f(n) = atan2(y_ay(n), sqrt(y_ax(n)^2+y_az(n)^2));
end

% figure(6);
% plot(acc_ang_x);
% hold on;
% plot(acc_ang_x_f, 'r');
% xlabel('Sample number');
% ylabel('X angle from accelerometer');
% 
% figure(7);
% plot(acc_ang_y);
% hold on;
% plot(acc_ang_y_f, 'r');
% xlabel('Sample number');
% ylabel('Y angle from accelerometer');

%% Complementary filtering
f_ang_x  = zeros(N,1);
f_ang_y  = zeros(N,1);
f_gyro_x = zeros(N,1);
f_gyro_y = zeros(N,1);
for n = 1:N
	if n == 1
		f_ang_x(n) = (acc_ang_x_f(n) + 0 - 0) / (tau2 + 1);
		f_ang_y(n) = (acc_ang_y_f(n) + 0 - 0) / (tau2 + 1);
		
		f_gyro_x(n) = (y_gx(n)*tau2 + 0 - 0) / (tau2 + 1);
		f_gyro_y(n) = (y_gy(n)*tau2 + 0 - 0) / (tau2 + 1);
	else
		f_ang_x(n) = (acc_ang_x_f(n) + acc_ang_x_f(n-1) - f_ang_x(n-1)*(1-tau2)) / (tau2 + 1);
		f_ang_y(n) = (acc_ang_y_f(n) + acc_ang_y_f(n-1) - f_ang_y(n-1)*(1-tau2)) / (tau2 + 1);
		
		f_gyro_x(n) = (y_gx(n)*tau2 + y_gx(n-1)*tau2 - f_gyro_x(n-1)*(1-tau2)) / (tau2 + 1);
		f_gyro_y(n) = (y_gy(n)*tau2 + y_gy(n-1)*tau2 - f_gyro_y(n-1)*(1-tau2)) / (tau2 + 1);
	end
end

angle_x = f_ang_x + f_gyro_x;
angle_y = f_ang_y + f_gyro_y;

% figure(8);
% plot(f_ang_x);
% xlabel('Sample number');
% ylabel('X angle (^\circ)');
% 
% figure(9);
% plot(f_ang_y);
% xlabel('Sample number');
% ylabel('Y angle (^\circ)');








