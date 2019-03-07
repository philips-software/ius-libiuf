function [] = create(obj, filename)
%CREATE Summary of this function goes here
%   Detailed explanation goes here

if ~ischar( filename )
    error('Input filename must be a string');
end

if ~isa( filename, 'file' )
    fprintf('Creating file %s for writing\n', filename);
    obj.fp = fopen( filename, 'w' );
else
    fprintf('Opening file %s for reading\n', filename);
    obj.fp = fopen( filename, 'r' );
end

end

