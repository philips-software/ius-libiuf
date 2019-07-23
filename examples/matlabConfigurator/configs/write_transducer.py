import yaml
import io
import argparse
import h5py

parser = argparse.ArgumentParser()
parser.add_argument('inputConfig', help='Name of input YAML file', type=str)
parser.add_argument('outputFile', help='Name of output HDF5 file', type=str)
args = parser.parse_args()

def load_config(config_file):
    with open(config_file, 'r') as stream:
        try:
            return yaml.safe_load(stream)
        except yaml.YAMLError as exc:
            print(exc)

config = load_config(args.inputConfig)
shapeDict = {"IUF_INVALID_TRANSDUCER_SHAPE": 0, 
                                                         "IUF_LINE": 1, 
                                                         "IUF_CIRCLE": 2,
                                                         "IUF_PLANE": 3, 	
                                                         "IUF_CYLINDER": 4, 	
                                                         "IUF_SPHERE": 5}
shapesType = h5py.special_dtype(enum=('i', shapeDict))
stringType = h5py.special_dtype(vlen=str)

with h5py.File(args.outputFile, 'w') as f:
   f.create_dataset('Shape', (1,1), dtype=shapesType, data=shapeDict[config['Shape']])
   is3D = shapeDict[config['Shape']] == shapeDict["IUF_PLANE"] or \
          shapeDict[config['Shape']] == shapeDict["IUF_CYLINDER"] or \
          shapeDict[config['Shape']] == shapeDict["IUF_SPHERE"] 
   dset = f.create_dataset('TransducerName', (1,), dtype=stringType)
   dset[0] = config['TransducerName']
   f.create_dataset('CenterFrequency', (1,1), 'f', data=config['CenterFrequency'])
   elementsGroup = f.create_group('Elements')
   for i in range(0,len(config['Elements'])):
       elementName = 'Element[%d]' % i
       elGroup = elementsGroup.create_group(elementName) 
       positionGroup = elGroup.create_group('Position')
       sizeGroup = elGroup.create_group('Size')
       
       positionGroup.create_dataset('X', (1,1), 'f', data=config['Elements'][elementName]['Position']['X'])
       positionGroup.create_dataset('Z', (1,1), 'f', data=config['Elements'][elementName]['Position']['Z'])
       sizeGroup.create_dataset('SX', (1,1), 'f', data=config['Elements'][elementName]['Size']['SX'])
       sizeGroup.create_dataset('SZ', (1,1), 'f', data=config['Elements'][elementName]['Size']['SZ'])
       elGroup.create_dataset('Theta', (1,1), 'f', data=config['Elements'][elementName]['Theta'])

       if is3D:
           print('3D')
           positionGroup.create_dataset('Y', (1,1), 'f', data=config['Elements'][elementName]['Position']['Y'])
           sizeGroup.create_dataset('SY', (1,1), 'f', data=config['Elements'][elementName]['Size']['SY'])
           elGroup.create_dataset('Phi', (1,1), 'f', data=config['Elements'][elementName]['Phi'])
   f.close()

#   for m in f.attrs.keys():
#      print('{} => {}'.format(m, f.attrs[m]))