% lotka_volterra_ssa.TIME_SERIES was generated on Sat Jan 04, 2014 02:33:44 PM by xcpp 
% 
%     Generate realizations from the Lotka-Volterra Model by
%     collecting points that Gillespie's stochastic simulation
%     algorithm generates at a given frequency. This function returns
%     `t` and `x`.
% 
%     parameters:
% 
%       `npoints`: number of points per realization
%       `ntrials`: number of realizations
%       `freq`: collection frequency
%     
function varargout = time_series(npoints, ntrials, freq, varargin)
    tdump = excentury.dump.textdumper;
    tdump.dump(npoints, 'npoints', {'I', '4'});
    tdump.dump(ntrials, 'ntrials', {'I', '4'});
    tdump.dump(freq, 'freq', {'R', '8'});
    in_str = tdump.close;
    len_in = length(in_str);
    [~, out_str] = lotka_volterra_ssa.time_series_mex(len_in, in_str);
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
