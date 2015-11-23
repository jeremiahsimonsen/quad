clear all;
close all;

file = '8 roll pitch complementary test.txt';

data = [];
fd = fopen(file);
tline = fgetl(fd);
while ischar(tline);
	data = [data; sscanf(tline, '%f %f')'];
	tline = fgetl(fd);
end
fclose(fd);

roll = data(:,1);
pitch = data(:,2);

DT = 20e-3;
t = [1:length(roll)] * DT;

figure(1);
plot(t,roll,t,pitch);
xlabel('Time (s)');
ylabel('Angle (^\circ)');
title('Roll and Pitch - calculated using complementary filter');