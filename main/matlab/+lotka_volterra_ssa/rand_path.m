% lotka_volterra_ssa.RAND_PATH was generated on Sat Jan 04, 2014 02:33:43 PM by xcpp 
% 
%     Generate realizations from the Lotka-Volterra Model by
%     collecting each point that Gillespie's stochastic simulation
%     algorithm generates. This function returns `t` and `x`.
% 
%     parameters:
% 
%       `npoints`: number of points per realization
%       `ntrials`: number of realizations
%     
function varargout = rand_path(npoints, ntrials, varargin)
    tdump = excentury.dump.textdumper;
    tdump.dump(npoints, 'npoints', {'I', '4'});
    tdump.dump(ntrials, 'ntrials', {'I', '4'});
    in_str = tdump.close;
    len_in = length(in_str);
    [~, out_str] = lotka_volterra_ssa.rand_path_mex(len_in, in_str);
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
