clear all;
close all;

fileNames = dir('*AODR.txt');

data = [];
for k = 1:length(fileNames)
	fd = fopen(fileNames(k).name);
	data_k = [];
	tline = fgetl(fd);
	while ischar(tline);
		data_k = [data_k; sscanf(tline, '%f %f %f %f %f %f %f %f %f')'];
		tline = fgetl(fd);
	end
	fclose(fd);
	
	if k > 1 && length(data_k(:,1)) > length(data(:,1,1))
		for j = 1:k-1
			data(:,:,j) = [data(:,:,j); zeros(length(data_k(:,1))-length(data(:,1,j)),9)];
		end
	elseif k > 1 && length(data_k(:,1)) < length(data(:,1,1))
		data_k = [data_k; zeros(length(data(:,1,1))-length(data_k(:,1)),9)];
	end
	
	data = cat(3, data, data_k);
end
ax = []; ay = []; az = [];
gx = []; gy = []; gz = [];
mx = []; my = []; mz = [];
for k = 1:length(fileNames)
	ax = [ax, data(:,1,k)];
	ay = [ay, data(:,2,k)];
	az = [az, data(:,3,k)];
	gx = [gx, data(:,4,k)];
	gy = [gy, data(:,5,k)];
	gz = [gz, data(:,6,k)];
	mx = [mx, data(:,7,k)];
	my = [my, data(:,8,k)];
	mz = [mz, data(:,9,k)];
end

clearvars -except ax ay az gx gy gz mx my mz fileNames;

DT = 20e-3;
t = [1:length(ax)] * DT;

for k = 1:length(fileNames)
% 	figure(3*(k-1)+1);
	figure(1);
	subplot(2,length(fileNames)/2,k);
	plot(t,ax(:,k),t,ay(:,k),t,az(:,k));
	xlabel('Time (s)');
	ylabel('Accelerometer output (g)');
	title(strcat(fileNames(k).name, 'Accelerometer data'));
	grid on;
	
% 	figure(3*(k-1)+2);
% 	plot(t,gx(:,k),t,gy(:,k),t,gz(:,k));
% 	xlabel('Time (s)');
% 	ylabel('Gyroscope output (^\circ/s)');
% 	title(strcat(fileNames(k).name, 'Gyroscope data'));
% 	grid on;
% 	
	figure (3);
	subplot(2,length(fileNames)/2,k);
	plot(t,mx(:,k),t,my(:,k),t,mz(:,k));
	xlabel('Time (s)');
	ylabel('Magnetometer output (gauss)');
	title(strcat(fileNames(k).name, 'Magnetometer data'));
	grid on;
end









