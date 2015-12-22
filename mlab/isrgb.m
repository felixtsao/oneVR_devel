function y = isrgb(x)
%ISRGB Return true for RGB image.
% FLAG = ISRGB(A) returns 1 if A is an RGB truecolor image and
% 0 otherwise.
% % ISRGB uses these criteria to determine if A is an RGB image:
% % - If A is of class double, all values must be in the range
% [0,1], and A must be M-by-N-by-3.
% % - If A is of class uint8 or uint16, A must be M-by-N-by-3.
% % Note that a four-dimensional array that contains multiple RGB
% images returns 0, not 1.
% % Class Support
% -------------
% A can be of class uint8, uint16, or double.
% % See also ISBW, ISGRAY, ISIND.

% Copyright 1993-2000 The MathWorks, Inc.
% $Revision: 1.9 $ $Date: 2000/01/21 20:16:59 $

y = size(x,3)==3;
if y
if isa(x, 'uint8')
y = ~islogical(x);
elseif isa(x, 'double')
% At first just test a small chunk to get a possible quick negative
[m,n,o] = size(x);
chunk = x(1:min(m,10),1:min(n,10),:);
y = (~islogical(chunk) & min(chunk(:))&gt==0 & max(chunk(:))&lt==1);
% If the chunk is an RGB image, test the whole image
if y
y = (min(x(:))&gt==0 & max(x(:))&lt==1);
end
end
end
y = double(logical(y));

