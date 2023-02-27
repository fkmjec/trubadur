# Developer documentation for Trubadur
Trubadur is a simple guitar tuner. It uses the fast fourier transform (from FFTW library) to convert incoming sound data into the frequency spectrum, then does some postprocessing and finds the peak. It displays the measured data in a window provided by Qt. What sets Trubadur apart from other tuners is the fact that a lot of constants and workings of the postprocessing are configurable in the interface, therefore allowing the user to tweak and experiment (and gain more understanding of how digital tuners work).

## Backend principle
We collect the sound data in the form of floats in a circular buffer. Then, every 0.2 seconds, we take a specified amount of data from that buffer and we run a Fourier transform on it. We thereby obtain a representation in the frequency spectrum.

Since we do not want to have low frequency humm (perhaps from power outlets) interfering with our measurements, we specifically set the intensities for frequencies up to X Hz to zero. Then, we normalize to values between 0 and 1. After that, we run a[harmonic product spectrum algorithm](http://musicweb.ucsd.edu/~trsmyth/analysis/Harmonic_Product_Spectrum.html) to remove the non-fundamental frequencies, and then we return the frequency where the peak is highest.

## Program structure
The entry point in `main.cpp` does four important things:
* it initializes the Config object, which is global for the whole app
* it creates the UI, with its Displayer and ConfigPanel components arranged above each other
* it starts the audio recording with the built-in class QAudioSource class and our AudioReader class, connecting them together with Qt signals and slots
* it initializes the FrequencyCalculator class and connects it to the AudioReader, so that new data is stored and processed. It also connects it to Displayer to send it newly calculated frequency data. Lastly, a QTimer is initialized to call the frequency calculation function every 0.2 seconds. 

Now let us look at the classes and what they do more in-depth:

### Config
The Config object stores app configuration. It contains fields configuring mostly the behaviour of the backend (so that it is changable by the user) as well as signals to notify other classes that depend on Config and its changes. The thought is that when an object needs to be aware of a configuration option, it stores a pointer to the Config object in itself and subscribes to the change signals. When the configuration is changed, the object is then notified. The best example of this behaviour is the FrequencyCalculator class which is dependant on the buffer size and window size configuration options and needs to reallocate every time they change.

The fields of the config object are the following:
* `concertPitch` - 440 Hz by default, western tradition
* `mainsHummThr` - 62 Hz by default, lower frequencies are automatically removed
* `HPSSteps` - harmonic product spectrum steps to remove non-fundamental frequencies
* `sampleRate` - sample rate of the sound card, currently hardcoded to be 48000Hz
* `windowSize` - the FFT window size, 96000 by default Together with the sample rate, this sets our frequency resolution to roughly 0.5 Hz.
* `bufferSize` - the capacity of the circular buffer in FrequencyCalculator, 96000 by default
* `noteBufferSize` - size of an internal buffer in Displayer - it only displays a note when all the notes in the buffer are the same. Default is three.
* `processingInterval` - the interval between subsequent calls to `updateFrequency` in FrequencyCalculator, given in miliseconds. Default is 200. 

### ConfigPanel
ConfigPanel is the user interface to Config. It is derived from QWidget, and it contains the input fields and validators for some of the configuration fields. When changed, these fields emit a signal that the Config object registers it, validates it and updates everything accordingly.

### FrequencyCalculator
This class contains most of the backend logic. Internally, it has a circular buffer that stores a number of floats (96000 by default). It also has two arrays of configurable length for FFT to run in. The main interface are the following slots:
* `updateFrequency` - calculates a new value for the fundamental heard frequency and emits it in a `frequencyChange` signal
* `newData` - takes new data from the AudioReader and stores it in the circular buffer.
When updateFrequency is called, we take `config->windowSize` samples from the buffer and multiply them with a Hann window to reduce spectral leakage. Then, we run a real FFT (from the FFTW library), remove low frequency humm (configured by `config->mainsHummThr`), run HPS filtering with `config->HPSSteps` as described in the section `Backend Principle` and return the frequency where peak is the highest.

### Displayer
The displayer class displays the tones and frequencies in user-legible form. It is derived from QWidget and takes the global Config object in the constructor to have access to the currently set concert pitch and the noteBufferSize config option. It communicates with the FrequencyCalculator class via a slot - when the FrequencyCalculator computes a new frequency reading, it sends it through a signal to Displayer. Also, Displayer only displays notes when all note readings in its internal noteBuffer are the same.

### AudioReader
The AudioReader class is only a simple wrapper that resends data written to it by QAudioSource to FrequencyCalculator. The implementation is nothing interesting apart from the signal `writeData`.

### CircularBuffer
The CircularBuffer class provides a simple generic implementation of a ring buffer with a vector of a fixed size as the underlying data structure. It has an `append` method and a `getLast` method for appending and extracting data. Notably, the `getLast` method has two implemetations, where one returns a vector and the other one copies the data to a provided buffer.

## Dependencies
The app depends on Qt 6.4 and FFTW3. The build is provided by the Meson build system.

## Sources
I benefited a lot from reading the guide [Programming a Guitar Tuner with Python](https://www.chciken.com/digital/signal/processing/2020/05/13/guitar-tuner.html). I would also like to thank [Jenda Hrach](https://jenda.hrach.eu/) for helping me understand the Fourier transform for signal processing. 


