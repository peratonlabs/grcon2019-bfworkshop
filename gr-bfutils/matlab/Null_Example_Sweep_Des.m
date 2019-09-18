% Copyright (c) 2019 Perspecta Labs Inc.  All Rights Reserved

function nullExample
    % Null steering example
    a.Nr=8;

    % Define uniform linear array on x-axis
    inds=(0:a.Nr-1).';

    a.fc=2e9;
    a.c=2.998e8;
    a.lambda=a.c/a.fc;
    a.dx=a.lambda/2;
    a.dy=0;
    a.x=inds.*a.dx;
    a.y=inds.*a.dy;

    % Define DOAs
    %fixed interfering signal at 45 degrees
    phii=45*pi/180;
    
    % set of steering vectors for all the scan angles
    ai=steeringVector(phii,a);
    angScan=(0:0.5:180);
    A=steeringVector(angScan*pi/180,a);
    
    w=ones(a.Nr,1)/sqrt(a.Nr);
    pat=10*log10(abs(A'*w).^2);
    figure(1); clf; hold on;
    hnd=plot(angScan,pat);
    set(gca,'box','on','fontsize',14);
    miny=-20;
    maxy=10;
    hndi=plot(phii*180/pi*[1,1],[miny,maxy]);
    set(hndi,'linewidth',4,'color','r');
    phid=0;
    hndd=plot(phid*180/pi*[1,1],[miny,maxy]);
    set(hndd,'linewidth',4,'color','g');
    grid on;
    axis([0,180,-20,10]);
    set(hnd,'linewidth',2);
    hndTi=text(phii*180/pi,maxy,'INT');
    hndTd=text(phid*180/pi,maxy,'DES');
    [~,iDes]=min(abs(angScan-phid*180/pi));
    hndMd=plot(phid*180/pi,pat(iDes),'o');
    
    angds=0:180;
    % angds=60;
    for nd=1:length(angds)
        phid=angds(nd)*pi/180;
        ad=steeringVector(phid,a);
        [U,D]=eig(ai*ai');
        d=diag(D);
        [ds,sortInds]=sort(d,'descend');
        U=U(:,sortInds);
        Un=U(:,2:end);
        wn=inv(Un'*Un+1e-6*eye(a.Nr-1,a.Nr-1))*Un'*ad;
        w=Un*wn;
        w=w./norm(w);
        pat=10*log10(abs(A'*w).^2);
        set(hnd,'ydata',pat);
        set(hndd,'xdata',phid*180/pi*[1,1]);
        set(hndTd,'position',[phid*180/pi,maxy]);
        [~,iDes]=min(abs(angScan-phid*180/pi));
        set(hndMd,'xdata',phid*180/pi,'ydata',pat(iDes));
        pause(0.2);
    end
       
    keyboard;
    
end

function a=steeringVector(phi,a)
    a=exp(j*2*pi/a.lambda*(a.x*cos(phi)+a.y*sin(phi)));
end