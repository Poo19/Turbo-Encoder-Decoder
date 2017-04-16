: Matlab codes
clear all
close all
clc
 
%Conversion of the input data into continuous stream of binary bits
data = 'turbo12345';                          %Input data
ascii = double(data);                         %Conversion into decimal value
datalen = length(data)
imp = 5-mod(datalen,5);
if (mod(datalen,5) ~= 0)
    dist = datalen+imp;
else
    dist = datalen;
end
ascii = [ascii,zeros(1,imp)];
bin = zeros(dist,8);
for i = 1:dist
    a = ascii(i);
    k = 8;
    while (a>=1)
        bin(i,k) = mod(a,2);     %Conversion into binary values
        a = floor(a/2);
        k = k-1;
    end
end
cont = reshape(bin,1,[]);        %Reshape to get a single long array of binary bits
len = length(cont);
encsig = zeros(0,0);
 
%Consider 40 bits at a time and encode the entire data

for i = 0:ceil(len/40)-1
    in = zeros(1,43);
    for j = 1:40
        in(j) = cont(40*i+j);                       %Consider 40 binary bits for encoding at a time
    end
    [v0,v1] = sysenc(in);                           %First encoder
    [in2] = interleaver(in);                        %Interleaving the input for the second encoder
    [v00,v11] = sysenc(in2);                        %Second encoder
    [enc] = concat(v0,v1,v11);                      %Encoded signal for every 40 bits of input data
    encsig = [encsig,enc];                          %Final encoded signal
end
len1 = length(encsig);
 
%Modulate the encoded signal
[modsig] = modulation(encsig,len1);
 
%Noise generation by box Muller method
snr = 0.5;                                            %snr value
noise = awgn(modsig,snr,'measured');
 
%Decoder stage
len2 = length(noise);
binout = zeros(0,0);
count = zeros(len2/129,4);
for i = 0:(len2/129)-1
    L = zeros(1,129);
    for j = 1:129
        L(j) = noise(129*i+j);                      %Consider 129 bits for decoding at a time 
    end
    [ru,rp1,rp2] = deconcat(L);
    la = zeros(1,43);                               %Initial intrinsic values will be zeros
    [ru2] = interleaver(ru);                        %Interleaving the received sytematic input for the second decoder
    lc = 4*snr;
    for b = 1:4                                     %Performing required number of iterations
        [lee1] = turbotest(ru,rp1,la,snr);          %First decoder
        [la] = interleaver(lee1);                   %Interleaving the extrinsic values of first decoder for obtaining intrinsic values of second decoder
        [lee2] = turbotest(ru2,rp2,la,snr);         %Second decoder
        l = lc*ru2+la+lee2;                         %Calculating the output
        [L] = deinterleaver(l);                     %Deinterleaving the output to get the output in the desired order
        out = zeros(1,40);
        for k = 1:40                                %Simple logic to compute the binary output
            if (L(k)<0)
                out(k) = 0;
            else
                out(k) = 1;
            end
        end
        for k=1:40                                  %Calculating the number of errors
            if out(k)~=cont(40*i+k)
            count(i+1,b) = count(i+1,b)+1;
            end
        end
        if count(i+1,b) == 0                        %If no errors are present, then step out of the iteration loop
            break;
        end
        [la] = deinterleaver(lee2);                 %Deinterleaving the extrinsic values of second decoder for obtaining intrinsic values of first decoder
    end
    binout = [binout,out];                          %Storing all the output binary bits
end
 
%Conversion of the decoded binary bits back to the data
binfinal = reshape(binout,[],8);
binstr = num2str(binfinal);
asciiout = bin2dec(binstr);
dataout = char(asciiout);
reshape(dataout,1,[])
count;

concatenation function 
function [out] = concat (in1,in2,in3)
for i=0:42
    out(3.*i+1)=in1(i+1); %combining the 3 outputs from the 2 encoder stages
    out(3.*i+2)=in2(i+1);
    out(3.*i+3)=in3(i+1);
end
end

Deconcatenation function 
function [out1,out2,out3] = deconcat (in)
for i=0:42
    out1(i+1)= in(3.*i+1); %combining the 3 outputs from the 2 encoder stages
    out2(i+1)=in(3.*i+2);
    out3(i+1)=in(3.*i+3);
end
end
Interleaver function
function [I] = interleaver (u)
f1=3;
f2=10;
i=0:1:39;
x(i+1)= mod(((f1.*i)+(f2.*i.*i)),40);
for i = 0:39
I(i+1)= u(x(i+1)+1);
end
I(41:1:43)=0;
End

Max function
function[z]=maxx(x,y)
z=max(x,y)+log(1+exp(-abs(x-y)));
end

Deinterleaver function
function [L] = deinterleaver (l)
f1=3; %interleaver input frequencies
f2=10; %interleaver input frequencies
i=0:1:39;
x(i+1)= mod(((f1.*i)+(f2.*i.*i)),40);
for i = 0:39
    L(x(i+1)+1)= l(i+1); %output of deinterleaver
end
end
Modulation Function
function [out] = modulation (in,l)
for i=1:l
    if in(i)==0
        out(i)=-1;
    elseif in(i)==1
        out(i)=1;
    end
end
end

function[a,b,c,d,e]=myfunc (u,s0,s1,s2)
a=u;
b=xor(s1,xor(u,s0));
c=xor(s1,xor(u,s2));
d=s0;
e=s1;
Encoder function
function [v0,v1] = sysenc (u)
s0(1) = 0;
s1(1) = 0;
s2(1) = 0;
for  n=1:43
[v0(n),v1(n),s0(n+1),s1(n+1),s2(n+1)] = myfunc(u(n),s0(n),s1(n),s2(n));
   if n>40
       if s0(n) == 0
           if s1(n) == 0
               if s2(n) == 1
                   v0(n) = 1;   
               end
           elseif s1(n) == 1
               if s2(n) == 0
                   v0(n) = 1;
               end
               if s2(n) == 1
                  v0(n) = 0;
               end
           end
           elseif s0(n) == 1
               if s1(n) == 0
                   if s2(n) == 0
                       v0(n) = 0;
                   end
               elseif s2(n) == 1
                   v0(n) = 1;
               end
           elseif s1(n) == 1
               if s2(n) == 0
                   v0(n) = 1;
               end
           elseif s2(n) == 1
               v0(n) = 0;
       end
   end
end
end

Turbo Test function
function [leu]=turbotest(ruu,rp,lint,snr)
lc=4*snr;
alp(1,1)=0;
alp(2,1)=-1/2*lint(1)+lc/2*(-ruu(1)-rp(1));
alp(2,5)=1/2*lint(1)+lc/2*(ruu(1)+rp(1));
alp(3,1)=-1/2*lint(2)+lc/2*(-ruu(2)-rp(2))+alp(2,1);
alp(3,3)=-1/2*lint(2)+lc/2*(-ruu(2)+rp(2))+alp(2,5);
alp(3,5)=1/2*lint(2)+lc/2*(ruu(2)+rp(2))+alp(2,1);
alp(3,7)=1/2*lint(2)+lc/2*(ruu(2)-rp(2))+alp(2,5);
 
alp(4,1)=-1/2*lint(3)+lc/2*(-ruu(3)-rp(3))+alp(3,1);
alp(4,2)=1/2*lint(3)+lc/2*(ruu(3)-rp(3))+alp(3,3);
alp(4,3)=-1/2*lint(3)+lc/2*(-ruu(3)+rp(3))+alp(3,5);
alp(4,4)=1/2*lint(3)+lc/2*(ruu(3)+rp(3))+alp(3,7);
alp(4,5)=1/2*lint(3)+lc/2*(ruu(3)+rp(3))+alp(3,1);
alp(4,6)=-1/2*lint(3)+lc/2*(-ruu(3)+rp(3))+alp(3,3);
alp(4,7)=1/2*lint(3)+lc/2*(ruu(3)-rp(3))+alp(3,5);
alp(4,8)=-1/2*lint(3)+lc/2*(-ruu(3)-rp(3))+alp(3,7);
k=40;
for i=5:k
alp(i,1)=maxx(-1/2*lint(i-1)+lc/2*(-ruu(i-1)-rp(i-1))+alp(i-1,1),1/2*lint(i-1)+lc/2*(ruu(i-1)+rp(i-1))+alp(i-1,2));
alp(i,2)=maxx(1/2*lint(i-1)+lc/2*(ruu(i-1)-rp(i-1))+alp(i-1,3),-1/2*lint(i-1)+lc/2*(-ruu(i-1)+rp(i-1))+alp(i-1,4));
alp(i,3)=maxx(-1/2*lint(i-1)+lc/2*(-ruu(i-1)+rp(i-1))+alp(i-1,5),1/2*lint(i-1)+lc/2*(ruu(i-1)-rp(i-1))+alp(i-1,6));
alp(i,4)=maxx(1/2*lint(i-1)+lc/2*(ruu(i-1)+rp(i-1))+alp(i-1,7),-1/2*lint(i-1)+lc/2*(-ruu(i-1)-rp(i-1))+alp(i-1,8));
alp(i,5)=maxx(1/2*lint(i-1)+lc/2*(ruu(i-1)+rp(i-1))+alp(i-1,1),-1/2*lint(i-1)+lc/2*(-ruu(i-1)-rp(i-1))+alp(i-1,2));
alp(i,6)=maxx(-1/2*lint(i-1)+lc/2*(-ruu(i-1)+rp(i-1))+alp(i-1,3),1/2*lint(i-1)+lc/2*(ruu(i-1)-rp(i-1))+alp(i-1,4));
alp(i,7)=maxx(1/2*lint(i-1)+lc/2*(ruu(i-1)-rp(i-1))+alp(i-1,5),-1/2*lint(i-1)+lc/2*(-ruu(i-1)+rp(i-1))+alp(i-1,6));
alp(i,8)=maxx(-1/2*lint(i-1)+lc/2*(-ruu(i-1)-rp(i-1))+alp(i-1,7),1/2*lint(i-1)+lc/2*(ruu(i-1)+rp(i-1))+alp(i-1,8));
end
 alp(k+1,1)=maxx(lc/2*(-ruu(k)-rp(k))+alp(k,1),lc/2*(ruu(k)+rp(k))+alp(k,2));
alp(k+1,2)=maxx(lc/2*(ruu(k)-rp(k))+alp(k,3),lc/2*(-ruu(k)+rp(k))+alp(k,4));
alp(k+1,3)=maxx(lc/2*(-ruu(k)+rp(k))+alp(k,5),lc/2*(ruu(k)-rp(k))+alp(k,6));
alp(k+1,4)=maxx(lc/2*(ruu(k)+rp(k))+alp(k,7),lc/2*(-ruu(k)-rp(k))+alp(k,8));
 
alp(k+2,1)=maxx(lc/2*(-ruu(k+1)-rp(k+1))+alp(k+1,1),lc/2*(ruu(k+1)+rp(k+1))+alp(k+1,2));
alp(k+2,2)=maxx(lc/2*(ruu(k+1)-rp(k+1))+alp(k+1,3),lc/2*(-ruu(k+1)+rp(k+1))+alp(k+1,4));
 
alp(k+3,1)=maxx(lc/2*(-ruu(k+2)-rp(k+2))+alp(k+2,1),lc/2*(ruu(k+2)+rp(k+2))+alp(k+2,2));
 
bet(k+3,1)=0;
 
bet(k+2,1)=lc/2*(-ruu(k+2)-rp(k+2));
bet(k+2,2)=lc/2*(ruu(k+2)+rp(k+2));
 
bet(k+1,1)=lc/2*(-ruu(k+1)-rp(k+1))+bet(k+2,1);
bet(k+1,2)=lc/2*(ruu(k+1)+rp(k+1))+bet(k+2,1);
bet(k+1,3)=lc/2*(ruu(k+1)-rp(k+1))+bet(k+2,2);
bet(k+1,4)=lc/2*(-ruu(k+1)+rp(k+1))+bet(k+2,2);
 
bet(k,1)=lc/2*(-ruu(k)-rp(k))+bet(k+1,1);
bet(k,2)=lc/2*(ruu(k)+rp(k))+bet(k+1,1);
bet(k,3)=lc/2*(ruu(k)-rp(k))+bet(k+1,2);
bet(k,4)=lc/2*(-ruu(k)+rp(k))+bet(k+1,2);
bet(k,5)=lc/2*(-ruu(k)+rp(k))+bet(k+1,3);
bet(k,6)=lc/2*(ruu(k)-rp(k))+bet(k+1,3);
bet(k,7)=lc/2*(ruu(k)+rp(k))+bet(k+1,4);
bet(k,8)=lc/2*(-ruu(k)-rp(k))+bet(k+1,4);
 
for i=k:-1:4
bet(i,1)=maxx(-1/2*lint(i)+lc/2*(-ruu(i)-rp(i))+bet(i+1,1),1/2*lint(i)+lc/2*(ruu(i)+rp(i))+bet(i+1,5));
bet(i,2)=maxx(-1/2*lint(i)+lc/2*(-ruu(i)-rp(i))+bet(i+1,5),1/2*lint(i)+lc/2*(ruu(i)+rp(i))+bet(i+1,1));
bet(i,3)=maxx(-1/2*lint(i)+lc/2*(-ruu(i)+rp(i))+bet(i+1,6),1/2*lint(i)+lc/2*(ruu(i)-rp(i))+bet(i+1,2));
bet(i,4)=maxx(-1/2*lint(i)+lc/2*(-ruu(i)+rp(i))+bet(i+1,2),1/2*lint(i)+lc/2*(ruu(i)-rp(i))+bet(i+1,6));
bet(i,5)=maxx(-1/2*lint(i)+lc/2*(-ruu(i)+rp(i))+bet(i+1,3),1/2*lint(i)+lc/2*(ruu(i)-rp(i))+bet(i+1,7));
bet(i,6)=maxx(-1/2*lint(i)+lc/2*(-ruu(i)+rp(i))+bet(i+1,7),1/2*lint(i)+lc/2*(ruu(i)-rp(i))+bet(i+1,3));
bet(i,7)=maxx(-1/2*lint(i)+lc/2*(-ruu(i)-rp(i))+bet(i+1,8),1/2*lint(i)+lc/2*(ruu(i)+rp(i))+bet(i+1,4));
bet(i,8)=maxx(-1/2*lint(i)+lc/2*(-ruu(i)-rp(i))+bet(i+1,4),1/2*lint(i)+lc/2*(ruu(i)+rp(i))+bet(i+1,8));
end
 
bet(3,1)=maxx(-1/2*lint(3)+lc/2*(-ruu(3)-rp(3))+bet(4,1),1/2*lint(3)+lc/2*(ruu(3)+rp(3))+bet(4,5));
bet(3,3)=maxx(-1/2*lint(3)+lc/2*(-ruu(3)+rp(3))+bet(4,6),1/2*lint(3)+lc/2*(ruu(3)-rp(3))+bet(4,2));
bet(3,5)=maxx(-1/2*lint(3)+lc/2*(-ruu(3)+rp(3))+bet(4,3),1/2*lint(3)+lc/2*(ruu(3)-rp(3))+bet(4,7));
bet(3,7)=maxx(-1/2*lint(3)+lc/2*(-ruu(3)-rp(3))+bet(4,8),1/2*lint(3)+lc/2*(ruu(3)+rp(3))+bet(4,4));
 
bet(2,1)=maxx(-1/2*lint(2)+lc/2*(-ruu(2)-rp(2))+bet(3,1),1/2*lint(2)+lc/2*(ruu(2)+rp(2))+bet(3,5));
bet(2,5)=maxx(-1/2*lint(2)+lc/2*(-ruu(2)+rp(2))+bet(3,3),1/2*lint(2)+lc/2*(ruu(2)-rp(2))+bet(3,7));
 
leu(1)=rp(1)+bet(2,5)-bet(2,1);
leu(2)=maxx(alp(2,1)+bet(3,5)+lc/2*rp(2),alp(2,5)+bet(3,7)-lc/2*rp(2))-maxx(alp(2,1)+bet(3,1)-lc/2*rp(2),alp(2,5)+bet(3,3)+lc/2*rp(2));
leu(3)=maxx(maxx(alp(3,1)+bet(4,5)+lc/2*rp(3),alp(3,3)+bet(4,2)-lc/2*rp(3)),maxx(alp(3,5)+bet(4,7)-lc/2*rp(3),alp(3,7)+bet(4,4)+lc/2*rp(3)))-maxx(maxx(alp(3,1)+bet(4,1)-lc/2*rp(3),alp(3,3)+bet(4,6)+lc/2*rp(3)),maxx(alp(3,5)+bet(4,3)+lc/2*rp(3),alp(3,7)+bet(4,8)-lc/2*rp(3)));
for i=4:k
    p1=alp(i,1)+bet(i+1,5)+lc/2*rp(i);
    p2=alp(i,2)+bet(i+1,1)+lc/2*rp(i);
    q1=alp(i,3)+bet(i+1,2)-lc/2*rp(i);
    q2=alp(i,4)+bet(i+1,6)-lc/2*rp(i);
    r1=alp(i,5)+bet(i+1,7)-lc/2*rp(i);
    r2=alp(i,6)+bet(i+1,3)-lc/2*rp(i);
    s1=alp(i,7)+bet(i+1,4)+lc/2*rp(i);
    s2=alp(i,8)+bet(i+1,8)+lc/2*rp(i);
    t1=alp(i,1)+bet(i+1,1)-lc/2*rp(i);
    t2=alp(i,2)+bet(i+1,5)-lc/2*rp(i);
    u1=alp(i,3)+bet(i+1,6)+lc/2*rp(i);
    u2=alp(i,4)+bet(i+1,2)+lc/2*rp(i);
    v1=alp(i,5)+bet(i+1,3)+lc/2*rp(i);
    v2=alp(i,6)+bet(i+1,7)+lc/2*rp(i);
    w1=alp(i,7)+bet(i+1,8)-lc/2*rp(i);
    w2=alp(i,8)+bet(i+1,4)-lc/2*rp(i);
leu(i)=maxx((maxx(maxx(p1,p2),maxx(q1,q2))),(maxx(maxx(r1,r2),maxx(s1,s2))))-maxx((maxx(maxx(t1,t2),maxx(u1,u2))),(maxx(maxx(v1,v2),maxx(w1,w2))));
end
leu(k+1)=maxx(maxx(alp(k+1,2)+bet(k+2,1)+lc/2*rp(k+1),alp(k+1,3)+bet(k+2,2)-lc/2*rp(k+1)),maxx(alp(k+1,6)+bet(k+2,3)-lc/2*rp(k+1),alp(k+1,7)+bet(k+2,4)+lc/2*rp(k+1)))-maxx(maxx(alp(k+1,1)+bet(k+2,1)-lc/2*rp(k+1),alp(k+1,4)+bet(k+2,2)+lc/2*rp(k+1)),maxx(alp(k+1,5)+bet(k+2,3)+lc/2*rp(k+1),alp(k+1,8)+bet(k+2,4)-lc/2*rp(k+1)));
leu(k+2)=maxx(alp(k+2,2)+bet(k+3,1)+lc/2*rp(k+2),alp(k+2,3)+bet(k+3,2)-lc/2*rp(k+2))-maxx(alp(k+2,1)+bet(k+3,1)-lc/2*rp(k+2),alp(k+2,4)+bet(k+3,2)+lc/2*rp(k+2));
leu(k+3)=rp(k+3)+alp(k+3,2)-alp(k+3,1);
end

