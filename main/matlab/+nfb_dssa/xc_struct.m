% nfb_dssa.XC_STRUCT was generated on Sun Jan 05, 2014 02:23:42 PM by xcpp
%
% Return an excentury.xc_strut object using the entries of the
% containers.Map provided by this package (nfb_dssa.xc_def).
%
% The following are equivalent:
%
%     p = excentury.xc_struct('classname', nfb_dssa.xc_def('classname'));
%
%     p = nfb_dssa.xc_struct('classname');
%
% 
%
function obj = xc_struct(name)
    obj = excentury.xc_struct(name, nfb_dssa.xc_def(name));
end

