
const int mic = A0;
const int leftRedLED = 5;
const int greenLED = 4;
const int rightRedLED = 3;
const int MIC_OFFSET = 335;
const int VOL_THRESHOLD = 30;
const int IDLE_TIME = 3000;
const double TOLERANCE = 3;
const int PLUCK_DELAY = 550;

#include <arduinoFFT.h>

const double notes[6] =
{
  78.12,
  105.47,
  140.63,
  191.41,
  242.19,
  328.13
};

const char* names[6] =
{
  "Low E",
  "A",
  "D",
  "G",
  "B",
  "High E"
};


void setup() {
  Serial.begin(115200);
  pinMode(leftRedLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(rightRedLED, OUTPUT);
}

const uint16_t samples = 512;
const double samplingFrequency = 2000;

double vReal[samples];
double vImag[samples];

ArduinoFFT<double> FFT(vReal, vImag, samples, samplingFrequency);

int previous = 0;
unsigned long prevTime = 0;
unsigned long lastPluckTime = 0;

void loop() 
{

  int vol_level = abs(analogRead(mic) - MIC_OFFSET);

  if (vol_level < VOL_THRESHOLD)
  {
      if (millis() - lastPluckTime > IDLE_TIME)
      {
        digitalWrite(leftRedLED, LOW);
        digitalWrite(greenLED, LOW);
        digitalWrite(rightRedLED, LOW);
      }
      return;   // Nothing has been plucked yet
  }

  lastPluckTime = millis();

  delay(PLUCK_DELAY);

  unsigned long samplingPeriod = round(1000000.0 / samplingFrequency);

  for (int i = 0; i < samples; i++)
  {
      unsigned long start = micros();

      vReal[i] = analogRead(mic) - MIC_OFFSET;
      vImag[i] = 0;

      while (micros() - start < samplingPeriod)
      {
          // wait
      }
  }

  double mean = 0;
  for (int i = 0; i < samples; i++) mean += vReal[i];
  mean /= samples;

  for (int i = 0; i < samples; i++) vReal[i] -= mean;


  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

  double threshold = 80.0;

  double lowestFreq = 10000;

  for (int i = 1; i < (samples / 2); i++)
  {
      double freq = (i * samplingFrequency) / samples;
      double magnitude = vReal[i];

      if (freq < 70) continue;

      if (magnitude > threshold)
      {
          if (freq < lowestFreq)
          {
              lowestFreq = freq;
          }
      }
  }

  double freq = lowestFreq;

  double smallestDifference = 100000;
  int closest = -1;

  for (int i = 0; i < 6; i++)
  {
      double difference = abs(freq - notes[i]);

      if (difference < smallestDifference)
      {
          smallestDifference = difference;
          closest = i;
      }
  }

    if (freq > 3000 || freq < 20) 
    {
        freq = 0; // no valid signal detected
    }

    double error = freq - notes[closest];
    if(error < -TOLERANCE)
    {
        digitalWrite(leftRedLED, HIGH);
        digitalWrite(greenLED, LOW);
        digitalWrite(rightRedLED, LOW);
    }
    else if(error > TOLERANCE)
    {
        digitalWrite(leftRedLED, LOW);
        digitalWrite(greenLED, LOW);
        digitalWrite(rightRedLED, HIGH);
    }
    else
    {
        digitalWrite(leftRedLED, LOW);
        digitalWrite(greenLED, HIGH);
        digitalWrite(rightRedLED, LOW);
    }

    Serial.print("Detected Frequency: ");
    Serial.println(freq);
    Serial.print("Closest String: ");
    Serial.println(names[closest]);

}





