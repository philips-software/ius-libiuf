import yaml
import io
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('fileName', help='Name of output YAML file', type=str)
parser.add_argument('centerFrequency', help='Transducers Center frequency', type=float)
parser.add_argument('shape', choices=['IUF_LINE', 'IUF_CIRCLE', 'IUF_PLANE', 'IUF_CYLINDER', 'IUF_SPHERE'])
parser.add_argument('transducerName', type=str)
parser.add_argument('numElements', help='Number of transducer elements', type=int)
parser.add_argument('pitchX', help='transducer pitch in X dimension', type=float)
parser.add_argument('pitchZ', help='transducer pitch in Z dimension', type=float)
parser.add_argument('-pitchY', help='transducer pitch in Y dimension for 3D probes only', type=float)
parser.add_argument('sizeX', help='transducer size in X dimension', type=float)
parser.add_argument('sizeZ', help='transducer size in Z dimension', type=float)
parser.add_argument('-sizeY', help='transducer size in Y dimension for 3D probes only', type=float)
parser.add_argument('theta', help='transducer angle around y axis', type=float)
parser.add_argument('-phi', help='transducer angle iaround x axis for 3D probes only', type=float)                    
args = parser.parse_args()

#print(args)
# Define data
transducer = {'CenterFrequency': args.centerFrequency, 
              'Shape': args.shape, 
              'TransducerName': args.transducerName}
transducer.update({'Elements': {}})
is3D = (args.shape == 'IUF_PLANE') or (args.shape == 'IUF_CYLINDER') or (args.shape == 'IUF_SPHERE') 
for i in range(0,args.numElements):
    xpos = -(args.numElements/2) * args.pitchX + i * args.pitchX
    zpos = -(args.numElements/2) * args.pitchZ + i * args.pitchZ
    #element = {'Position': {'X': xpos, 'Z': zpos}}    

    elementName = 'Element[%d]' % i
    transducer['Elements'].update({elementName: {}})
    transducer['Elements'][elementName].update({'Theta': args.theta})
    if is3D:
        ypos = -(args.numElements/2) * args.pitchY + i * args.pitchY   
        transducer['Elements'][elementName].update({'Position': {'X': xpos, 'Y': ypos, 'Z': zpos}})
        transducer['Elements'][elementName].update({'Size': {'SX': args.sizeX, 'SY': args.sizeY, 'SZ': args.sizeZ}})
        transducer['Elements'][elementName].update({'Phi': args.phi})        
    else:
        transducer['Elements'][elementName].update({'Position': {'X': xpos, 'Z': zpos}})
        transducer['Elements'][elementName].update({'Size': {'SX': args.sizeX, 'SZ': args.sizeZ}})

# Write YAML file
with io.open(args.fileName, 'w', encoding='utf8') as outfile:
    yaml.dump(transducer, outfile, default_flow_style=False, allow_unicode=True)
