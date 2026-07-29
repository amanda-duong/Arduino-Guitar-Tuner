# Arduino Guitar Tuner
Arduino-based guitar tuner developed with C++ to perform real-time pitch detection using FFT-based signal processing. A GY-MAX4466 microphone is incorporated in the circuit to sense, amplify, and process audio input. 
Targeted topics/skills: embedded systems, circuit design, signal processing in a language other than Java

## Hardware Materials
- 1 ELEGOO MEGA R3 Board ATmega 2560
- 1 Small breadboard
- 1 Electret microphone (GY-MAX4466)
- 3 1kΩ Resistors
- 2 Red LEDs
- 1 Green LED
- Assortment of jumper wires

## Software Materials
- C++
- Arduino IDE
- ArduinoFFT library

## Functionality
- Determines frequency of plucked guitar string in real-time
- Determines which string was plucked based on standard string/note frequencies
- LEDs indicate if the string is flat, sharp, or "in tune"

## Implementation
When a string is plucked the microphone captures the analog signal and converts the information into a number using Arduino's ADC (analog-to-digital converter). The program waits for a short period before audio is analyzed using FFT-based methods. The signal is centered based on standard baseline of signal oscillations, and changed from time domain to frequency domain. The processed frequency measurements are compared to the ideal frequency values of the desired strings/notes to determine pitch accuracy. 

## Challenges and Successes
- Simple noise detection was unsuccessful at first. Recordings were stable after soldering microphone components to improve connection.
- Signal processing using zero-cross detection resulted in inconsistent pitch identification. This improved after switching to FFT-based analysis.
- Potential improvements: improve pitch identification with lowered error margin, expand beyond standard tuning / allow for custom tuning