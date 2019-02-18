clear all;
close all;
clc;

cd('../src');
mxCreate
cd('../m')

fnlist = {...
    'create', 1;...
    'frameCreate', 1;...
    'responseCreate', 2;...
    'channelCreate', 2;...
    'delete', 1;...
    'nodeLoad', 1;...
    'nodeSave', 1;...
    'close', 1;...
    'compare', 2;...
    'getFrameList', 1;...
    'getPatternListDict', 1;...
    'getPulseDict', 1;...
    'getSourceDict', 1;...
    'getReceiveChannelMapDict', 1;...
    'getTransmitApodizationDict', 1;...
    'getReceiveSettingsDict', 1;...
    'getAcquisition', 1;...
    'getTransducer', 1;...
    'getNumFrames', 1;...
    'getNumResponses', 2;...
    'getNumChannels', 2;...
    'getSamplesPerLine', 2;...
    'setFrameList', 2;...
    'setPatternListDict', 2;...
    'setPulseDict', 2;...
    'setSourceDict', 2;...
    'setReceiveChannelMapDict', 2;...
    'setTransmitApodizationDict', 2;...
    'setReceiveSettingsDict', 2;...
    'setAcquisition', 2;...
    'setTransducer', 2;...
    'channelSave', 4;...
    'channelLoad', 4;...
    'responseSave', 4;...
    'responseLoad', 4;...
    'frameSave', 4;...
    'frameLoad', 4 ...
    };

for c1 = 1:size(fnlist,1)
    fn = fnlist{c1,1};
    nargs = fnlist{c1,2};
    fprintf(1, 'Testing method: %s\n', fn);
    switch fn
        case 'create'
            hdel = mxInputFile( fn, 'myTestInputFileDel.input' );
            hclose = mxInputFile( fn, 'myTestInputFileClose.input' );
        case 'delete'
            mxInputFile( fn, hdel );
        case 'close'
            mxInputFile( fn, hclose );
            mxInputFile( 'delete', hclose );
        case 'nodeLoad'
            mxInputFile( fn, 'exampleV3file.input' );
        case 'compare'
            filename = 'exampleV3file.input';
            reference = mxInputFile('nodeLoad', filename);   
            actual = mxInputFile('nodeLoad', filename);
            mxInputFile(fn, reference, actual)
            isEqual = mxInputFile('compare', reference, actual)            
        case 'getTransmitApodizationDict'
            filename = 'exampleV3file.input';
            h = mxInputFile('nodeLoad', filename);
            txApodDict = mxInputFile('getTransmitApodizationDict', h)
        
        otherwise
            switch nargs
                case 1
                    mxInputFile( fn, 2 );
                case 2
                    mxInputFile( fn, 3, 4 );
                case 4
                    mxInputFile( fn, 5, 6, 7, 8 );
                otherwise
                    error('Number of arguments not supported');
            end
    end
end

clear all;

filename = 'exampleV3file.input';
h = mxInputFile('nodeLoad', filename);
txApodDict = mxInputFile('getTransmitApodizationDict', h)

% clear all
% cd('../src');
% mxCreate
% cd('../m');
% h = mxInputFile('create', 'myInputFile.input')
% %h2 = mxInputFile('create', 'myInputFile2.input')
% h3 = mxInputFile('nodeLoad', 'exampleV3file.input');
%
% mxInputFile('close', h);
% mxInputFile('delete', h);
%
% mxInputFile('close', h3);
% mxInputFile('delete', h3);
%
%
%
% % h2 = mxInputFile('create', 'myInputFile2.input')
% % h3 = mxInputFile('create', 'myInputFile3.input')
%
% %mxInputFile('close', h)
% %mxInputFile('close', h2);
% % mxInputFile('close', h3);
%
% % clear all
%
