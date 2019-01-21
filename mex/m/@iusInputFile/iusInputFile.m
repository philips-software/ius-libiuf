classdef iusInputFile < handle
    %IUSINPUTFILE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        fp = [];
    end
    
    methods
        function obj = iusInputFile( filename )
            %IUSINPUTFILE Construct an instance of this class
            %   Detailed explanation goes here
            disp('Allocating iusInputFile');
            if nargin > 0 && ischar(filename)
                disp('Creating new file');
                obj.create(filename)
            end
        end
        
        function delete(obj)
            %DELETE Summary of this function goes here
            %   Detailed explanation goes here
            disp('delete called');
            if ~isempty(obj.fp)
                fclose(obj.fp);
                disp('closing file handle');
            end
        end
    end
end

