clear; format short
axesSize = 18;
tickSize = 12;
titleSize = 18;
markerSize = 8;
legSize = 16;
lw = 1;

%% WITH LIPO CHARGER, NO VOLTAGE DIVIDER, POWERED WITH 3.3 
curr_pos = [0 1 2 3 4 5 5.9]; % in A
signal_pos = [1.649 1.655 1.662 1.669 1.675 1.683 1.688]; % in V
curr_neg = [-1 -2 -3 -4 -5]; % in A
signal_neg = [1.629 1.626 1.618 1.610 1.604]; % in V
curr = [curr_pos curr_neg]; % in A
signal = [signal_pos signal_neg]; % in V

clf; figure(1)
plot(curr,signal,'bo','MarkerFaceColor','blue')
set(gca,'FontSize',tickSize)
grid on
xlabel('Current (A)','FontSize',axesSize)
ylabel('Signal (V)','FontSize',axesSize)
title('Signal vs. Current','FontSize',titleSize)
hold on

% linear fit
P = polyfit(curr,signal,1);
modelCurr = -10:.5:10;
modelSignal = polyval(P,modelCurr);

% slope is the EFFECTIVE resistance of the current sensor
R = P(1)  
% the actual resistor should be 100 um

plot(modelCurr,modelSignal,'k-','Linewidth',lw);

% linear fit for postive and negative current separately
P_pos = polyfit(curr_pos, signal_pos,1);
R_pos = P_pos(1)
modelCurr_pos = 0:.5:10;
modelSignal_pos = polyval(P_pos,modelCurr_pos);

P_neg = polyfit(curr_neg, signal_neg, 1);
R_neg = P_neg(1)
modelCurr_neg = -10:.5:0;
modelSignal_neg = polyval(P_neg,modelCurr_neg);

plot(modelCurr_pos, modelSignal_pos, 'r-','Linewidth',lw)
plot(modelCurr_neg, modelSignal_neg, 'b-','Linewidth',lw)

totalFitLeg = sprintf('total fit, R_{eff} = %.4g',R);
posFitLeg = sprintf('positive fit, R_{eff} = %.4g',R_pos);
negFitLeg = sprintf('negative fit, R_{eff} = %g',R_neg);

leg = legend('data points',totalFitLeg, posFitLeg, negFitLeg, 'Location','northwest');
set(leg,'FontSize',legSize);

hold off;
print('currSensorPlot1','-dpng')


%% WITH MOTOR, NO VOLTAGE DIVIDER
% base voltage 1.62 --> half the input power?
% highest reading was 1.665

%% (12/2/16) WITH LIPO CHARGER, NO VOLTAGE DIVIDER, POWERED BY 3.3 V -- 
pwrVoltage = 3.27; % V
curr = [-4 -3 -2 -1 0 1 2 3 4 5]; % in A
signal = [1.588 1.577 1.582 1.592 1.6 1.616 1.616 1.622 1.629 1.636]; % V


% calculate percent error based on my model of voltage division
expectedZeroSignal = pwrVoltage./2
zeroIndex = find(curr==0);
percentError = ((signal(zeroIndex) - expectedZeroSignal)./(signal(zeroIndex))).*100


% fit line
P = polyfit(curr,signal,1);
modelSignal = polyval(P,modelCurr);
figure
plot(curr,signal,'bo','MarkerFaceColor','blue')
hold on
plot(modelCurr,modelSignal,'r--','Linewidth',lw);
set(gca,'FontSize',tickSize)
grid on
xlabel('Current (A)','FontSize',axesSize)
ylabel('Signal (V)','FontSize',axesSize)
title('Signal vs. Current','FontSize',titleSize)
hold on

R = P(1);
zeroSignal = P(2);

totalFitLeg = sprintf('total fit, R_{eff} = %.4g',R);

leg = legend('data points',totalFitLeg, 'Location','northwest');
set(leg,'FontSize',legSize);

hold off;
print('currSensorPlot2','-dpng')


% compare hypothesis with linear fit
expectedRange = (expectedZeroSignal - zeroSignal)./R % expected range from fit
hypotheticalRange =  pwrVoltage.*(polyval(P,1)./1) % expected range from hyptohesis
