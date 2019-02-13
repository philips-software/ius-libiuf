function convertMatlab2IusIqFile( iqFilename, iusIqStruct, iqData )
% CONVERTMATLAB2IUSIQFILE writes meta data and iq data to ius iq file.
%    CONVERTMATLAB2IUSIQFILE( iqFilename, iusIqstruct, iqData ) converts
%    meta-data in the iusIqStruct to header information in the iusIq file
%    and writes the data iqData to the iusIq file with version format 3.

% Initialize the iusLibrary wrapper
initIusLibrary()

% Get ius functions directly into local namespace
import py.Python3Ius.*

% Create the iq file
iqHandle = iusIqFileCreate(iqFilename);
if iqHandle == py.None
    error('Unable to open file!');
end

convert( iqHandle, iusIqStruct, iqData, 'doppler' );

% Store to disk
iusIqFileNodeSave(iqHandle);
% Close the file
iusIqFileClose(iqHandle);
% Free memory
iusIqFileDelete(iqHandle);

disp('done');
end

function convert( h, iusIqStruct, iqData, mode )

% Get ius functions directly into local namespace
import py.Python3Ius.*

% sourceDict
% FIXME: correct interpretation of numLocations!
if iusIqStruct.DrivingScheme.drivingSchemeType == 3 % Single element!
    SOURCE_LABEL = 'single_element';
    numLocations = iusIqStruct.DrivingScheme.numTransmitSources;
    if numLocations ~= 1
        error('Single element recording expected.');
    end
    source = ius2DParametricSourceCreate( numLocations, 0, 0, 0);
else
    error('Driving scheme type not implemented yet');
end
sourceDict = iusSourceDictCreate();
if 0 ~= iusSourceDictSet( sourceDict, SOURCE_LABEL, source)
    iusErrorPrint();
end
if 0 ~= iusIqFileSetSourceDict( h, sourceDict )
    iusErrorPrint();
end

% pulseDict
PULSE_LABEL = 'doppler';
if iusIqStruct.DrivingScheme.TransmitPulse.numPulseValues == 0
    pulseFrequency = iusIqStruct.DrivingScheme.TransmitPulse.pulseFrequency;
    pulseAmplitude = iusIqStruct.DrivingScheme.TransmitPulse.pulseAmplitude;
    numPulses = iusIqStruct.DrivingScheme.TransmitPulse.pulseCount;
    pulse = iusParametricPulseCreate( pulseFrequency, pulseAmplitude, numPulses );
else
    assert('Non parametric pulse not implemented yet!');
end
pulseDict = iusPulseDictCreate();
if 0 ~= iusPulseDictSet( pulseDict, PULSE_LABEL, pulse )
    iusErrorPrint();
end
if 0 ~= iusIqFileSetPulseDict( h, pulseDict )
   iusErrorPrint();
end 

% receiveSettingsDict
%if 0 ~= iusIqFileSetReceiveSettingsDict( 

% receiveChannelMapDict ==> Demodulation scheme!

% patternListDict

% transmitApodizationDict
% Convert from many TA's to a dict!
TRANSMIT_APODIZATION_LABEL = 'doppler';
ta = iusIqStruct.DrivingScheme.transmitApodization;
transmitApodization = iusTransmitApodizationCreate( size(ta, 2) );
% for c1 = 1:size(ta, 2)
%     iusTransmitApodizationSetElement( transmitApodization, c1-1, ta(1, c1) );
% end
iusTransmitApodizationSetApodization( transmitApodization, ta(1,:) ); 
transmitApodizationDict = iusTransmitApodizationDictCreate();
if 0 ~= iusTransmitApodizationDictSet( transmitApodizationDict, TRANSMIT_APODIZATION_LABEL, transmitApodization );
    iusErrorPrint();
end
if 0 ~= iusIqFileSetTransmitApodizationDict( h, transmitApodizationDict )
    iusErrorPrint();
end

% acquisition
speedOfSound = iusIqStruct.Experiment.speedOfSound;
date = int32(iusIqStruct.Experiment.date);
description = iusIqStruct.Experiment.description;

acquisition = iusAcquisitionCreate( speedOfSound, date, description);
iusIqFileSetAcquisition( h, acquisition );

% transducer

% frameList

end

function initIusLibrary()

% Set python version
[version, executable, isLoaded] = pyversion;
if ~isLoaded
    if ispc
        pyversion 3.6
    else
        pyversion('~/cadbin/python3')
    end
end

% Register python library wrapper to python search path (uses distribution
% i.s.o. local build!
if ispc
    libpath = fullfile( pwd, '..', 'build', 'Windows', 'dist', 'python36', 'Windows');
else
    libpath = fullfile( pwd, '..', 'build', 'Linux', 'dist', 'python36', 'Linux');
end
if count(py.sys.path, libpath) == 0
    insert(py.sys.path,int32(0),libpath);
end
end
