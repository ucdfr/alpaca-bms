clear all;
close all;
T=[29,36,41,48];
T=273+T;
R=[8.82,6.376,5.297,3.92];
R=1000.*R;
p = polyfit(R,T,2);         %from T to R
r1 = linspace(1,30000,99);
y1 = polyval(p,r1);
plot(R,T,'o');
hold on;
%plot(r1,y1,'-');

T0=298.15;
B=3950;
R0=10000;

Rlin=linspace(1,30000,99);
y2 = 1./((1/T0)+(1/B).*log(Rlin./R0));

plot(Rlin,y2);