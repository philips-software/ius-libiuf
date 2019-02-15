clear all;
close all;
clc;

if ispc
    DATA_INPUT_PATH = '\\code1\storage\2016_ius_ux\INPUT\preclinical\2018-11-30_Verasonics_CORFLOW1_0_UtrechtGDL\';
    DATA_OUTPUT_PATH = '\\code1\storage\2016_ius_ux\processing\preclinical\2018-11-30_Verasonics_CORFLOW1_0_UtrechtGDL\';
else
    DATA_INPUT_PATH = '/home/2016_ius/INPUT/preclinical/2018-11-30_Verasonics_CORFLOW1_0_UtrechtGDL/';
    DATA_OUTPUT_PATH = '/home/2016_ius/processing/preclinical/2018-11-30_Verasonics_CORFLOW1_0_UtrechtGDL/';
end

infname = '20181130_140810_20181130_SpectralDoppler_GDL_study_Utrecht_ProtoWireXICE6M_wireCode_07.mat';
outfname = strrep(infname, '.mat', '.iq');

data = load( fullfile(DATA_INPUT_PATH, infname) );

convertMatLab2IusIqFile( outfname, data.iusIq, data.M );

import py.Python3Ius.*
iq = data.iusIq;
iusIqStruct = data.iusIq;



