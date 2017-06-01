# ColorTrace
A Simple CLI-program for vectorizing and upscaling images.

## Dependencies
- SFML2 (libsfml-graphics2.3v5 and libsfml-system2.3v5 or similar version)
- Potrace (libpotrace)

- Inkscape has to be installed and added to PATH to export the svg to an png file.

## Usage
ColorTrace

- --help            : Display different options and parameters
- -f, --file        : define input-file (.bmp, .png, .tga, .jpg, .gif, .psd, .hdr, .pic)
- -o, --output      : define output-name (without format!)
- -t, --turdsize    : vectorization suppresses speckles of up to this size (default 2)
- -a, --alphamax    : vectorization corner threshold parameter (default 1)
- -O, --opttolerance: curve optimization tolerance (default 0.2)
- -z, --turnpolicy  : how to resolve ambiguities in path decomposition (black,white,left,right,majority,minority,random)  

It will then generate an png and svg file as output.