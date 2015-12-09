function df2tsos_print(Hd)
sos = Hd.sosMatrix;
fprintf('\tstatic float32_t coef[%d] = {\n', length(sos(:,1))*5);
for row = 1:length(sos(:,1))
	fprintf('\t\t%.12g, %.12g, %.12g,\n', sos(row,1), sos(row,2), sos(row,3));
	if row ~= length(sos(:,1))
		fprintf('\t\t%.12g, %.12g,\n', -sos(row,5), -sos(row,6));
	else
		fprintf('\t\t%.12g, %.12g\n', -sos(row,5), -sos(row,6));
	end
end
fprintf('\t};\n');
g = prod(Hd.ScaleValues);
fprintf('\tg = %.12g;\n', g);
fprintf('\tint num_sections = %d;\n', length(sos(:,1)));