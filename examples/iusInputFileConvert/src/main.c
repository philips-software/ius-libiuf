//
// Created by Ruijzendaal on 07/03/2018.
//
#include "ius.h"
#include "hdf5.h"
#include "iusInputFileReadV2.h"


static int copyFrameData(hid_t sourceFile, iuif_t destinationFile, char* label)
{
	int status = 0;
	int i;
	iud_t frame = iusInputFileFrameCreate(destinationFile, label);
	iuo_t offset = iusOffsetCreate();

	int numChannels = iusInputFileGetNumChannels(destinationFile, label);
	int numResponses = iusInputFileGetNumResponses(destinationFile, label);
	int numSamplesPerLine = 10 ;//iusInputFileGetSamplesPerLine(destinationFile, label);
	int frameDataSize = numChannels * numResponses * numSamplesPerLine;

	hid_t   memspace;
	hid_t   dataspace;
	hsize_t memdim[3];
	hsize_t count[4];
	hsize_t inputOffset[4];
	hid_t rfData = H5Dopen(sourceFile, "rfData", H5P_DEFAULT);

	//float *frameData = (float *)calloc(frameDataSize, sizeof(float));
	float *pData = iusDataGetPointer(frame);
	if (pData == NULL || frameDataSize == 0)
	{
		return IUS_ERR_MEMORY;
	}

	count[0] = numChannels;
	count[1] = numSamplesPerLine;
	count[2] = numResponses;
	count[3] = 1;

	memdim[0] = numChannels;
	memdim[1] = numSamplesPerLine;
	memdim[2] = numResponses;
	memspace = H5Screate_simple(3, memdim, NULL);
	dataspace = H5Dget_space(rfData);


	int numFrames = iusInputFileGetNumFrames(destinationFile);
	for (i = 0; i<numFrames; i++)
	{
		inputOffset[0] = 0;
		inputOffset[1] = 0;
		inputOffset[2] = 0;
		inputOffset[3] = i;
		offset->t = i;

		status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, inputOffset, NULL, count, NULL);
		printf("%d: reading frame\n", i);
		status |= H5Dread(rfData, H5T_NATIVE_FLOAT, memspace, dataspace, H5P_DEFAULT, pData); //read the data into frame
		printf("%d: writing frame\n", i);
		status |= iusInputFileFrameSave(destinationFile, label, frame, offset); //save frame
	}

	iusDataDelete(frame);
	printf("done\n");
	return status;

}

int main
(
    int argc,
    char *argv[]
)
{
	int iusVersion = 0;
	int status;
	iuif_t ifh = IUIF_INVALID; //fileHandle for version 3
    iusErrorLog(IUS_TRUE);

	// check input arguments, we expect 2: an inputFile and a target filename 
	if (argc != 3)
	{
        IUS_ERROR_PRINT(IUS_ERR_MAJ_GENERAL, IUS_ERR_MIN_ARG_VALUE,
                "expected 2 arguments: and input filename and an output filename. If the input file is V2 it will be converted to V3 and vice versa");
		exit(-1);
	}
	// we use the ius V3 library, so for V2 we use direct HDF5 reads
	hid_t inputHandle = H5Fopen(argv[1], H5F_ACC_RDONLY, H5P_DEFAULT);
	if (inputHandle < 0)
	{
        IUS_ERROR_PRINT(IUS_ERR_MAJ_GENERAL, IUS_ERR_MIN_ARG_FILENAME, "input file does not seem to be a HD5F file");
		exit(-1);
	}
	status = iusHdf5ReadInt(inputHandle, "IUSVersion", &iusVersion);
	if (status != IUS_E_OK)
	{
        IUS_ERROR_PRINT(IUS_ERR_MAJ_GENERAL, IUS_ERR_MIN_ARG_VALUE, "could not find a version number in the file");
		exit(-11);
	}

	char *mode = "bmode";
	if (iusVersion == 2)
	{
		ifh = iusInputFileCreate(argv[2]); //create output file
		if (readInputFileV2(ifh, inputHandle, mode))
		{
			printf("successfully converted file.\n");
		}
		status = iusInputFileNodeSave(ifh);
		if (status != IUS_E_OK)
		{
            IUS_ERROR_PRINT(IUS_ERR_MAJ_GENERAL, IUS_ERR_MIN_HDF5, "saving file");
		}
		else
		{
			//copy data
			printf("Copying data\n");
			status |= copyFrameData(inputHandle, ifh, mode);
		}
	}
	else
	{
		printf("Conversion from V3 to V2 not yet implemented\n");
	}
    
    // Create Meta data in Node
    iusInputFileClose(ifh);
    iusInputFileDelete(ifh);
    return status;
}
