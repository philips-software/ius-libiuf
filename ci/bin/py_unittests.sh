#!/usr/bin/env bash
ScriptPath=$(dirname $(realpath $0))
SdkRoot=$(realpath ${ScriptPath}/../..)
Python3Module=Python3Iuf.py
Python2Module=Python2Iuf.py

ModuleFolder=$SdkRoot/build/$(uname)/python/python2
[[ -f $ModuleFolder/$Python2Module ]] && export PYTHONPATH=$ModuleFolder:$PYTHONPATH
ModuleFolder=$SdkRoot/build/$(uname)/python/python3
[[ -f $ModuleFolder/$Python3Module ]] && export PYTHONPATH=$ModuleFolder:$PYTHONPATH


ModuleFolder=$SdkRoot/python/python2/$(uname)
[[ -f $ModuleFolder/$Python2Module ]] && export PYTHONPATH=$ModuleFolder:$PYTHONPATH
ModuleFolder=$SdkRoot/python/python3/$(uname)
[[ -f $ModuleFolder/$Python3Module ]] && export PYTHONPATH=$ModuleFolder:$PYTHONPATH

echo === Python UnitTests
python -m unittest discover python/test

