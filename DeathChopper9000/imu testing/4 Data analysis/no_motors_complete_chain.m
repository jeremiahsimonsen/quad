%% Raw data
ax = [];
ay = [];
az = [];
gx = [];
gy = [];
gz = [];

fileName = '1 No motor - 1 raw accel and gyro.txt';
fd = fopen(fileName);
tline = fgetl(fd);
while ischar(tline);
	if ~isempty(strfind(tline, 'Ax'))
		ax = [ax sscanf(tline, 'Ax %f')];
	elseif ~isempty(strfind(tline, 'Ay'))
		ay = [ay sscanf(tline, 'Ay %f')];
	elseif ~isempty(strfind(tline, 'Az'))
		az = [az sscanf(tline, 'Az %f')];
	elseif ~isempty(strfind(tline, 'Gx'))
		gx = [gx sscanf(tline, 'Gx %f')];
	elseif ~isempty(strfind(tline, 'Gy'))
		gy = [gy sscanf(tline, 'Gy %f')];
	elseif ~isempty(strfind(tline, 'Gz'))
		gz = [gz sscanf(tline, 'Gz %f')];
	end
	tline = fgetl(fd);
end
fclose(fd);

DT = 20e-3;
t = [1:length(ax)] * DT;
figure(1);
plot(t,ax,t,ay,t,az);

%% Prefiltered data
ax_f = [];
ay_f = [];
az_f = [];
gx_f = [];
gy_f = [];

fileName = '1 No motor - 2 prefiltered.txt';
fd = fopen(fileName);
tline = fgetl(fd);
while ischar(tline);
	if ~isempty(strfind(tline, 'Axf'))
		ax_f = [ax_f sscanf(tline, 'Axf %f')];
	elseif ~isempty(strfind(tline, 'Ayf'))
		ay_f = [ay_f sscanf(tline, 'Ayf %f')];
	elseif ~isempty(strfind(tline, 'Azf'))
		az_f = [az_f sscanf(tline, 'Azf %f')];
	elseif ~isempty(strfind(tline, 'Gxf'))
		gx_f = [gx_f sscanf(tline, 'Gxf %f')];
	elseif ~isempty(strfind(tline, 'Gyf'))
		gy_f = [gy_f sscanf(tline, 'Gyf %f')];
% 	elseif ~isempty(strfind(tline, 'Gz'))
% 		gz_f = [gz_f; sscanf(tline, 'Gz %f')];
	end
	tline = fgetl(fd);
end
fclose(fd);

DT = 20e-3;
t = [1:length(ax_f(2:end))] * DT;
figure(2);
% plot(t,ax_f(2:end),t,ay_f(2:end),t,az_f);

%% Accelerometer based angles
angle_x = [];
angle_y = [];

fileName = '1 No motor - 3 accel angles.txt';
fd = fopen(fileName);
tline = fgetl(fd);
while ischar(tline);
	if ~isempty(strfind(tline, 'P'))
		angle_x = [angle_x sscanf(tline, 'P %f')];
	elseif ~isempty(strfind(tline, 'R'))
		angle_y = [angle_y sscanf(tline, 'R %f')];
	end
	tline = fgetl(fd);
end
fclose(fd);

%% Complementary filtered values
angle_x_f = [];
angle_y_f = [];
gyro_x_f = [];
gyro_y_f = [];

fileName = '1 No motor - 4 complementary filtered.txt';
fd = fopen(fileName);
tline = fgetl(fd);
while ischar(tline);
	if ~isempty(strfind(tline, 'Pf'))
		angle_x_f = [angle_x_f sscanf(tline, 'Pf %f')];
	elseif ~isempty(strfind(tline, 'Rf'))
		angle_y_f = [angle_y_f sscanf(tline, 'Rf %f')];
	elseif ~isempty(strfind(tline, 'Pdf'))
		gyro_x_f = [gyro_x_f sscanf(tline, 'Pdf %f')];
	elseif ~isempty(strfind(tline, 'Rdf'))
		gyro_y_f = [gyro_y_f sscanf(tline, 'Rdf %f')];
	end
	tline = fgetl(fd);
end
fclose(fd);

%% Outputs
pitch = [];
roll = [];

fileName = '1 No motor - 5 outputs.txt';
fd = fopen(fileName);
tline = fgetl(fd);
while ischar(tline);
	if ~isempty(strfind(tline, 'PT'))
		pitch = [pitch sscanf(tline, 'PT %f')];
	elseif ~isempty(strfind(tline, 'RT'))
		roll = [roll sscanf(tline, 'RT %f')];
	end
	tline = fgetl(fd);
end
fclose(fd);



