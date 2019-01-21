%=============================================================================
%    COPYRIGHT 2016 PHILIPS RESEARCH
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
        
    mxAlgo      = fullfile( '../m', filesep, ['mx' Algo]);
    mxcAlgo     = ['mx' Algo '.c'];
    
    if isunix  
        if( g == 0 )
            iuslib = fullfile(here, 'build/Linux/library/libius-3.1.0.a');
        else
            iuslib = fullfile(here, 'build/Linux/library/libius-3.1.0d.a');
        end                
        hdf5path = fullfile('/cadappl', 'python', '2.7-64');
        hdf5lib = fullfile(hdf5path, 'lib', 'libhdf5.so');
        hdf5_hllib = fullfile(hdf5path, 'lib', 'libhdf5_hl.so');
        libzlib = fullfile(hdf5path, 'lib', 'libz.so');
        libszip = ''; %fullfile(hdf5path, 'lib', 'libszip.lib');
        hdf5incl = ''; %['-I' fullfile(hdf5path, 'include')];
        
    elseif ispc
        if( g == 0 )
            iuslib  = fullfile(here, 'build/Windows/library/Release/ius-3.1.0.lib');
        else            
            iuslib  = fullfile(here, 'build/Windows/library/Debug/ius-3.1.0d.lib');
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
        %mex('-largeArrayDims', inclPath1, inclPath2, hdf5incl, '-output', mxAlgo, mxcAlgo, iuslib, hdf5lib, hdf5_hllib, libzlib, libszip );
        mex('-largeArrayDims', inclPath1, inclPath2, '-output', mxAlgo, mxcAlgo, iuslib, hdf5lib, hdf5_hllib, libzlib, '-lgcov', '-luuid');
    else
        fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
        mex('-g', '-largeArrayDims', inclPath1, inclPath2, '-output', mxAlgo, mxcAlgo, iuslib, hdf5lib, hdf5_hllib, libzlib, '-lgcov', '-luuid');
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
%     cAlgo       = fullfile(here, ['src' filesep], ['ius' Algo '.c']);
%     cMatrix     = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cVector     = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cVectorC    = fullfile(here, ['src' filesep 'base/iusVectorC.c']);
%     cLUT        = fullfile(here, ['src' filesep 'base/iusLUT.c']);
%     cBasic      = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
%     cFFTC       = fullfile(here, ['src' filesep 'base/iusFFTC.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep './' ], ['ius' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cVectorC       = fullfile(here, ['src' filesep 'base/iusVectorC.c']);
%     cFFT           = fullfile(here, ['src' filesep 'base/iusFFT.c']);
%     cFFTC          = fullfile(here, ['src' filesep 'base/iusFFTC.c']);
%     cDFT           = fullfile(here, ['src' filesep 'base/iusDFT.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep './' ], ['ius' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cMatrixC       = fullfile(here, ['src' filesep 'base/iusMatrixC.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cVectorC       = fullfile(here, ['src' filesep 'base/iusVectorC.c']);
%     cFFT           = fullfile(here, ['src' filesep 'base/iusFFT.c']);
%     cFFTC          = fullfile(here, ['src' filesep 'base/iusFFTC.c']);
%     cDFT           = fullfile(here, ['src' filesep 'base/iusDFT.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
%     cHilbert       = fullfile(here, ['src' filesep 'iusHilbertDetect.c']);
%     cQuadratureDetectUtilsC = fullfile(here, ['src' filesep 'base/iusQuadratureDetectUtils.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep './' ], ['ius' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cMatrixC       = fullfile(here, ['src' filesep 'base/iusMatrixC.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cVectorC       = fullfile(here, ['src' filesep 'base/iusVectorC.c']);
%     cFFT           = fullfile(here, ['src' filesep 'base/iusFFT.c']);
%     cFFTC          = fullfile(here, ['src' filesep 'base/iusFFTC.c']);
%     cDFT           = fullfile(here, ['src' filesep 'base/iusDFT.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
%     cQuadratureDetectUtilsC = fullfile(here, ['src' filesep 'base/iusQuadratureDetectUtils.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep], ['ius' Algo '.c']);
%     cFilter        = fullfile(here, ['src' filesep 'iusFilter.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cFFT           = fullfile(here, ['src' filesep 'base/iusFFT.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep], ['ius' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep], ['ius' Algo '.c']);
%     cMatrixUtils   = fullfile(here, ['src' filesep 'base/iusMatrixUtils.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep], ['ius' Algo '.c']);
%     cMatrixUtils   = fullfile(here, ['src' filesep 'base/iusMatrixUtils.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep 'fileio' filesep], ['ius' Algo 'Reader.c']);
%     cMatrix  = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cVector  = fullfile(here, ['src' filesep 'base/iusVector.c']);
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
%     cAlgo    = fullfile(here, ['src' filesep], ['ius' Algo '.c']);
%     cMatrix        = fullfile(here, ['src' filesep 'base/iusMatrix.c']);
%     cVector        = fullfile(here, ['src' filesep 'base/iusVector.c']);
%     cBasic         = fullfile(here, ['src' filesep 'base/iusBasicFunctions.c']);
%     if (g == 0)
%         fprintf( 'creating %s.mexw64 (RELEASE)\n', mxAlgo);
%         mex('-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cBasic);
%     else
%         fprintf( 'creating %s.mexw64 (DEBUG)\n', mxAlgo);
%         mex('-g', '-largeArrayDims', inclPath, '-output', mxAlgo, mxcAlgo, cAlgo, cMatrix, cVector, cBasic);
%     end
% end

