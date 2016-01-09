declare name        "HOA scene flipping";
declare version     "1.0";
declare author      "Pierre Lecomte";
declare license     "GPL";
declare copyright   "(c) Pierre Lecomte 2015";

// HOA scene flipping : front-back, left-right and up-down, according to [1,p46-47]
// [1] Kronlachner, M. (2014). Spatial Transformations for the Alteration of Ambisonic Recordings. Graz University Of Technology, Austria.

import("math.lib");
on=checkbox("up-down");

//up-down switching
updown=vgroup("HOA scene flipping",(
_,
_,(_<:if(on==1,_*-1,_)),_,
_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,
_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,
_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,
_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_,(_<:if(on==1,_*-1,_)),_));

process=updown;