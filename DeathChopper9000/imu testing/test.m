tau = 0.8;
s = tf('s');
Ha1 = 1 / (tau*s + 1);
Hg1 = tau / (tau*s + 1);
Ha2 = (2*tau*s+1) / (tau*s+1)^2
Hg2 = (tau^2*s) / (tau*s+1)^2

figure(1);
bode(Ha1,Hg1);

figure(2);
bode(Ha2,Hg2);