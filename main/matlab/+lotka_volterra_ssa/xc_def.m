% lotka_volterra_ssa.XC_DEF was generated on Sat Jan 04, 2014 02:33:42 PM by xcpp
%
% Provides the definition of the objects that the functions in
% this package return. Provide no arguments to obtain the whole
% map. If you need only one definition then provide the key.
%
function obj = xc_def(varargin)
    persistent defs;
    if isempty(defs)
        defs = containers.Map;
            end
    if isempty(varargin)
        obj = defs;
    else
        obj = defs(varargin{1});
    end
end
