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

convert( iqHandle, iusIqStruct, iqData, 'doppler' );

% Store to disk
iusInputFileNodeSave(iqHandle);
% Close the file
iusIqFileClose(iqHandle);
% Free memory
iusIqFileDelete(iqHandle);

disp('done');
end

function convert( h, iusIqStruct, iqData, mode )

% Get ius functions directly into local namespace
import py.Python3Ius.*

SOURCE_LABEL = 'single_element';
% sourceDict
numLocations = iusIqStruct.DrivingScheme.numTransmitSources;
if numLocations ~= 1
    error('Single element recording expected.');
end
sourceDict = iusSourceDictCreate();
% Would be nice if a double holding an integer automatically gets converted
% to int
source = ius2DParametricSourceCreate( int32(numLocations), 0, 0, 0);
%source is an Ius2DParametricSource * while ius_t is required
iusSourceDictSet( sourceDict, SOURCE_LABEL, source);
iusIqFileSetSourceDict( h, sourceDict );

% pulseDict

% receiveSettingsDict

% receiveChannelMapDict

% patternListDict

% transmitApodizationDict

% acquisition

% transducer

% frameList

end

function initIusLibrary()

% Set python version
[version, executable, isLoaded] = pyversion;
if ~isLoaded
    pyversion 3.6
end

% Register python library wrapper to python search path (uses distribution
% i.s.o. local build!
if ispc
    libpath = fullfile( pwd, '..', 'build', 'Windows', 'dist', 'python36', 'Windows');
else
    libpath = fullfile( pwd, '..', 'build', 'Linux', 'dist', 'python36', 'Windows');
end
if count(py.sys.path, libpath) == 0
    insert(py.sys.path,int32(0),libpath);
end
end
