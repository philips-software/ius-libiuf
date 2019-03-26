function varargout = inputFile(varargin)
% INPUTFILE MATLAB code for inputFile.fig
%      INPUTFILE, by itself, creates a new INPUTFILE or raises the existing
%      singleton*.
%
%      H = INPUTFILE returns the handle to a new INPUTFILE or the handle to
%      the existing singleton*.
%
%      INPUTFILE('CALLBACK',hObject,~,handles,...) calls the local
%      function named CALLBACK in INPUTFILE.M with the given input arguments.
%
%      INPUTFILE('Property','Value',...) creates a new INPUTFILE or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before inputFile_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to inputFile_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help inputFile

% Last Modified by GUIDE v2.5 25-Mar-2019 15:38:10

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @inputFile_OpeningFcn, ...
                   'gui_OutputFcn',  @inputFile_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before inputFile is made visible.
function inputFile_OpeningFcn(hObject, ~, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to inputFile (see VARARGIN)

% Choose default command line output for inputFile
handles.output = hObject;
handles.fileName = '';
handles.filePath = '';
%mxCreate;
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes inputFile wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = inputFile_OutputFcn(hObject, ~, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in ReceiveSettingsDict.
function ReceiveSettingsDict_Callback(hObject, ~, handles)
% hObject    handle to ReceiveSettingsDict (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns ReceiveSettingsDict contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ReceiveSettingsDict


% --- Executes during object creation, after setting all properties.
function ReceiveSettingsDict_CreateFcn(hObject, ~, handles)
% hObject    handle to ReceiveSettingsDict (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in PulseDict.
function PulseDict_Callback(hObject, ~, handles)
% hObject    handle to PulseDict (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns PulseDict contents as cell array
%        contents{get(hObject,'Value')} returns selected item from PulseDict


% --- Executes during object creation, after setting all properties.
function PulseDict_CreateFcn(hObject, ~, handles)
% hObject    handle to PulseDict (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in SourceDict.
function SourceDict_Callback(hObject, ~, handles)
% hObject    handle to SourceDict (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns SourceDict contents as cell array
%        contents{get(hObject,'Value')} returns selected item from SourceDict


% --- Executes during object creation, after setting all properties.
function SourceDict_CreateFcn(hObject, ~, handles)
% hObject    handle to SourceDict (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in listbox6.
function listbox6_Callback(hObject, ~, handles)
% hObject    handle to listbox6 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns listbox6 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from listbox6


% --- Executes during object creation, after setting all properties.
function listbox6_CreateFcn(hObject, ~, handles)
% hObject    handle to listbox6 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in listbox7.
function listbox7_Callback(hObject, ~, handles)
% hObject    handle to listbox7 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns listbox7 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from listbox7


% --- Executes during object creation, after setting all properties.
function listbox7_CreateFcn(hObject, ~, handles)
% hObject    handle to listbox7 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in listbox8.
function listbox8_Callback(hObject, ~, handles)
% hObject    handle to listbox8 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns listbox8 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from listbox8


% --- Executes during object creation, after setting all properties.
function listbox8_CreateFcn(hObject, ~, handles)
% hObject    handle to listbox8 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in PatternList.
function PatternList_Callback(hObject, ~, handles)
% hObject    handle to PatternList (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns PatternList contents as cell array
%        contents{get(hObject,'Value')} returns selected item from PatternList


% --- Executes during object creation, after setting all properties.
function PatternList_CreateFcn(hObject, ~, handles)
% hObject    handle to PatternList (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in PatterListRefresh.
function PatterListRefresh_Callback(hObject, ~, handles)
% hObject    handle to PatterListRefresh (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in PatternListAdd.
function PatternListAdd_Callback(hObject, ~, handles)
% hObject    handle to PatternListAdd (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in PatternListDelete.
function PatternListDelete_Callback(hObject, ~, handles)
% hObject    handle to PatternListDelete (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function SampleFrequency_Callback(hObject, ~, handles)
% hObject    handle to SampleFrequency (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SampleFrequency as text
%        str2double(get(hObject,'String')) returns contents of SampleFrequency as a double


% --- Executes during object creation, after setting all properties.
function SampleFrequency_CreateFcn(hObject, ~, handles)
% hObject    handle to SampleFrequency (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function NumSamplesPerLine_Callback(hObject, ~, handles)
% hObject    handle to NumSamplesPerLine (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of NumSamplesPerLine as text
%        str2double(get(hObject,'String')) returns contents of NumSamplesPerLine as a double


% --- Executes during object creation, after setting all properties.
function NumSamplesPerLine_CreateFcn(hObject, ~, handles)
% hObject    handle to NumSamplesPerLine (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function TGCtime_Callback(hObject, ~, handles)
% hObject    handle to TGCtime (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of TGCtime as text
%        str2double(get(hObject,'String')) returns contents of TGCtime as a double


% --- Executes during object creation, after setting all properties.
function TGCtime_CreateFcn(hObject, ~, handles)
% hObject    handle to TGCtime (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function TGCgain_Callback(hObject, ~, handles)
% hObject    handle to TGCgain (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of TGCgain as text
%        str2double(get(hObject,'String')) returns contents of TGCgain as a double


% --- Executes during object creation, after setting all properties.
function TGCgain_CreateFcn(hObject, ~, handles)
% hObject    handle to TGCgain (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes on button press in ReceiveSettingsGet.
function ReceiveSettingsGet_Callback(hObject, ~, handles)
% hObject    handle to ReceiveSettingsGet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
import py.Python3Iuf.*
index = handles.ReceiveSettingsDict.Value;
label = handles.ReceiveSettingsDict.String{index};
handles.ReceiveSettingsLabel.String{1} = label;

rs = iufReceiveSettingsDictGet(handles.instance.receiveSettingsDict, label);
sampleFrequency = iufReceiveSettingsGetSampleFrequency(rs);

handles.SampleFrequency.String = num2str(sampleFrequency);
numSamplesPerLine = double(iufReceiveSettingsGetNumSamplesPerLine(rs));
handles.NumSamplesPerLine.String = num2str(numSamplesPerLine);

tgc = iufReceiveSettingsGetTGC(rs);
numTGCValues = double(iufTGCGetNumValues(tgc));
timeString="";
gainString="";
for i=0:numTGCValues-1
    timeString = timeString + num2str(iufTGCGetTime(tgc,i)) +" ";
    gainString = gainString + num2str(iufTGCGetGain(tgc,i)) +" ";
end
handles.TGCtime.String = timeString;
handles.TGCgain.String = gainString;
guidata(hObject, handles);

% --- Executes on button press in ReceiveSettingsSet.
function ReceiveSettingsSet_Callback(hObject, ~, handles)
% hObject    handle to ReceiveSettingsSet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
import py.Python3Iuf.*
index = handles.ReceiveSettingsDict.Value;
label = handles.ReceiveSettingsDict.String{index};

%set the new label
sampleFrequency = str2double(handles.SampleFrequency.String);
numSamplesPerLine = str2double(handles.NumSamplesPerLine.String);
TGCtime = strread(handles.TGCtime.String); %#ok<*DSTRRD>
TGCgain = strread(handles.TGCgain.String);

if length(TGCtime) ~= length(TGCgain)
    handles.ErrorLog.String{1} = 'ReceiveSettingsSet TCG time and gain have different number of elements! not setting element';
    return;
end

rs = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, length(TGCtime));
for i=0:length(TGCtime)-1
    tgc = iufReceiveSettingsGetTGC(rs);
    iufTGCSet(tgc, i, TGCtime(i+1), TGCgain(i+1));    
end

labelExists = ismember(handles.ReceiveSettingsLabel.String{1}, handles.ReceiveSettingsDict.String);
if sum(labelExists)~=0 %remove the old receiveSettings
    iufReceiveSettingsDictRemove(handles.instance.receiveSettingsDict, label);
end
iufReceiveSettingsDictSet(handles.instance.receiveSettingsDict, ...
                                   handles.ReceiveSettingsLabel.String{1}, ...
                                   rs);
labels = iufReceiveSettingsDictGetKeys(handles.instance.receiveSettingsDict);
for i=1:length(labels)
  handles.ReceiveSettingsDict.String{i} = string(labels{i});
end
guidata(hObject, handles);

function pulseFrequency_Callback(hObject, ~, handles)
% hObject    handle to pulseFrequency (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pulseFrequency as text
%        str2double(get(hObject,'String')) returns contents of pulseFrequency as a double


% --- Executes during object creation, after setting all properties.
function pulseFrequency_CreateFcn(hObject, ~, handles)
% hObject    handle to pulseFrequency (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function pulseAmplitude_Callback(hObject, ~, handles)
% hObject    handle to pulseAmplitude (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pulseAmplitude as text
%        str2double(get(hObject,'String')) returns contents of pulseAmplitude as a double


% --- Executes during object creation, after setting all properties.
function pulseAmplitude_CreateFcn(hObject, ~, handles)
% hObject    handle to pulseAmplitude (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function pulseCount_Callback(hObject, ~, handles)
% hObject    handle to pulseCount (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pulseCount as text
%        str2double(get(hObject,'String')) returns contents of pulseCount as a double


% --- Executes during object creation, after setting all properties.
function pulseCount_CreateFcn(hObject, ~, handles)
% hObject    handle to pulseCount (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in parametric.
function parametric_Callback(hObject, ~, handles)
% hObject    handle to parametric (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of parametric



function numPulseValues_Callback(hObject, ~, handles)
% hObject    handle to numPulseValues (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of numPulseValues as text
%        str2double(get(hObject,'String')) returns contents of numPulseValues as a double


% --- Executes during object creation, after setting all properties.
function numPulseValues_CreateFcn(hObject, ~, handles)
% hObject    handle to numPulseValues (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function pRawPulseAmplitudes_Callback(hObject, ~, handles)
% hObject    handle to pRawPulseAmplitudes (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pRawPulseAmplitudes as text
%        str2double(get(hObject,'String')) returns contents of pRawPulseAmplitudes as a double


% --- Executes during object creation, after setting all properties.
function pRawPulseAmplitudes_CreateFcn(hObject, ~, handles)
% hObject    handle to pRawPulseAmplitudes (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function pRawPulseRimes_Callback(hObject, ~, handles)
% hObject    handle to pRawPulseRimes (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pRawPulseRimes as text
%        str2double(get(hObject,'String')) returns contents of pRawPulseRimes as a double


% --- Executes during object creation, after setting all properties.
function pRawPulseRimes_CreateFcn(hObject, ~, handles)
% hObject    handle to pRawPulseRimes (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in PulseGet.
function PulseGet_Callback(hObject, ~, handles)
% hObject    handle to PulseGet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in PulseSet.
function PulseSet_Callback(hObject, ~, handles)
% hObject    handle to PulseSet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in SourceGet.
function SourceGet_Callback(hObject, ~, handles)
% hObject    handle to SourceGet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in sourceSet.
function sourceSet_Callback(hObject, ~, handles)
% hObject    handle to sourceSet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on selection change in SourceType.
function SourceType_Callback(hObject, ~, handles)
% hObject    handle to SourceType (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: contents = cellstr(get(hObject,'String')) returns SourceType contents as cell array
%        contents{get(hObject,'Value')} returns selected item from SourceType
contents = cellstr(get(hObject,'String'));
handles.group_2DParametric.Visible = 'off';
handles.group_3DParametric.Visible = 'off';
handles.group_2DNonParametric.Visible = 'off';
handles.group_3DNonParametric.Visible = 'off';
selected = contents{get(hObject,'Value')};
if strcmp(selected, '2D parametric')
    handles.group_2DParametric.Visible='on';
elseif strcmp(selected, '3D parametric')
    handles.group_3DParametric.Visible='on';
elseif strcmp(selected, '2D non-parametric')
    handles.group_2DNonParametric.Visible='on';
elseif strcmp(selected, '3D non-parametric')
    handles.group_3DNonParametric.Visible='on';
end

% --- Executes during object creation, after setting all properties.
function SourceType_CreateFcn(hObject, ~, handles)
% hObject    handle to SourceType (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function NumLocations_2DP_Callback(hObject, ~, handles)
% hObject    handle to NumLocations_2DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of NumLocations_2DP as text
%        str2double(get(hObject,'String')) returns contents of NumLocations_2DP as a double


% --- Executes during object creation, after setting all properties.
function NumLocations_2DP_CreateFcn(hObject, ~, handles)
% hObject    handle to NumLocations_2DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function FNumber_2DP_Callback(hObject, ~, handles)
% hObject    handle to FNumber_2DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of FNumber_2DP as text
%        str2double(get(hObject,'String')) returns contents of FNumber_2DP as a double


% --- Executes during object creation, after setting all properties.
function FNumber_2DP_CreateFcn(hObject, ~, handles)
% hObject    handle to FNumber_2DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function StartTheta_2DP_Callback(hObject, ~, handles)
% hObject    handle to StartTheta_2DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of StartTheta_2DP as text
%        str2double(get(hObject,'String')) returns contents of StartTheta_2DP as a double


% --- Executes during object creation, after setting all properties.
function StartTheta_2DP_CreateFcn(hObject, ~, handles)
% hObject    handle to StartTheta_2DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function DeltaTheta_2DP_Callback(hObject, ~, handles)
% hObject    handle to DeltaTheta_2DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of DeltaTheta_2DP as text
%        str2double(get(hObject,'String')) returns contents of DeltaTheta_2DP as a double


% --- Executes during object creation, after setting all properties.
function DeltaTheta_2DP_CreateFcn(hObject, ~, handles)
% hObject    handle to DeltaTheta_2DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function NumThetaLocations_3DP_Callback(hObject, ~, handles)
% hObject    handle to NumThetaLocations_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of NumThetaLocations_3DP as text
%        str2double(get(hObject,'String')) returns contents of NumThetaLocations_3DP as a double


% --- Executes during object creation, after setting all properties.
function NumThetaLocations_3DP_CreateFcn(hObject, ~, handles)
% hObject    handle to NumThetaLocations_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function numPhiLocations_3DP_Callback(hObject, ~, handles)
% hObject    handle to numPhiLocations_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of numPhiLocations_3DP as text
%        str2double(get(hObject,'String')) returns contents of numPhiLocations_3DP as a double


% --- Executes during object creation, after setting all properties.
function numPhiLocations_3DP_CreateFcn(hObject, ~, handles)
% hObject    handle to numPhiLocations_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function FNumber_3DP_Callback(hObject, ~, handles)
% hObject    handle to FNumber_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of FNumber_3DP as text
%        str2double(get(hObject,'String')) returns contents of FNumber_3DP as a double


% --- Executes during object creation, after setting all properties.
function FNumber_3DP_CreateFcn(hObject, ~, handles)
% hObject    handle to FNumber_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function StartTheta_3DP_Callback(hObject, ~, handles)
% hObject    handle to StartTheta_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of StartTheta_3DP as text
%        str2double(get(hObject,'String')) returns contents of StartTheta_3DP as a double


% --- Executes during object creation, after setting all properties.
function StartTheta_3DP_CreateFcn(hObject, ~, handles)
% hObject    handle to StartTheta_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function StartPhi_3DP_Callback(hObject, ~, handles)
% hObject    handle to StartPhi_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of StartPhi_3DP as text
%        str2double(get(hObject,'String')) returns contents of StartPhi_3DP as a double


% --- Executes during object creation, after setting all properties.
function StartPhi_3DP_CreateFcn(hObject, ~, handles)
% hObject    handle to StartPhi_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function DeltaTheta_3DP_Callback(hObject, ~, handles)
% hObject    handle to DeltaTheta_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of DeltaTheta_3DP as text
%        str2double(get(hObject,'String')) returns contents of DeltaTheta_3DP as a double


% --- Executes during object creation, after setting all properties.
function DeltaTheta_3DP_CreateFcn(hObject, ~, handles)
% hObject    handle to DeltaTheta_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function DeltaPhi_3DP_Callback(hObject, ~, handles)
% hObject    handle to DeltaPhi_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of DeltaPhi_3DP as text
%        str2double(get(hObject,'String')) returns contents of DeltaPhi_3DP as a double


% --- Executes during object creation, after setting all properties.
function DeltaPhi_3DP_CreateFcn(hObject, ~, handles)
% hObject    handle to DeltaPhi_3DP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function NumLocations_2DNP_Callback(hObject, ~, handles)
% hObject    handle to NumLocations_2DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of NumLocations_2DNP as text
%        str2double(get(hObject,'String')) returns contents of NumLocations_2DNP as a double


% --- Executes during object creation, after setting all properties.
function NumLocations_2DNP_CreateFcn(hObject, ~, handles)
% hObject    handle to NumLocations_2DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosX_2DNP_Callback(hObject, ~, handles)
% hObject    handle to PosX_2DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosX_2DNP as text
%        str2double(get(hObject,'String')) returns contents of PosX_2DNP as a double


% --- Executes during object creation, after setting all properties.
function PosX_2DNP_CreateFcn(hObject, ~, handles)
% hObject    handle to PosX_2DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosZ_2DNP_Callback(hObject, ~, handles)
% hObject    handle to PosZ_2DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosZ_2DNP as text
%        str2double(get(hObject,'String')) returns contents of PosZ_2DNP as a double


% --- Executes during object creation, after setting all properties.
function PosZ_2DNP_CreateFcn(hObject, ~, handles)
% hObject    handle to PosZ_2DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function NumLocations_3DNP_Callback(hObject, ~, handles)
% hObject    handle to NumLocations_3DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of NumLocations_3DNP as text
%        str2double(get(hObject,'String')) returns contents of NumLocations_3DNP as a double


% --- Executes during object creation, after setting all properties.
function NumLocations_3DNP_CreateFcn(hObject, ~, handles)
% hObject    handle to NumLocations_3DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosX_3DNP_Callback(hObject, ~, handles)
% hObject    handle to PosX_3DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosX_3DNP as text
%        str2double(get(hObject,'String')) returns contents of PosX_3DNP as a double


% --- Executes during object creation, after setting all properties.
function PosX_3DNP_CreateFcn(hObject, ~, handles)
% hObject    handle to PosX_3DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosY_3DNP_Callback(hObject, ~, handles)
% hObject    handle to PosY_3DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosY_3DNP as text
%        str2double(get(hObject,'String')) returns contents of PosY_3DNP as a double


% --- Executes during object creation, after setting all properties.
function PosY_3DNP_CreateFcn(hObject, ~, handles)
% hObject    handle to PosY_3DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosZ_3DNP_Callback(hObject, ~, handles)
% hObject    handle to PosZ_3DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosZ_3DNP as text
%        str2double(get(hObject,'String')) returns contents of PosZ_3DNP as a double


% --- Executes during object creation, after setting all properties.
function PosZ_3DNP_CreateFcn(hObject, ~, handles)
% hObject    handle to PosZ_3DNP (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton10.
function pushbutton10_Callback(hObject, ~, handles)
% hObject    handle to pushbutton10 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton11.
function pushbutton11_Callback(hObject, ~, handles)
% hObject    handle to pushbutton11 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function numElements_Callback(hObject, ~, handles)
% hObject    handle to numElements (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of numElements as text
%        str2double(get(hObject,'String')) returns contents of numElements as a double


% --- Executes during object creation, after setting all properties.
function numElements_CreateFcn(hObject, ~, handles)
% hObject    handle to numElements (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Apodization_Callback(hObject, ~, handles)
% hObject    handle to Apodization (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Apodization as text
%        str2double(get(hObject,'String')) returns contents of Apodization as a double


% --- Executes during object creation, after setting all properties.
function Apodization_CreateFcn(hObject, ~, handles)
% hObject    handle to Apodization (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function NumChannels_Callback(hObject, ~, handles)
% hObject    handle to NumChannels (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of NumChannels as text
%        str2double(get(hObject,'String')) returns contents of NumChannels as a double


% --- Executes during object creation, after setting all properties.
function NumChannels_CreateFcn(hObject, ~, handles)
% hObject    handle to NumChannels (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Map_Callback(hObject, ~, handles)
% hObject    handle to Map (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Map as text
%        str2double(get(hObject,'String')) returns contents of Map as a double


% --- Executes during object creation, after setting all properties.
function Map_CreateFcn(hObject, ~, handles)
% hObject    handle to Map (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in ReceiveChannelMapGet.
function ReceiveChannelMapGet_Callback(hObject, ~, handles)
% hObject    handle to ReceiveChannelMapGet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in ReceiveChannelMapSet.
function ReceiveChannelMapSet_Callback(hObject, ~, handles)
% hObject    handle to ReceiveChannelMapSet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function StartDelay_Callback(hObject, ~, handles)
% hObject    handle to StartDelay (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of StartDelay as text
%        str2double(get(hObject,'String')) returns contents of StartDelay as a double


% --- Executes during object creation, after setting all properties.
function StartDelay_CreateFcn(hObject, ~, handles)
% hObject    handle to StartDelay (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton14.
function pushbutton14_Callback(hObject, ~, handles)
% hObject    handle to pushbutton14 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pushbutton15.
function pushbutton15_Callback(hObject, ~, handles)
% hObject    handle to pushbutton15 (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function Time_Callback(hObject, ~, handles)
% hObject    handle to Time (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Time as text
%        str2double(get(hObject,'String')) returns contents of Time as a double


% --- Executes during object creation, after setting all properties.
function Time_CreateFcn(hObject, ~, handles)
% hObject    handle to Time (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PulseKey_Callback(hObject, ~, handles)
% hObject    handle to PulseKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PulseKey as text
%        str2double(get(hObject,'String')) returns contents of PulseKey as a double


% --- Executes during object creation, after setting all properties.
function PulseKey_CreateFcn(hObject, ~, handles)
% hObject    handle to PulseKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function SourceKey_Callback(hObject, ~, handles)
% hObject    handle to SourceKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SourceKey as text
%        str2double(get(hObject,'String')) returns contents of SourceKey as a double


% --- Executes during object creation, after setting all properties.
function SourceKey_CreateFcn(hObject, ~, handles)
% hObject    handle to SourceKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ReceiveChannelMapKey_Callback(hObject, ~, handles)
% hObject    handle to ReceiveChannelMapKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ReceiveChannelMapKey as text
%        str2double(get(hObject,'String')) returns contents of ReceiveChannelMapKey as a double


% --- Executes during object creation, after setting all properties.
function ReceiveChannelMapKey_CreateFcn(hObject, ~, handles)
% hObject    handle to ReceiveChannelMapKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function TransmitApodizationKey_Callback(hObject, ~, handles)
% hObject    handle to TransmitApodizationKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of TransmitApodizationKey as text
%        str2double(get(hObject,'String')) returns contents of TransmitApodizationKey as a double


% --- Executes during object creation, after setting all properties.
function TransmitApodizationKey_CreateFcn(hObject, ~, handles)
% hObject    handle to TransmitApodizationKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ReceiveSettingsKey_Callback(hObject, ~, handles)
% hObject    handle to ReceiveSettingsKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ReceiveSettingsKey as text
%        str2double(get(hObject,'String')) returns contents of ReceiveSettingsKey as a double


% --- Executes during object creation, after setting all properties.
function ReceiveSettingsKey_CreateFcn(hObject, ~, handles)
% hObject    handle to ReceiveSettingsKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in PatternGet.
function PatternGet_Callback(hObject, ~, handles)
% hObject    handle to PatternGet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in PatternSet.
function PatternSet_Callback(hObject, ~, handles)
% hObject    handle to PatternSet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on selection change in dimensions.
function dimensions_Callback(hObject, ~, handles)
% hObject    handle to dimensions (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Hints: contents = cellstr(get(hObject,'String')) returns dimensions contents as cell array
%        contents{get(hObject,'Value')} returns selected item from dimensions
contents = cellstr(get(hObject,'String'));
handles.group_2DTransducer.Visible = 'off';
handles.group_3DTransducer.Visible = 'off';
selected = contents{get(hObject,'Value')};
if strcmp(selected, '2D')
    handles.group_2DTransducer.Visible='on';
elseif strcmp(selected, '3D')
    handles.group_3DTransducer.Visible='on';
end

% --- Executes during object creation, after setting all properties.
function dimensions_CreateFcn(hObject, ~, handles)
% hObject    handle to dimensions (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes on selection change in TransducerType_3DT.
function TransducerType_3DT_Callback(hObject, ~, handles)
% hObject    handle to TransducerType_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns TransducerType_3DT contents as cell array
%        contents{get(hObject,'Value')} returns selected item from TransducerType_3DT


% --- Executes during object creation, after setting all properties.
function TransducerType_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to TransducerType_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function TransducerName_3DT_Callback(hObject, ~, handles)
% hObject    handle to TransducerName_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of TransducerName_3DT as text
%        str2double(get(hObject,'String')) returns contents of TransducerName_3DT as a double


% --- Executes during object creation, after setting all properties.
function TransducerName_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to TransducerName_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function CenterFrequency_3DT_Callback(hObject, ~, handles)
% hObject    handle to CenterFrequency_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of CenterFrequency_3DT as text
%        str2double(get(hObject,'String')) returns contents of CenterFrequency_3DT as a double


% --- Executes during object creation, after setting all properties.
function CenterFrequency_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to CenterFrequency_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function NumElements_3DT_Callback(hObject, ~, handles)
% hObject    handle to NumElements_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of NumElements_3DT as text
%        str2double(get(hObject,'String')) returns contents of NumElements_3DT as a double


% --- Executes during object creation, after setting all properties.
function NumElements_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to NumElements_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosX_3DT_Callback(hObject, ~, handles)
% hObject    handle to PosX_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosX_3DT as text
%        str2double(get(hObject,'String')) returns contents of PosX_3DT as a double


% --- Executes during object creation, after setting all properties.
function PosX_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to PosX_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosY_3DT_Callback(hObject, ~, handles)
% hObject    handle to PosY_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosY_3DT as text
%        str2double(get(hObject,'String')) returns contents of PosY_3DT as a double


% --- Executes during object creation, after setting all properties.
function PosY_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to PosY_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosZ_3DT_Callback(hObject, ~, handles)
% hObject    handle to PosZ_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosZ_3DT as text
%        str2double(get(hObject,'String')) returns contents of PosZ_3DT as a double


% --- Executes during object creation, after setting all properties.
function PosZ_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to PosZ_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Theta_3DT_Callback(hObject, ~, handles)
% hObject    handle to Theta_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Theta_3DT as text
%        str2double(get(hObject,'String')) returns contents of Theta_3DT as a double


% --- Executes during object creation, after setting all properties.
function Theta_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to Theta_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Phi_3DT_Callback(hObject, ~, handles)
% hObject    handle to Phi_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Phi_3DT as text
%        str2double(get(hObject,'String')) returns contents of Phi_3DT as a double


% --- Executes during object creation, after setting all properties.
function Phi_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to Phi_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function SizeX_3DT_Callback(hObject, ~, handles)
% hObject    handle to SizeX_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SizeX_3DT as text
%        str2double(get(hObject,'String')) returns contents of SizeX_3DT as a double


% --- Executes during object creation, after setting all properties.
function SizeX_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to SizeX_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function SizeY_3DT_Callback(hObject, ~, handles)
% hObject    handle to SizeY_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SizeY_3DT as text
%        str2double(get(hObject,'String')) returns contents of SizeY_3DT as a double


% --- Executes during object creation, after setting all properties.
function SizeY_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to SizeY_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function SizeZ_3DT_Callback(hObject, ~, handles)
% hObject    handle to SizeZ_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SizeZ_3DT as text
%        str2double(get(hObject,'String')) returns contents of SizeZ_3DT as a double


% --- Executes during object creation, after setting all properties.
function SizeZ_3DT_CreateFcn(hObject, ~, handles)
% hObject    handle to SizeZ_3DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in TransducerType_2DT.
function TransducerType_2DT_Callback(hObject, ~, handles)
% hObject    handle to TransducerType_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns TransducerType_2DT contents as cell array
%        contents{get(hObject,'Value')} returns selected item from TransducerType_2DT


% --- Executes during object creation, after setting all properties.
function TransducerType_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to TransducerType_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function TransducerName_2DT_Callback(hObject, ~, handles)
% hObject    handle to TransducerName_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of TransducerName_2DT as text
%        str2double(get(hObject,'String')) returns contents of TransducerName_2DT as a double


% --- Executes during object creation, after setting all properties.
function TransducerName_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to TransducerName_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function CenterFrequency_2DT_Callback(hObject, ~, handles)
% hObject    handle to CenterFrequency_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of CenterFrequency_2DT as text
%        str2double(get(hObject,'String')) returns contents of CenterFrequency_2DT as a double


% --- Executes during object creation, after setting all properties.
function CenterFrequency_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to CenterFrequency_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function NumElements_2DT_Callback(hObject, ~, handles)
% hObject    handle to NumElements_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of NumElements_2DT as text
%        str2double(get(hObject,'String')) returns contents of NumElements_2DT as a double


% --- Executes during object creation, after setting all properties.
function NumElements_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to NumElements_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosX_2DT_Callback(hObject, ~, handles)
% hObject    handle to PosX_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosX_2DT as text
%        str2double(get(hObject,'String')) returns contents of PosX_2DT as a double


% --- Executes during object creation, after setting all properties.
function PosX_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to PosX_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PosZ_2DT_Callback(hObject, ~, handles)
% hObject    handle to PosZ_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PosZ_2DT as text
%        str2double(get(hObject,'String')) returns contents of PosZ_2DT as a double


% --- Executes during object creation, after setting all properties.
function PosZ_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to PosZ_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Theta_2DT_Callback(hObject, ~, handles)
% hObject    handle to Theta_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Theta_2DT as text
%        str2double(get(hObject,'String')) returns contents of Theta_2DT as a double


% --- Executes during object creation, after setting all properties.
function Theta_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to Theta_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function SizeX_2DT_Callback(hObject, ~, handles)
% hObject    handle to SizeX_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SizeX_2DT as text
%        str2double(get(hObject,'String')) returns contents of SizeX_2DT as a double


% --- Executes during object creation, after setting all properties.
function SizeX_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to SizeX_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function SizeZ_2DT_Callback(hObject, ~, handles)
% hObject    handle to SizeZ_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SizeZ_2DT as text
%        str2double(get(hObject,'String')) returns contents of SizeZ_2DT as a double


% --- Executes during object creation, after setting all properties.
function SizeZ_2DT_CreateFcn(hObject, ~, handles)
% hObject    handle to SizeZ_2DT (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function SpeedOfSound_Callback(hObject, ~, handles)
% hObject    handle to SpeedOfSound (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of SpeedOfSound as text
%        str2double(get(hObject,'String')) returns contents of SpeedOfSound as a double


% --- Executes during object creation, after setting all properties.
function SpeedOfSound_CreateFcn(hObject, ~, handles)
% hObject    handle to SpeedOfSound (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Date_Callback(hObject, ~, handles)
% hObject    handle to Date (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Date as text
%        str2double(get(hObject,'String')) returns contents of Date as a double


% --- Executes during object creation, after setting all properties.
function Date_CreateFcn(hObject, ~, handles)
% hObject    handle to Date (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Description_Callback(hObject, ~, handles)
% hObject    handle to Description (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Description as text
%        str2double(get(hObject,'String')) returns contents of Description as a double


% --- Executes during object creation, after setting all properties.
function Description_CreateFcn(hObject, ~, handles)
% hObject    handle to Description (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in FrameList.
function FrameList_Callback(hObject, ~, handles)
% hObject    handle to FrameList (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns FrameList contents as cell array
%        contents{get(hObject,'Value')} returns selected item from FrameList


% --- Executes during object creation, after setting all properties.
function FrameList_CreateFcn(hObject, ~, handles)
% hObject    handle to FrameList (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in FrameListAdd.
function FrameListAdd_Callback(hObject, ~, handles)
% hObject    handle to FrameListAdd (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in FrameListDel.
function FrameListDel_Callback(hObject, ~, handles)
% hObject    handle to FrameListDel (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function FrameTime_Callback(hObject, ~, handles)
% hObject    handle to FrameTime (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of FrameTime as text
%        str2double(get(hObject,'String')) returns contents of FrameTime as a double


% --- Executes during object creation, after setting all properties.
function FrameTime_CreateFcn(hObject, ~, handles)
% hObject    handle to FrameTime (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function PatternListKey_Callback(hObject, ~, handles)
% hObject    handle to PatternListKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of PatternListKey as text
%        str2double(get(hObject,'String')) returns contents of PatternListKey as a double


% --- Executes during object creation, after setting all properties.
function PatternListKey_CreateFcn(hObject, ~, handles)
% hObject    handle to PatternListKey (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function DataIndex_Callback(hObject, ~, handles)
% hObject    handle to DataIndex (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of DataIndex as text
%        str2double(get(hObject,'String')) returns contents of DataIndex as a double


% --- Executes during object creation, after setting all properties.
function DataIndex_CreateFcn(hObject, ~, handles)
% hObject    handle to DataIndex (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in FrameGet.
function FrameGet_Callback(hObject, ~, handles)
% hObject    handle to FrameGet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in FrameSet.
function FrameSet_Callback(hObject, ~, handles)
% hObject    handle to FrameSet (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in Load.
function Load_Callback(hObject, ~, handles) %#ok<*DEFNU>
% hObject    handle to Load (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
import py.Python3Iuf.*
iufErrorSetStreamToFile('errorlog.txt');
[handles.fileName, handles.filePath] = uigetfile({'*.hdf5';'*.input'});
handles.iufInputFileInstance = iufInputFileNodeLoad(fullfile(handles.filePath, handles.fileName));
%update the receiveSettingsDict
handles.instance.receiveSettingsDict = iufInputFileGetReceiveSettingsDict(handles.iufInputFileInstance);
cellP = cellfun(@char,cell(handles.instance.receiveSettingsDict.keys),'UniformOutput',false);
handles.ReceiveSettingsDict.String = cellP; 

handles.instance.pulseDict = iufInputFileGetPulseDict(handles.iufInputFileInstance);
cellP = cellfun(@char,cell(handles.instance.pulseDict.keys),'UniformOutput',false);
handles.PulseDict.String = cellP; 

handles.instance.sourceDict = iufInputFileGetSourceDict(handles.iufInputFileInstance);
cellP = cellfun(@char,cell(handles.instance.sourceDict.keys),'UniformOutput',false);
handles.SourceDict.String = cellP; 

handles.instance.transmitApodizationDict = iufInputFileGetTransmitApodizationDict(handles.iufInputFileInstance);
cellP = cellfun(@char,cell(handles.instance.transmitApodizationDict.keys),'UniformOutput',false);
handles.TransmitApodizationDict.String = cellP;

handles.instance.receiveChannelMapDict = iufInputFileGetReceiveChannelMapDict(handles.iufInputFileInstance);
cellP = cellfun(@char,cell(handles.instance.receiveChannelMapDict.keys),'UniformOutput',false);
handles.ReceiveChannelMapDict.String = cellP;

handles.instance.patternListDict = iufInputFileGetPatternListDict(handles.iufInputFileInstance);
cellP = cellfun(@char,cell(handles.instance.patternListDict.keys),'UniformOutput',false);
handles.PatternListDict.String = cellP;

guidata(hObject, handles);
ReceiveSettingsGet_Callback(hObject, 0, handles);

% --- Executes on button press in Save.
function Save_Callback(hObject, ~, handles)
% hObject    handle to Save (see GCBO)
% ~  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



function ReceiveSettingsLabel_Callback(hObject, eventdata, handles)
% hObject    handle to ReceiveSettingsLabel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ReceiveSettingsLabel as text
%        str2double(get(hObject,'String')) returns contents of ReceiveSettingsLabel as a double


% --- Executes during object creation, after setting all properties.
function ReceiveSettingsLabel_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ReceiveSettingsLabel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on key press with focus on ReceiveSettingsLabel and none of its controls.
function ReceiveSettingsLabel_KeyPressFcn(hObject, ~, handles)
% hObject    handle to ReceiveSettingsLabel (see GCBO)
% eventdata  structure with the following fields (see MATLAB.UI.CONTROL.UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)
handles.ReceiveSettingsLabel.String{1} = hObject.String{1};
guidata(hObject, handles);


% --- Executes on button press in ErrorClear.
function ErrorClear_Callback(hObject, ~, handles)
% hObject    handle to ErrorClear (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.ErrorLog.String{1} = '';
guidata(hObject, handles);


% --- Executes on button press in ReceiveSettingsDel.
function ReceiveSettingsDel_Callback(hObject, eventdata, handles)
% hObject    handle to ReceiveSettingsDel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
import py.Python3Iuf.*
index = handles.ReceiveSettingsDict.Value;
label = handles.ReceiveSettingsDict.String{index};

iufReceiveSettingsDictRemove(handles.instance.receiveSettingsDict, label);
labels = iufReceiveSettingsDictGetKeys(handles.instance.receiveSettingsDict);
handles.ReceiveSettingsDict.String = {};
for i=1:length(labels)
  handles.ReceiveSettingsDict.String{i} = string(labels{i});
end
handles.ReceiveSettingsDict.Value = 1;
guidata(hObject, handles);
