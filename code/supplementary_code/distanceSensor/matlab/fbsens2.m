function fbsens2

close all

% 4cm-30cm, used on rear of car
distance=[4:1:6,8:2:12,15:3:30];
voltage=[2.78,2.36,2.06,1.6,1.32,1.12,.92,.776,.664,.567,.536,.476];

invdist=1./distance;

[line1]=polyfit(invdist(5:end),voltage(5:end),1)
[line2]=polyfit(invdist(1:5),voltage(1:5),1)


figure(1)
hold on
plot(distance,voltage,'b.');

xlabel('distance')
ylabel('voltage')
hold off


figure(2)
hold on
plot(invdist,voltage,'b.');
plot(invdist(5:end),polyval(line1,invdist(5:end)),'g');
plot(invdist(1:5),polyval(line2,invdist(1:5)),'r');

xlabel('1/distance')
ylabel('voltage')
hold off