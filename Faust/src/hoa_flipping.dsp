declare name        "HOA scene flipping";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

// HOA scene flipping : front-back, left-right and up-down, according to [1,p46-47]
// [1] Kronlachner, M. (2014). Spatial Transformations for the Alteration of Ambisonic Recordings. Graz University Of Technology, Austria.

import("math.lib");
ud=checkbox("up-down");
lr=checkbox("left-right");
fb=checkbox("front-back");

//up-down switching : all spherical harmonics with order and degree as m+n odd
updown=(
_,
_,(_<:if(ud==1,_*-1,_)),_,
_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,
_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,
_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,
_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_,(_<:if(ud==1,_*-1,_)),_);

//left-right switching : all spherical harmonics of degree n<0
leftright=(
_,
(_<:if(lr==1,_*-1,_)),_,_,
(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),_,_,_,
(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),_,_,_,_,
(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),_,_,_,_,_,
(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),(_<:if(lr==1,_*-1,_)),_,_,_,_,_,_);

//front-back switching : all spherical harmonics of degree (n<0 & n even) and (n>0 & n odd)
frontback=(
_,
_,_,(_<:if(fb==1,_*-1,_)),
(_<:if(fb==1,_*-1,_)),_,_,(_<:if(fb==1,_*-1,_)),_,
_,(_<:if(fb==1,_*-1,_)),_,_,(_<:if(fb==1,_*-1,_)),_,(_<:if(fb==1,_*-1,_)),
(_<:if(fb==1,_*-1,_)),_,(_<:if(fb==1,_*-1,_)),_,_,(_<:if(fb==1,_*-1,_)),_,(_<:if(fb==1,_*-1,_)),_,
_,(_<:if(fb==1,_*-1,_)),_,(_<:if(fb==1,_*-1,_)),_,_,(_<:if(fb==1,_*-1,_)),_,(_<:if(fb==1,_*-1,_)),_,(_<:if(fb==1,_*-1,_)));

process=vgroup("HOA scene flipping",updown:leftright:frontback);