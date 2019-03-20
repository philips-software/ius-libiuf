function convertMatlab2IufIqFile( iqFilename, iufIqStruct, iqData, Receive )
% CONVERTMATLAB2IUFIQFILE writes meta data and iq data to iuf iq file.
%    CONVERTMATLAB2IUFIQFILE( iqFilename, iufIqstruct, iqData ) converts
%    meta-data in the iufIqStruct to header information in the iufIq file
%    and writes the data iqData to the iufIq file with version format 3.

% Initialize the iufLibrary wrapper
initIufLibrary()

% Get iuf functions directly into local namespace
import py.Python3Iuf.*

% Create the iq file
iqFileHandle = iufIqFileCreate(iqFilename);
if iqFileHandle == py.None
    error('Unable to open file!');
end
try
    %convert( iqFileHandle, iufIqStruct, iqData, 'doppler' );
    fillInstanceData( iqFileHandle, iufIqStruct, 'doppler', Receive, iqData );
catch ME
    iufIqFileClose(iqFileHandle);
    rethrow(ME);
end
% Close the file
iufIqFileClose(iqFileHandle);
% Free memory
iufIqFileDelete(iqFileHandle);

iufErrorCloseFileStream();
disp('done');
end

function fillInstanceData( h, iufIqStruct, mode, Receive, iqData )

% Get iuf functions directly into local namespace
import py.Python3Iuf.*

%% transducer
transducer = iuf2DTransducerCreate( ...
    iufIqStruct.Transducer.transducerName, ...
    IUF_LINE, ...
    iufIqStruct.Transducer.centerFrequency, ...
    iufIqStruct.Transducer.numElements ) ;

elementPos = cell( iufIqStruct.Transducer.numElements, 1);
elementSize = cell( iufIqStruct.Transducer.numElements, 1);
elementTheta = cell( iufIqStruct.Transducer.numElements, 1);
element = cell( iufIqStruct.Transducer.numElements, 1);
for c1 = 1:iufIqStruct.Transducer.numElements
    elementPos{c1} = iuf2DPositionCreate( ...
        iufIqStruct.Transducer.Elements.positions.x(c1), ...
        iufIqStruct.Transducer.Elements.positions.z(c1) );
    elementSize{c1} = iuf2DSizeCreate( ...
        iufIqStruct.Transducer.Elements.sizes.x(c1), ...
        iufIqStruct.Transducer.Elements.sizes.z(c1) );
    elementTheta{c1} = iufIqStruct.Transducer.Elements.angles.theta(c1);
    element{c1} = iuf2DTransducerElementCreate( elementPos{c1}, elementTheta{c1}, elementSize{c1} );
    iuf2DTransducerSetElement( transducer, c1-1, element{c1} );
end
iufIqFileSetTransducer( h, transducer );

%% acquisition
acquisition = iufAcquisitionCreate( ...
    iufIqStruct.Experiment.speedOfSound, ...
    iufIqStruct.Experiment.date, ...
    iufIqStruct.Experiment.description);
iufIqFileSetAcquisition( h, acquisition );

%% pulseDict
if iufIqStruct.DrivingScheme.TransmitPulse.numPulseValues == 0
    pulseFrequency = iufIqStruct.DrivingScheme.TransmitPulse.pulseFrequency;
    pulseAmplitude = iufIqStruct.DrivingScheme.TransmitPulse.pulseAmplitude;
    numPulses = iufIqStruct.DrivingScheme.TransmitPulse.pulseCount;
    pulse = iufParametricPulseCreate( pulseFrequency, pulseAmplitude, numPulses );
else
    error('Non parametric pulse not implemented yet!');
end
pulseDict = iufPulseDictCreate();
iufPulseDictSet( pulseDict, mode, pulse );
iufIqFileSetPulseDict( h, pulseDict );

%% pulseSourceDict
if iufIqStruct.DrivingScheme.drivingSchemeType == 3 % Single element!
    numLocations = iufIqStruct.DrivingScheme.numTransmitSources;
    if numLocations ~= 1
        error('Single element recording expected.');
    end
    source = iuf2DParametricSourceCreate( numLocations, 0, 0, 0);
else
    error('Driving scheme type not implemented yet');
end
sourceDict = iufSourceDictCreate();
iufSourceDictSet( sourceDict, mode, source );
iufIqFileSetSourceDict( h, sourceDict );

%% receiveChannelMapDict
numChannels = iufIqStruct.Transducer.numElements;
startDelays = iufIqStruct.ReceiveSettings.startDepth;
if all(diff(startDelays) == 0) % All transmit apodizations are the same.
    startDelay = startDelays(1);
else
    error('Different startDelays per pulse not yet supported!');
end
receiveChannelMap = iufReceiveChannelMapCreate( numChannels );
for c1 = 1:numChannels
    iufReceiveChannelMapSetChannel( receiveChannelMap, c1-1, c1-1 );
    iufReceiveChannelMapSetChannel( receiveChannelMap, c1-1, startDelay );
end
receiveChannelMapDict = iufReceiveChannelMapDictCreate();
iufReceiveChannelMapDictSet( receiveChannelMapDict, mode, receiveChannelMap );
iufIqFileSetReceiveChannelMapDict( h, receiveChannelMapDict );

%% transmitApodizationDict
ta = iufIqStruct.DrivingScheme.transmitApodization;
if all(all(diff(ta,1) == 0)) % All transmit apodizations are the same.
    transmitApodization = iufTransmitApodizationCreate( ta(1,:) );
    transmitApodizationDict = iufTransmitApodizationDictCreate();
    iufTransmitApodizationDictSet( transmitApodizationDict, mode, ...
        transmitApodization );
    iufIqFileSetTransmitApodizationDict( h, transmitApodizationDict );
else
    error('Different transmitApodizations per pulse not yet supported!');
end

%% demodulationDict
numTGCValues = iufIqStruct.ReceiveSettings.TimeGainControl.numValues;
tgc = iufTGCCreate( numTGCValues );
for c1 = 1:numTGCValues
    iufTGCSet( tgc, c1-1, ...
        iufIqStruct.ReceiveSettings.TimeGainControl.timings(c1), ...
        iufIqStruct.ReceiveSettings.TimeGainControl.gains(c1) );
end
filter = [Receive.InputFilter Receive.InputFilter((end-1):-1:1)];
kernelSize = length(filter);
preFilter = iufFirFilterCreate( kernelSize );
for c1 = 1:kernelSize
    iufFirFilterSetCoefficient( preFilter, c1-1, filter(c1) );
end

centerFrequency = 0.0;
if Receive.quadDecim == 1
    error('Is demodulation applied?');
end
demodulationDict = iufDemodulationDictCreate();
demodulation = iufDemodulationCreate( ...
    IUF_DEMODULATION_FOURX, ...
    iufIqStruct.ReceiveSettings.sampleFrequency, ...
    centerFrequency, ...
    iufIqStruct.DrivingScheme.numSamplesPerLine / 2, ...
    numTGCValues, ...
    kernelSize);
iufDemodulationSetTGC( demodulation, tgc );
iufDemodulationSetPreFilter( demodulation, preFilter );

iufDemodulationDictSet( demodulationDict, mode, demodulation );
iufIqFileSetDemodulationDict( h, demodulationDict );

%% iqPatternListDict
iqPatternListDict = iufIqPatternListDictCreate();
numTransmitPulses = iufIqStruct.DrivingScheme.numTransmitPulses;
iqPatternList = iufIqPatternListCreate( numTransmitPulses, demodulationDict, receiveChannelMapDict );
iqPattern = cell(numTransmitPulses, 1);
for c1 = 1:numTransmitPulses
    iqPattern{c1} = iufIqPatternCreate( iufIqStruct.DrivingScheme.TransmitPattern.transmitPatternTime(c1), mode, mode, mode, mode, mode );
    iufIqPatternListSet( iqPatternList, iqPattern{c1}, c1-1 );
end
iufIqPatternListDictSet( iqPatternListDict, mode, iqPatternList );
iufIqFileSetPatternListDict( h, iqPatternListDict );

%% frameList
numFrames = iufIqStruct.numFrames;
frameList = iufFrameListCreate( numFrames );
frames = cell(numFrames, 1);
timeToNextFrame = iufIqStruct.DrivingScheme.TransmitPattern.transmitPatternTime(end) + iufIqStruct.DrivingScheme.TransmitPattern.transmitPatternDelay;
for c1 = 1:numFrames
    frames{c1} = iufFrameCreate( mode, c1-1, (c1-1) * timeToNextFrame );
    iufFrameListSet( frameList, frames{c1}, c1-1 );
end
iufIqFileSetFrameList( h, frameList );

% Store instance data to disk
iufIqFileNodeSave( h );

%% Store iqData to disk
iData = iufIqFileFrameCreate( h, mode );
qData = iufIqFileFrameCreate( h, mode );

numTransducers = iufTransducerGetNumElements( transducer );
numSamplesPerLine = iufDemodulationGetNumSamplesPerLine( demodulation );
numTransmitPulses = iufIqPatternListGetSize( iqPatternList );
numSamplesPerFrame = numTransducers * numSamplesPerLine * numTransmitPulses;
offset = iufOffsetCreate();
for c1 = 1:numFrames
    iufDataFill( iData, reshape(iqData(:,1:2:end,:,c1), 1, numSamplesPerFrame ) );
    iufDataFill( iData, reshape(iqData(:,2:2:end,:,c1), 1, numSamplesPerFrame ) );
    offset.t = c1-1;
    iufIqFileFrameSave( h, mode, IUF_IQ_COMPONENT_I, iData, offset );
    iufIqFileFrameSave( h, mode, IUF_IQ_COMPONENT_Q, qData, offset );
end
iufOffsetDelete(offset)
iufDataDelete( qData );
iufDataDelete( iData );


%% Cleanup memory

% Transducer
for c1 = 1:iufIqStruct.Transducer.numElements
    iuf2DPositionDelete( elementPos{c1} );
    iuf2DSizeDelete( elementSize{c1} );
    iuf2DTransducerElementDelete( element{c1} );
end
iufTransducerDelete( transducer );

% Acquisition
iufAcquisitionDelete( acquisition );

% Pulse
iufPulseDelete( pulse );
iufPulseDictDelete( pulseDict );

% Source
iufSourceDelete( source );
iufSourceDictDelete( sourceDict );

% ReceiveChannelMap
iufReceiveChannelMapDelete( receiveChannelMap );
iufReceiveChannelMapDictDelete( receiveChannelMapDict );

% TransmitApodization
iufTransmitApodizationDelete( transmitApodization );
iufTransmitApodizationDictDelete( transmitApodizationDict );

% Demodulation
iufTGCDelete( tgc );
iufFirFilterDelete( preFilter );
iufDemodulationDelete( demodulation );
iufDemodulationDictDelete( demodulationDict );

% IqPatternList
for c1 = 1:numTransmitPulses
    iufIqPatternDelete( iqPattern{c1} );
end
iufIqPatternListDelete( iqPatternList );
iufIqPatternListDictDelete( iqPatternListDict );

% Framelist
for c1 = 1:numFrames
    iufFrameDelete( frames{c1} );
end
iufFrameListDelete( frameList );

end
