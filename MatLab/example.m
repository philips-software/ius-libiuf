clear all;
close all;
clc;

numChannels = 2;
numSamplesPerLine = 3;
numPulses = 4;

matlabFrame = zeros(numChannels, numSamplesPerLine, numPulses);

matlabFrame(:,:,1) = [1 3 5; 2 4 6];
matlabFrame(:,:,2) = matlabFrame(:,:,1) + 10;
matlabFrame(:,:,3) = matlabFrame(:,:,1) + 100;
matlabFrame(:,:,4) = matlabFrame(:,:,1) + 1000;

% Het platslaan:
tic 
rfData = zeros(1, numChannels * numSamplesPerLine * numPulses);
i = 0;
for c1 = 1:numPulses
    for c2 = 1:numSamplesPerLine
        for c3 = 1:numChannels
            i = i+1;
            rfData(i) = matlabFrame(c3, c2, c1);
        end
    end
end
toc

clear all;
close all;

numChannels = 2;
numSamplesPerLine = 3;
numPulses = 4;

matlabFrame = zeros(numChannels, numSamplesPerLine, numPulses);

matlabFrame(:,:,1) = [1 3 5; 2 4 6];
matlabFrame(:,:,2) = matlabFrame(:,:,1) + 10;
matlabFrame(:,:,3) = matlabFrame(:,:,1) + 100;
matlabFrame(:,:,4) = matlabFrame(:,:,1) + 1000;

tic
rfDataFast = reshape(matlabFrame, 1, numChannels * numSamplesPerLine * numPulses );
toc

%rfData
%rfData - rfDataFast


