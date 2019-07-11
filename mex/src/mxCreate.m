%=============================================================================
%
%  Source Name   : mxCreate.m
%                  Creates a mexdll for a processing C algorithm
%
%=============================================================================
%
%  Usage   mxCreate( selection )
%          mxCreate( selection, debug )
%
%  Input   selection: choose between
%           'all'
%           'mxInputFile'
%
%          debug: 0 or 1 (default is 1 when argument omitted)
%                 When 0, a mex release build is created
%                 When 1, a mex debug build is created
%
%=============================================================================

function mxCreate( selection, g)

    if nargin == 0
        selection = 'all';
        g = 1;
    end

    if nargin==1
        g = 1;
    end
    
    if ~exist( '../m', 'dir' )
        mkdir( '../m' )
    end

    switch ( selection)
        case 'all'
            clear mxInputFile
            mkInputFile( g );
        case 'mxInputFile'
            clear mxInputFile
            mkInputFile( g );
        otherwise
            error('mxCreate: incorrect algorithm selection.\n');
    end
end

function [] = mkInputFile( g )
    Algo = 'InputFile';
    
    here = pwd; idx = strfind(here,[filesep 'mex' filesep ]); here = here(1:idx-1);
    inclPath1 = ['-I' fullfile(here, 'library/include')];
    inclPath2 = ['-I' fullfile(here, 'library')];
    inclPath3 = ['-I.'];
        
    mxAlgo      = fullfile( '../m', filesep, ['mx' Algo]);
    mxcAlgo     = ['mx' Algo '.c'];
    mxcDep      = 'mxHandleList.c';
    
    version = '3.6dd7a4a';
    if isunix  
        
        if( g == 0 )            
            iuflib = fullfile(here, sprintf('build/Linux/library/libiuf-%s.a', version'));
        else
            iuflib = fullfile(here, sprintf('build/Linux/library/libiuf-%sd.a', version));
        end                
        hdf5path = fullfile('/cadappl', 'python', '2.7-64');
        hdf5lib = fullfile(hdf5path, 'lib', 'libhdf5.so');
        hdf5_hllib = fullfile(hdf5path, 'lib', 'libhdf5_hl.so');
        libzlib = fullfile(hdf5path, 'lib', 'libz.so');
        libszip = ''; %fullfile(hdf5path, 'lib', 'libszip.lib');
        hdf5incl = ''; %['-I' fullfile(hdf5path, 'include')];
        
    elseif ispc
        if( g == 0 )
            iuflib  = fullfile(here, sprintf('build/Windows/library/Release/iuf-%s.lib',version) );
        else            
            iuflib  = fullfile(here, sprintf('build/Windows/library/Debug/iuf-%sd.lib', version) );
        end
        hdf5path = fullfile('c:', 'Program Files', 'HDF_Group', 'HDF5', '1.8.20');
        hdf5lib = fullfile(hdf5path, 'lib', 'libhdf5.lib');
        hdf5_hllib = fullfile(hdf5path, 'lib', 'libhdf5_hl.lib');
        libzlib = fullfile(hdf5path, 'lib', 'libzlib.lib');
        libszip = fullfile(hdf5path, 'lib', 'libszip.lib');
        hdf5incl = ['-I' fullfile(hdf5path, 'include')];
    end
    
       
    if (g == 0)
        fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
        %mex('-largeArrayDims', inclPath1, inclPath2, hdf5incl, '-output', mxAlgo, mxcAlgo, mxcDep, iuflib, hdf5lib, hdf5_hllib, libzlib, libszip );
        if ispc 
            mex('-largeArrayDims', inclPath1, inclPath2, inclPath3, hdf5incl, '-output', mxAlgo, mxcAlgo, mxcDep, iuflib, hdf5lib, hdf5_hllib, libzlib, libszip, '-luuid');
        else
            mex('-largeArrayDims', inclPath1, inclPath2, inclPath3, hdf5incl, '-output', mxAlgo, mxcAlgo, mxcDep, iuflib, hdf5lib, hdf5_hllib, libzlib, '-lgcov', '-luuid');
        end
    else
        fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
        if ispc
            mex('-g', '-largeArrayDims', inclPath1, inclPath2, inclPath3, hdf5incl, '-output', mxAlgo, mxcAlgo, mxcDep, iuflib, hdf5lib, hdf5_hllib, libzlib, libszip, '-luuid');
        else
            mex('-g', '-largeArrayDims', inclPath1, inclPath2, inclPath3, hdf5incl, '-output', mxAlgo, mxcAlgo, mxcDep, iuflib, hdf5lib, hdf5_hllib, libzlib, '-lgcov', '-luuid');
        end
    end
end
% 
% function [] = mkCWC4DWD( g )
%     Algo = 'CWC4DWD';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
% %     mxAlgo = fullfile(here, ['mex' filesep 'm' filesep], ['mx' Algo]);
%     mxAlgo      = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo     = ['mx' Algo '.c'];
%     cAlgo       = fullfile(here, ['src' filesep], ['iuf' Algo '.c']);
%     cMatrix     = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cVector     = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cVectorC    = fullfile(here, ['src' filesep 'base/iufVectorC.c']);
%     cLUT        = fullfile(here, ['src' filesep 'base/iufLUT.c']);
%     cBasic      = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     cFFTC       = fullfile(here, ['src' filesep 'base/iufFFTC.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cVectorC, cLUT, cBasic, cFFTC);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cVectorC, cLUT, cBasic, cFFTC);
%     end
% end
% 
% function [] = mkHilbertDetect( g )
%     Algo = 'HilbertDetect';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
% %     mxAlgo = fullfile(here, ['mex' filesep 'm' filesep], ['mx' Algo]);
%     mxAlgo   = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep './' ], ['iuf' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cVectorC       = fullfile(here, ['src' filesep 'base/iufVectorC.c']);
%     cFFT           = fullfile(here, ['src' filesep 'base/iufFFT.c']);
%     cFFTC          = fullfile(here, ['src' filesep 'base/iufFFTC.c']);
%     cDFT           = fullfile(here, ['src' filesep 'base/iufDFT.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cVectorC, cFFT, cFFTC, cDFT, cBasic);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cVectorC, cFFT, cFFTC, cDFT, cBasic);
%     end
%     disp(mxAlgo)
% end
% 
% function [] = mkKasai( g )
%     Algo = 'Kasai';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
% %     mxAlgo = fullfile(here, ['mex' filesep 'm' filesep], ['mx' Algo]);
%     mxAlgo   = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep './' ], ['iuf' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cMatrixC       = fullfile(here, ['src' filesep 'base/iufMatrixC.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cVectorC       = fullfile(here, ['src' filesep 'base/iufVectorC.c']);
%     cFFT           = fullfile(here, ['src' filesep 'base/iufFFT.c']);
%     cFFTC          = fullfile(here, ['src' filesep 'base/iufFFTC.c']);
%     cDFT           = fullfile(here, ['src' filesep 'base/iufDFT.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     cHilbert       = fullfile(here, ['src' filesep 'iufHilbertDetect.c']);
%     cQuadratureDetectUtilsC = fullfile(here, ['src' filesep 'base/iufQuadratureDetectUtils.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cHilbert, cMatrix, cMatrixC, cVector, cVectorC, cFFT, cFFTC, cDFT, cBasic, cQuadratureDetectUtilsC);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cHilbert, cMatrix, cMatrixC, cVector, cVectorC, cFFT, cFFTC, cDFT, cBasic, cQuadratureDetectUtilsC);
%     end
%     disp(mxAlgo)
% end
% 
% function [] = mkSpectralDoppler( g )
%     Algo = 'SpectralDoppler';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
% %     mxAlgo = fullfile(here, ['mex' filesep 'm' filesep], ['mx' Algo]);
%     mxAlgo   = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep './' ], ['iuf' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cMatrixC       = fullfile(here, ['src' filesep 'base/iufMatrixC.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cVectorC       = fullfile(here, ['src' filesep 'base/iufVectorC.c']);
%     cFFT           = fullfile(here, ['src' filesep 'base/iufFFT.c']);
%     cFFTC          = fullfile(here, ['src' filesep 'base/iufFFTC.c']);
%     cDFT           = fullfile(here, ['src' filesep 'base/iufDFT.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     cQuadratureDetectUtilsC = fullfile(here, ['src' filesep 'base/iufQuadratureDetectUtils.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cMatrixC, cVector, cVectorC, cFFT, cFFTC, cDFT, cBasic, cQuadratureDetectUtilsC);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cMatrixC, cVector, cVectorC, cFFT, cFFTC, cDFT, cBasic, cQuadratureDetectUtilsC);
%     end
%     disp(mxAlgo)
% end
% 
% function [] = mkLocalizePulse( g )
%     Algo = 'LocalizePulse';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
% %     mxAlgo = fullfile(here, ['mex' filesep 'm' filesep], ['mx' Algo]);
%     mxAlgo = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep], ['iuf' Algo '.c']);
%     cFilter        = fullfile(here, ['src' filesep 'iufFilter.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cFFT           = fullfile(here, ['src' filesep 'base/iufFFT.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     cDSP_HilbMag   = fullfile(here, ['src' filesep 'DSP_hilbmag_r8_h16_cn.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cFilter, cMatrix, cVector, cFFT, cBasic, cDSP_HilbMag);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cFilter, cMatrix, cVector, cFFT, cBasic, cDSP_HilbMag);
%     end
% end
% 
% function [] = mkQuadratureDetect( g )
%     Algo = 'QuadratureDetect';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
% %     mxAlgo = fullfile(here, ['mex' filesep 'm' filesep], ['mx' Algo]);
%     mxAlgo = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep], ['iuf' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cBasic);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cBasic);
%     end
% end
% 
% function [] = mkSift3dRigidMatch( g )
%     Algo = 'Sift3dRigidMatch';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
% %     mxAlgo = fullfile(here, ['mex' filesep 'm' filesep], ['mx' Algo]);
%     mxAlgo = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep], ['iuf' Algo '.c']);
%     cMatrixUtils   = fullfile(here, ['src' filesep 'base/iufMatrixUtils.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrixUtils, cMatrix, cVector, cBasic);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrixUtils, cMatrix, cVector, cBasic);
%     end
% end
% 
% function [] = mkScanFormat( g )
%     Algo = 'ScanFormat';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
% %     mxAlgo = fullfile(here, ['mex' filesep 'm' filesep], ['mx' Algo]);
%     mxAlgo = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep], ['iuf' Algo '.c']);
%     cMatrixUtils   = fullfile(here, ['src' filesep 'base/iufMatrixUtils.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrixUtils, cMatrix, cVector, cBasic);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrixUtils, cMatrix, cVector, cBasic);
%     end
% end
% 
% function [] = mkCamFile( g )
%     Algo = 'CamFile';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
%     mxAlgo = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep 'fileio' filesep], ['iuf' Algo 'Reader.c']);
%     cMatrix  = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cVector  = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector );
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector );
%     end
% end
% 
% function [] = mkScanConvert( g )
%     Algo = 'ScanConvert';
%     
%     here = pwd; idx = strfind(here,[filesep 'mex' filesep 'src']); here = here(1:idx-1);
%     inclPath = ['-I' fullfile(here, 'include')];
%     
%     mxAlgo = fullfile( '..\m', filesep, ['mx' Algo]);
%     mxcAlgo  = ['mx' Algo '.c'];
%     cAlgo    = fullfile(here, ['src' filesep], ['iuf' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iufMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iufVector.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iufBasicFunctions.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cBasic);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cBasic);
%     end
% end

