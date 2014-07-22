% nfb_dsde.XC_STRUCT was generated on Sun Jan 05, 2014 10:32:14 PM by xcpp
%
% Return an excentury.xc_strut object using the entries of the
% containers.Map provided by this package (nfb_dsde.xc_def).
%
% The following are equivalent:
%
%     p = excentury.xc_struct('classname', nfb_dsde.xc_def('classname'));
%
%     p = nfb_dsde.xc_struct('classname');
%
% 
%
function obj = xc_struct(name)
    obj = excentury.xc_struct(name, nfb_dsde.xc_def(name));
end

