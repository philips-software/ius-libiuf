
create_transducer_config.py generates a yaml config file from transducer parameters.
example:
python create_transducer_config.py s51.yaml 2500000.0 IUF_LINE S5_1 80 2.5E-6 0 2.5E-4 2.5E-4 0.0

usage: create_transducer_config.py [-h] [-pitchY PITCHY] [-sizeY SIZEY]
                                   [-phi PHI]
                                   fileName centerFrequency
                                   {IUF_LINE,IUF_CIRCLE,IUF_PLANE,IUF_CYLINDER,IUF_SPHERE}
                                   transducerName numElements pitchX pitchZ
                                   sizeX sizeZ theta


write_transducer.py converts a yaml config file of a transducer to a hdf5 file
example:
python write_transducer.py s51.yaml s51.hdf5

usage: write_transducer.py [-h] inputConfig outputFile

