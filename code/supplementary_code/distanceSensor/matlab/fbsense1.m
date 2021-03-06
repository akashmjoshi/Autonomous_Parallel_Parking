function fbsens1

close all

% 4cm-30cm, used on front of car
distance=[4:1:6,8:2:12,15:3:30];
voltage=[2.6,2.22,1.92,1.52,1.26,1.08,.88,.76,.656,.576,.52,.48];

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