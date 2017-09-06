# PixelTrace
A Simple CLI-program for vectorizing and upscaling images.

## Dependencies
- Potrace (libpotrace)

## Usage
PixelTrace ouputs a SVG file, output can be altered with the following options:

PixelTrace

- --help            : Display different options and parameters
- -f, --file        : define input-file (.bmp, .png, .tga, .jpg, .gif, .psd, .hdr, .pic)
- -o, --output      : define output-name (without format!)
- -t, --turdsize    : vectorization suppresses speckles of up to this size (default 2)
- -a, --alphamax    : vectorization corner threshold parameter (default 1)
- -O, --opttolerance: curve optimization tolerance (default 0.2)
- -z, --turnpolicy  : how to resolve ambiguities in path decomposition (black,white,left,right,majority,minority,random)  