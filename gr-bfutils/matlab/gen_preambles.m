% Copyright (c) 2019 Perspecta Labs Inc. All rights reserved

% Generate gold sequences as preambles for testing

numbytes = 16;
bits_per_symbol = 2;

goldseq = comm.GoldSequence('FirstPolynomial',[11 2 0],...
    'SecondPolynomial',[11 8 5 2 0],...
    'FirstInitialConditions',[0 0 0 0 0 0 0 0 0 0 1],...
    'SecondInitialConditions',[0 0 0 0 0 0 0 0 0 0 1],...
    'Index',0,'SamplesPerFrame',8*bits_per_symbol*numbytes);
x1 = goldseq();
x2 = goldseq();

p1 = dec2hex(bin2dec(reshape(num2str(x1),[],8)))
p2 = dec2hex(bin2dec(reshape(num2str(x2),[],8)))

fprintf('[');
for i = 1:size(p1,1)
    fprintf('0x%s,',p1(i,1:2))
end
fprintf(']\n');
fprintf('[');
for i = 1:size(p2,1)
    fprintf('0x%s,',p2(i,1:2))
end
fprintf(']\n')