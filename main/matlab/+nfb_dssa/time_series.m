% nfb_dssa.TIME_SERIES was generated on Sun Jan 05, 2014 03:21:07 PM by xcpp 
% 
%     Generate realizations from the Negative Feedback model by
%     collecting points that the DSSA generates at a given frequency.
%     This function returns `t` and `x`.
% 
%     parameters:
% 
%       `npoints`: number of points per realization
%       `ntrials`: number of realizations
%       `freq`: collection frequency
%       `delay`: time by which an event is delayed
%     
function varargout = time_series(npoints, ntrials, freq, delay, varargin)
    tdump = excentury.dump.textdumper;
    tdump.dump(npoints, 'npoints', {'I', '4'});
    tdump.dump(ntrials, 'ntrials', {'I', '4'});
    tdump.dump(freq, 'freq', {'R', '8'});
    tdump.dump(delay, 'delay', {'R', '8'});
    in_str = tdump.close;
    len_in = length(in_str);
    [~, out_str] = nfb_dssa.time_series_mex(len_in, in_str);
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
