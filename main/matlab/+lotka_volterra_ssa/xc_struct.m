% lotka_volterra_ssa.XC_STRUCT was generated on Sat Jan 04, 2014 02:33:42 PM by xcpp
%
% Return an excentury.xc_strut object using the entries of the
% containers.Map provided by this package (lotka_volterra_ssa.xc_def).
%
% The following are equivalent:
%
%     p = excentury.xc_struct('classname', lotka_volterra_ssa.xc_def('classname'));
%
%     p = lotka_volterra_ssa.xc_struct('classname');
%
% 
%
function obj = xc_struct(name)
    obj = excentury.xc_struct(name, lotka_volterra_ssa.xc_def(name));
end

