s = serial('COM9','baudrate',57600,'databits',8,'stopbits',1,'parity','even','flowcontrol','none');
fopen(s);

for k = 1:1
	str = sprintf('Hello world %d\r', mod(k,10));
	fprintf(s,str);
	pause(0.5);
end

fclose(s);
delete(s);
clear s;