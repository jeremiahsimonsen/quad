z = tf('z',20e-3);

tau = 1;
s = tf('s');
Hs = 1 / (tau*s + 1);

Ts = 20e-3;
Hz1 = tau*Ts / (1-tau*z^-1);
Hz2 = (1-tau) / (1-tau*z^-1);

figure(1);
bode(Hs);

figure(2);
bode(Hz1,Hz2);