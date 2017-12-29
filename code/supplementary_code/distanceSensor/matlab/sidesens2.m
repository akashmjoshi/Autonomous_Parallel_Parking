function sidesens2

close all

% % 10cm-80cm
% distance=[5:5:85]
% voltage=[3.08,2.28,1.64,1.28,1.08,.904,.816,.728,.625,.596,.56,.52,.496,.
% 444,.396,.382,.374]
%.808,.728,.672,.612,.556,.516,.488,.452,.432,.42]
% .52-60cm

% 10cm-80cm, used on right side of car
distance=[10:5:80];
voltage=[2.28,1.64,1.28,1.08,.904,.808,.728,.672,.612,.556,.516,.488,.452,.432,.42];

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