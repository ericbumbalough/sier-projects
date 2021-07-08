# sier-projects
Codes that were used to generate Sierpinski Triangle patterns for 3d printing or laser engraving.

Please excuse issues in this code, it was only intended as one-time use.

## Book Cover
This notebook cover is a piece of laser engraved cedar plywood.

![Final Book Cover](https://github.com/ericbumbalough/sier-projects/blob/main/finished%20cover.jpg)

First, I wrote a [C++ code to generate an the base image using Cairo](https://github.com/ericbumbalough/sier-projects/blob/main/Entourage%20Book%20Cover.cpp). Different darknesses indidicate different depths.

![Laser Engraving Image](https://github.com/ericbumbalough/sier-projects/blob/main/cover.png)

An emperical correlation between laser settings and depth of cut was found to create the desired cuts.

## Ring
This ring is 3D printed in stainless steel.
![Final Ring](https://github.com/ericbumbalough/sier-projects/blob/main/final%20ring.jpg)
It was impractical to create the highly detailed pattern below manually in Autodesk Inventor, so I used their API from VBA. I wrote a code (embedded in the .ipt file) that created all of the triangles.
![Pattern](https://github.com/ericbumbalough/sier-projects/blob/main/Final%20Sierpinski%20Wrap.png)
Then, in Inventor I mapped those triangles to the surface, cut them out, and exported the file as a .stl for the 3D printing company.
![STL Model](https://github.com/ericbumbalough/sier-projects/blob/main/ring%20solid%20mode.png)
