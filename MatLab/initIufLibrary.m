function initIufLibrary()

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
    %libpath = fullfile( pwd, '..', 'build', 'Windows', 'dist', 'python36', 'Windows');
    libpath = fullfile( pwd, '..', 'build', 'Windows', 'python', 'python3');
    if count(py.sys.path, libpath) == 0
        insert(py.sys.path,int32(0),libpath);
    end
    libpath = fullfile( pwd, '..', 'build', 'Windows', 'python', 'python3', 'Release');
    if count(py.sys.path, libpath) == 0
        insert(py.sys.path,int32(0),libpath);
    end
else
    %libpath = fullfile( pwd, '..', 'build', 'Linux', 'dist', 'python36', 'Linux');
    cd ..
    libpath = fullfile( pwd, 'build', 'Linux', 'python', 'python3');
    cd MatLab
    if count(py.sys.path, libpath) == 0
        insert(py.sys.path,int32(0),libpath);
    end
end
end
