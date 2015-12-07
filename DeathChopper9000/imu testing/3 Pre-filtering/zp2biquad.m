function zp2biquad(z,p,k,filename)
%ZP2BIQUAD Combines an arbitrary collection of poles, zeros, and a gain
%	constant into second-order sections. The filter coefficients are
%	formatted to work with the C biquad filter implementation developed in
%	Lab 2 and are written to "biquad_coef.c". The coefficients are declared
%	as extern variables in the "biquad_coef.h" file. This allows the task 4
%	test program ("ece486_biquad_task4.c") to use these coefficients
%	directly, without having to copy and paste the output of this function
%	into the code. Thus, to implement a new filter, this function just has
%	to be run and then the source re-compiled.
%
% Note that this function assumes that the "biquad_coef.h" file already
% exists
%
% Authors: Jacob Allenwood, Travis Russell, Jeremiah Simonsen
% ECE 486 - Laboratory 3
% March 12, 2015

% Generate the filter coefficients using the MATLAB zp2sos() function
[sos, g] = zp2sos(z,p,k);	% Returns an Lx6 array containing the filter
							% coefficients. L=#of sections
b = sos(:,1:3);				% Slice "b" coefficients; numerator
a = sos(:,4:6);				% Slice "a" coefficients; denominator

% Open the source file that will contain the filter coefficients
fd = fopen(filename,'w');

% Define Doxygen header
header = {'/*!';
		  ' * @file';
		  ' * ';
		  ' * @brief ECE 486 Spring 2015 Lab 4 - Source file containing ';
		  ' * definitions of filter coefficients for external use.';
		  ' * ';
		  ' * @author ECE486 Lab Group 2';
		  ' * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen';
		  ' *';
		  ' * @date Mar 22, 2015';
		  ' *';
		  ' * This file contains the definitions of filter coefficients';
		  ' * for external use. The coefficients are the output of the ';
		  ' * zp2biquad() function. This implementation allows for the';
		  ' * testing of new filters simply by re-running the zp2biquad()';
		  ' * function and then re-compiling.';
		  ' *';
		  ' */'};
% Print Doxygen header to the file
for row=1:size(header,1)
	fprintf(fd,'%s\n',header{row,:});	% print each line
end

% Now add the necessary include and start of the b_coef 2d array to the
% output string
fprintf(fd,'\n#include "%s"\n\nfloat %s_b_coef[%d][3] = {', ...
	strcat(filename(1:end-1),'h'), ...
	filename(1:7), ...
	length(b(:,1)));
s = '';
% Add all the b coefficients to our output string
for n=1:length(b(:,1))
	% If this is not the last section / set of coefficients
	if n ~= length(b(:,1))
		s = strcat(s, sprintf('{%.12g, %.12g, %.12g},\n',b(n,1),b(n,2),b(n,3)));
	% If it is the last one
	else
		s = strcat(s, sprintf('{%.12g, %.12g, %.12g}};\n',b(n,1),b(n,2),b(n,3)));
	end
end
% Add start of the a_coef 2d array to output string
s = strcat(s, sprintf('\nfloat %s_a_coef[%d][3] = {', ...
	filename(1:7), ...
	length(a(:,1))));
% Add all the a coefficients to output string
for n=1:length(a(:,1))
	% If this is not the last section
	if n ~= length(a(:,1))
		s = strcat(s, sprintf('{%.12g, %.12g, %.12g},\n',a(n,1),a(n,2),a(n,3)));
	% If it is the last one
	else
		s = strcat(s, sprintf('{%.12g, %.12g, %.12g}};\n',a(n,1),a(n,2),a(n,3)));
	end
end
% Add the gain factor, g, to the output string
s = strcat(s, sprintf('\nfloat %s_g = %.12g;\n',filename(1:7),g));

% Add the number of biquad stages
s = strcat(s, sprintf('\nint %s_num_stages = %d;\n',filename(1:7),length(b(:,1))));

fprintf(fd,'%s',s);		% Print the string to the file
fclose(fd);				% Close the file