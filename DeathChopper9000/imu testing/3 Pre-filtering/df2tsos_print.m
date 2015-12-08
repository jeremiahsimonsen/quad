sos = ans.sosMatrix;
for row = 1:length(ans.sosMatrix(:,1))
   fprintf('%.12g, %.12g, %.12g,\n', sos(row,1), sos(row,2), sos(row,3));
   fprintf('%.12g, %.12g,\n', sos(row,5), sos(row,6));
end
g = prod(ans.ScaleValues);
fprintf('g = %.12g;\n', g);