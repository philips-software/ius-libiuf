function convertMatlab2IusIqFile( iqFilename, iusIqStruct, iqData )
% CONVERTMATLAB2IUSIQFILE writes meta data and iq data to ius iq file.
%    CONVERTMATLAB2IUSIQFILE( iqFilename, iusIqstruct, iqData ) converts
%    meta-data in the iusIqStruct to header information in the iusIq file
%    and writes the data iqData to the iusIq file with version format 3.

% Initialize the iusLibrary wrapper
initIusLibrary()

% Get ius functions directly into local namespace
import py.Python3Ius.*

iusErrorSetStreamToFile('errorlog.txt');
iusErrorAutoReport(1);

% Create the iq file
iqFileHandle = iusIqFileCreate(iqFilename);
if iqFileHandle == py.None
    error('Unable to open file!');
end
try
    %convert( iqFileHandle, iusIqStruct, iqData, 'doppler' );
    fillInstanceData( iqFileHandle, iusIqStruct, 'doppler' );
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

function fillInstanceData( h, iusIqStruct, mode )

% Get ius functions directly into local namespace
import py.Python3Ius.*

% acquisition
acquisition = iusAcquisitionCreate( ...
    iusIqStruct.Experiment.speedOfSound, ...
    iusIqStruct.Experiment.date, ...
    iusIqStruct.Experiment.description);
iusIqFileSetAcquisition( h, acquisition );

% dataStreamDict??

% demodulationDict

% frameList

% iqPatternListDict

% pulseDict
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

% pulseSourceDict
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

% receiveChannelMapDict

% transducer
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

% transmitApodizationDict
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

% Store instance data to disk
iusIqFileNodeSave( h );

%% Cleanup memory
% Acquisition
iusAcquisitionDelete( acquisition );

% Pulse
iusPulseDelete( pulse );
iusPulseDictDelete( pulseDict );

% Source
iusSourceDelete( source );
iusSourceDictDelete( sourceDict );

% Transducer
for c1 = 1:iusIqStruct.Transducer.numElements
    ius2DPositionDelete( elementPos{c1} );
    ius2DSizeDelete( elementSize{c1} );
    ius2DTransducerElementDelete( element{c1} );
end
iusTransducerDelete( transducer );

% TransmitApodization
iusTransmitApodizationDelete( transmitApodization );
iusTransmitApodizationDictDelete( transmitApodizationDict );
end
