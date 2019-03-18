classdef iufInputFile < handle
    %IUFINPUTFILE Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        PatternLists = struct();
        Pulses = struct();
        Sources = struct();
        ReceiveChannels = struct();
        TransmitApodizations = struct();
        ReceiveSettings = struct();
    end
    
    methods        
        function obj = iufInputFile( )
            %IUFINPUTFILE Construct an instance of this class
            %   Detailed explanation goes here
            
            % Set some initial test data
            
            % TransmitApodization
            obj.TransmitApodizations.bmode = rand(15,1);
            
            % ReceiveChannelMap
            ReceiveChannelMap.map = 1:15;
            ReceiveChannelMap.startDelay = zeros(size(ReceiveChannelMap.map));            
            obj.ReceiveChannels.bmode = ReceiveChannelMap;
                       
            % Pulses
            obj.Pulses
            
        end
       
    end
end

