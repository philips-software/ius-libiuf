%module libiuf

// Add necessary symbols to generated header
%{
#include "iuf.h"
%}

// Process symbols in header
%include "iuf.h"

%pythoncode %{
    __version__ = iufGetVersion()
%}