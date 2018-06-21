clear; format short
axesSize = 18;
tickSize = 12;
titleSize = 18;
markerSize = 8;
legSize = 16;
lw = 1;

%% data
curr = [0 -2 -2.5 -3 -4 -5 -5.5 .5 1 2 3];
signal = [0.01 16 33 42 48 55 58 -.4 -0.338 -.2 -.28];

clf; figure(1)
plot(curr,signal,'bo','MarkerFaceColor','blue')
set(gca,'FontSize',tickSize)
grid on
xlabel('Current (A)','FontSize',axesSize)
ylabel('Signal (mV)','FontSize',axesSize)
title('Signal vs. Current','FontSize',titleSize)
hold on

% linear fit
P = polyfit(curr,signal,1);
modelCurr = -10:.5:10;
modelSignal = polyval(P,modelCurr);

% slope is the resistance of the current sensor
R = P(1)

plot(modelCurr,modelSignal,'r--');

hold off;

