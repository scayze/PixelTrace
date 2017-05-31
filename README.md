# ColorTrace
A Simple CLI-program for vectorizing and upscaling images.

## Dependencies
- SFML2
- Potrace

## Usage
ColorTrace

-f --file [inputFile]

-o --output [outputFileName]



#

Vectorization settings (see potrace documentation for further information)

-t --turdsize [turdsize]

-a --alphamax [alphamax]

-O --opttolerance [OptTolerance]

-z --turnpolicy [turnpolicy] (black,left,majority,minority,random,right,left,white or corresponding number)

It will then generate an png and svg file as output.
