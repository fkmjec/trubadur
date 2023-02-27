# Trubadur
Trubadur is a guitar tuner. It was written as a semester project for the C++ programming class at Charles University.
It is designed to be extensible and have configurable parameters of the sound processing algorithm.

## Setup
* install dependencies `Qt 6.4`, `FFTW3` and the `meson` build system
* run `meson build`, move to the `build` subdirectory and run `meson compile`
* run the resulting binary with `trubadur`

## Usage
The most should be self-evident - you run the application, you play the guitar strings and you will get a frequency reading telling you how correct/incorrect they are. The configuration options have this meaning:
* concert pitch (Hz) - what is the A tone of our tuning system. We use the classical western [twelve-tone equal temperament](https://en.wikipedia.org/wiki/Equal_temperament). The default value is 440 Hz.
* High pass filter - in most of our homes, there is some sort of an electrical system that humms at 50 or 60 Hz. We therefore use a high pass filter to nullify such frequencies - you do not usually need them for tuning a guitar anyway.
* Harmonics removal steps - Trubadur uses the [harmonic product spectrum](http://musicweb.ucsd.edu/~trsmyth/analysis/Harmonic_Product_Spectrum.html) to find the fundamental frequency. The number of steps is the R parameter in the product. 
* FFT window size - we use the fast Fourier transform to convert the signal to the frequency spectrum. This parameter sets the window size and therefore, together with the sample rate, also the frequency resolution. The sample rate is currently hardcoded to be 48kHz, so the frequency resolution is 48kHz divided by window size.
* Saved buffer size - we internally use a ring buffer to store data from the sound card. This is the number of samples that fit into the buffer.

## Further possible improvements (feel free to contribute)
* a command-line interface. It would be very nice to be able to run the program without a window. Also, everything should be almost ready for it, it is just about implementing a switch and another class for an interface.
* more configuration options in the window. There are some parameters (interval between calculations of frequencies...) that could be exposed to the user.
* better filtering of sound input data