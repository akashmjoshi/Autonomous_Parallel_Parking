function sidesens1

close all


% 10cm-80cm, did not use
distance=[10:5:80];
voltage=[2.42,1.72,1.35,1.1,.94,.83,.75,.7,.64,.6,.54,.52,.48,.44,.42];

invdist=1./distance;

[line1]=polyfit(invdist,voltage,1)


figure(1)
hold on
plot(distance,voltage,'b.');

xlabel('distance')
ylabel('voltage')
hold off


figure(2)
hold on
plot(invdist,voltage,'b.');
plot(invdist,polyval(line1,invdist,'g'));

xlabel('1/distance')
ylabel('voltage')
hold off