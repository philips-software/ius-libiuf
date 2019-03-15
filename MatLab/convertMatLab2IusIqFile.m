function convertMatlab2IusIqFile( iqFilename, iusIqStruct, iqData, Receive )
% CONVERTMATLAB2IUSIQFILE writes meta data and iq data to ius iq file.
%    CONVERTMATLAB2IUSIQFILE( iqFilename, iusIqstruct, iqData ) converts
%    meta-data in the iusIqStruct to header information in the iusIq file
%    and writes the data iqData to the iusIq file with version format 3.

% Initialize the iusLibrary wrapper
initIusLibrary()

% Get ius functions directly into local namespace
import py.Python3Ius.*

iusErrorSetStreamToFile('errorlog.txt');
%iusErrorAutoReport(1);

% Create the iq file
iqFileHandle = iusIqFileCreate(iqFilename);
if iqFileHandle == py.None
    error('Unable to open file!');
end
try
    %convert( iqFileHandle, iusIqStruct, iqData, 'doppler' );
    fillInstanceData( iqFileHandle, iusIqStruct, Receive, 'doppler' );
catch ME
    iusIqFileClose(iqFileHandle);
    rethrow(ME);
end
% Close the file
iusIqFileClose(iqFileHandle);
% Free memory
iusIqFileDelete(iqFileHandle);

iusErrorCloseFileStream();
disp('done');
end

function fillInstanceData( h, iusIqStruct, Receive, mode )

% Get ius functions directly into local namespace
import py.Python3Ius.*

%% transducer
transducer = ius2DTransducerCreate( ...
    iusIqStruct.Transducer.transducerName, ...
    IUS_LINE, ...
    iusIqStruct.Transducer.centerFrequency, ...
    iusIqStruct.Transducer.numElements ) ;

elementPos = cell( iusIqStruct.Transducer.numElements, 1);
elementSize = cell( iusIqStruct.Transducer.numElements, 1);
elementTheta = cell( iusIqStruct.Transducer.numElements, 1);
element = cell( iusIqStruct.Transducer.numElements, 1);
for c1 = 1:iusIqStruct.Transducer.numElements
    elementPos{c1} = ius2DPositionCreate( ...
        iusIqStruct.Transducer.Elements.positions.x(c1), ...
        iusIqStruct.Transducer.Elements.positions.z(c1) );
    elementSize{c1} = ius2DSizeCreate( ...
        iusIqStruct.Transducer.Elements.sizes.x(c1), ...
        iusIqStruct.Transducer.Elements.sizes.z(c1) );    
    elementTheta{c1} = iusIqStruct.Transducer.Elements.angles.theta(c1);
    element{c1} = ius2DTransducerElementCreate( elementPos{c1}, elementTheta{c1}, elementSize{c1} );
    ius2DTransducerSetElement( transducer, c1-1, element{c1} );
end
iusIqFileSetTransducer( h, transducer );

%% acquisition
acquisition = iusAcquisitionCreate( ...
    iusIqStruct.Experiment.speedOfSound, ...
    iusIqStruct.Experiment.date, ...
    iusIqStruct.Experiment.description);
iusIqFileSetAcquisition( h, acquisition );

%% pulseDict
if iusIqStruct.DrivingScheme.TransmitPulse.numPulseValues == 0
    pulseFrequency = iusIqStruct.DrivingScheme.TransmitPulse.pulseFrequency;
    pulseAmplitude = iusIqStruct.DrivingScheme.TransmitPulse.pulseAmplitude;
    numPulses = iusIqStruct.DrivingScheme.TransmitPulse.pulseCount;
    pulse = iusParametricPulseCreate( pulseFrequency, pulseAmplitude, numPulses );
else
    error('Non parametric pulse not implemented yet!');
end
pulseDict = iusPulseDictCreate();
iusPulseDictSet( pulseDict, mode, pulse );
iusIqFileSetPulseDict( h, pulseDict );

%% pulseSourceDict
if iusIqStruct.DrivingScheme.drivingSchemeType == 3 % Single element!
    numLocations = iusIqStruct.DrivingScheme.numTransmitSources;
    if numLocations ~= 1
        error('Single element recording expected.');
    end
    source = ius2DParametricSourceCreate( numLocations, 0, 0, 0);
else
    error('Driving scheme type not implemented yet');
end
sourceDict = iusSourceDictCreate();
iusSourceDictSet( sourceDict, mode, source );
iusIqFileSetSourceDict( h, sourceDict );

%% receiveChannelMapDict
numChannels = iusIqStruct.Transducer.numElements;
startDelays = iusIqStruct.ReceiveSettings.startDepth;
if all(diff(startDelays) == 0) % All transmit apodizations are the same.
    startDelay = startDelays(1);
else
    error('Different startDelays per pulse not yet supported!');
end
receiveChannelMap = iusReceiveChannelMapCreate( numChannels );
for c1 = 1:numChannels
    iusReceiveChannelMapSetChannel( receiveChannelMap, c1-1, c1-1 );
    iusReceiveChannelMapSetChannel( receiveChannelMap, c1-1, startDelay );
end
receiveChannelMapDict = iusReceiveChannelMapDictCreate();
iusReceiveChannelMapDictSet( receiveChannelMapDict, mode, receiveChannelMap );
iusIqFileSetReceiveChannelMapDict( h, receiveChannelMapDict );

%% transmitApodizationDict
ta = iusIqStruct.DrivingScheme.transmitApodization;
if all(all(diff(ta,1) == 0)) % All transmit apodizations are the same.
    transmitApodization = iusTransmitApodizationCreate( ta(1,:) );
    transmitApodizationDict = iusTransmitApodizationDictCreate();
    iusTransmitApodizationDictSet( transmitApodizationDict, mode, ...
        transmitApodization );
    iusIqFileSetTransmitApodizationDict( h, transmitApodizationDict );    
else
    error('Different transmitApodizations per pulse not yet supported!');
end

%% demodulationDict
numTGCValues = iusIqStruct.ReceiveSettings.TimeGainControl.numValues;
tgc = iusTGCCreate( numTGCValues );
for c1 = 1:numTGCValues
    iusTGCSet( tgc, c1-1, ...
        iusIqStruct.ReceiveSettings.TimeGainControl.timings(c1), ...
        iusIqStruct.ReceiveSettings.TimeGainControl.gains(c1) );
end
filter = [Receive.InputFilter Receive.InputFilter((end-1):-1:1)];
kernelSize = length(filter);
preFilter = iusFirFilterCreate( kernelSize );
for c1 = 1:kernelSize
    iusFirFilterSetCoefficient( preFilter, c1-1, filter(c1) );
end

centerFrequency = 0.0;
if Receive.quadDecim == 1
    error('Is demodulation applied?');
end
demodulationDict = iusDemodulationDictCreate();
demodulation = iusDemodulationCreate( ...
    IUS_DEMODULATION_QUADRATURE, ...
    iusIqStruct.ReceiveSettings.sampleFrequency, ...
    centerFrequency, ...
    Receive.demodFrequency, ...
    numTGCValues, ...
    kernelSize);
iusDemodulationSetTGC( demodulation, tgc );
iusDemodulationSetPreFilter( demodulation, preFilter );

iusDemodulationDictSet( demodulationDict, mode, demodulation );
iusIqFileSetDemodulationDict( h, demodulationDict );

%% iqPatternListDict
iqPatternListDict = iusIqPatternListDictCreate();
numTransmitPulses = iusIqStruct.DrivingScheme.numTransmitPulses;
iqPatternList = iusIqPatternListCreate( numTransmitPulses, demodulationDict, receiveChannelMapDict );
iqPattern = cell(numTransmitPulses, 1);
for c1 = 1:numTransmitPulses
    iqPattern{c1} = iusIqPatternCreate( iusIqStruct.DrivingScheme.TransmitPattern.transmitPatternTime(c1), mode, mode, mode, mode, mode );
    iusIqPatternListSet( iqPatternList, iqPattern{c1}, c1-1 );
end
iusIqPatternListDictSet( iqPatternListDict, mode, iqPatternList );
iusIqFileSetPatternListDict( h, iqPatternListDict );

%% frameList
numFrames = iusIqStruct.numFrames;
frameList = iusFrameListCreate( numFrames );
frames = cell(numFrames, 1);
timeToNextFrame = iusIqStruct.DrivingScheme.TransmitPattern.transmitPatternTime(end) + iusIqStruct.DrivingScheme.TransmitPattern.transmitPatternDelay;
for c1 = 1:numFrames
    frames{c1} = iusFrameCreate( mode, c1-1, (c1-1) * timeToNextFrame );
    iusFrameListSet( frameList, frames{c1}, c1-1 );
end
iusIqFileSetFrameList( h, frameList );

% Store instance data to disk
iusIqFileNodeSave( h );

%% Cleanup memory

% Transducer
for c1 = 1:iusIqStruct.Transducer.numElements
    ius2DPositionDelete( elementPos{c1} );
    ius2DSizeDelete( elementSize{c1} );
    ius2DTransducerElementDelete( element{c1} );
end
iusTransducerDelete( transducer );

% Acquisition
iusAcquisitionDelete( acquisition );

% Pulse
iusPulseDelete( pulse );
iusPulseDictDelete( pulseDict );

% Source
iusSourceDelete( source );
iusSourceDictDelete( sourceDict );

% ReceiveChannelMap
iusReceiveChannelMapDelete( receiveChannelMap );
iusReceiveChannelMapDictDelete( receiveChannelMapDict );

% TransmitApodization
iusTransmitApodizationDelete( transmitApodization );
iusTransmitApodizationDictDelete( transmitApodizationDict );

% Demodulation
iusTGCDelete( tgc );
iusFirFilterDelete( preFilter );
iusDemodulationDelete( demodulation );
iusDemodulationDictDelete( demodulationDict );

% IqPatternList
for c1 = 1:numTransmitPulses
    iusIqPatternDelete( iqPattern{c1} );
end
iusIqPatternListDelete( iqPatternList );
iusIqPatternListDictDelete( iqPatternListDict );

% Framelist
for c1 = 1:numFrames
    iusFrameDelete( frames{c1} );
end
iusFrameListDelete( frameList );

end
