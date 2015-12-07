b1 = [1, -1.8256, 1];
b2 = [1, -1.9673, 1];

a1 = [1, -1.94551, 0.946985];
a2 = [1, -1.97569, 0.979843];

g = 0.00101447;

x = rand(1,10001);

y1 = filter(g.*b1,a1,x);
y2 = filter(b2,a2,y1);

Hf_in = fft(x);
Hf_out = fft(y2);
f = linspace(0,1,length(x));
plot(f,20*log10(abs(Hf_out./Hf_in)));
