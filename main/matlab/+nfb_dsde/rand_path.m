% nfb_dsde.RAND_PATH was generated on Sun Jan 05, 2014 10:32:15 PM by xcpp 
% 
%     Generate realizations from the Negative Feedback model by
%     collecting each point that the DSSA generates. This function
%     returns `t` and `x`.
% 
%     parameters:
% 
%       `npoints`: number of points per realization
%       `ntrials`: number of realizations
%       `delay`: time by which an event is delayed
%     
function varargout = rand_path(npoints, ntrials, delay, varargin)
    tdump = excentury.dump.textdumper;
    tdump.dump(npoints, 'npoints', {'I', '4'});
    tdump.dump(ntrials, 'ntrials', {'I', '4'});
    tdump.dump(delay, 'delay', {'R', '8'});
    in_str = tdump.close;
    len_in = length(in_str);
    [~, out_str] = nfb_dsde.rand_path_mex(len_in, in_str);
    [val, order] = excentury.load.textparser(out_str).parse;
    if isempty(varargin)
        nout = length(order);
        varargout = cell(1, nout);
        for num=1:nout;
            varargout{num} = val(order{num});
        end
    else
        varargout{1} = val;
    end
end
