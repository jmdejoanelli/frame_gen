# Dependencies

- cmake
- make
- gcc
- python3.x

# Build:
```bash
mkdir build
cd build
cmake ..
make
```

# Run:
```bash
./frame_gen w h d p r g b f fp
```
w = width (pixels)
h = height (pixels)
d = pixel packing
p = pattern
	0 solid fill
	1 checker fill
	2 horizontal ramp
	3 vertical ramp
	4 diagonal ramp
  5 prbs15
r g b = red, green and blue values to pass to pattern generators
f = output path of unpacked frame
fp = output path of packed frame

Example:
```bash
./frame_gen 1920 1080 R210 1 2048 0 2048 1920x1080_purple_checker.bin 1920x1080_purple_checker.R210
```
Generates a purple checker frame 1920 x 1080
Outputs an unpacked frame: 1920x1080_purple_checker.bin
And a packed frame: 1920x1080_purple_checker.R210

# Batch Run

./gen_frames.sh

# Usage in SignalLab

- Select the Video Mode and Pixel Format to match the test pattern file you want to use.
- Add a "Video: Raw File" generator and enter the path to the test pattern file. Leave the "packing" and "depth" fields set to 0.
- Add any other generators you want to use.
- Start.