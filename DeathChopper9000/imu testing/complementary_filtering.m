s = tf('s');
tau = 0.95;

Ha = 1 / (tau*s + 1);
Hg = tau*s / (tau*s + 1);

bode(Ha,Hg);