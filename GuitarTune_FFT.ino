// Assign pins for input (micrphone) and output (LEDs)
const int mic = A0;
const int leftRedLED = 5;
const int greenLED = 4;
const int rightRedLED = 3;

// Initializing microphone calibration parameters
// MIC_OFFSET: Center / Baseline of signal oscillation
// VOL_THRESHOLD: Baseline volume to begin analyzing frequency
const int MIC_OFFSET = 335; 
const int VOL_THRESHOLD = 30;  

// Initializing timing parameters
// IDLE_TIME: Time (ms) before LEDs turn off due to no note detected
// PLUCK_DELAY: Short delay (ms) after pluck for vibrations to normalize before signal is analyzed
const int IDLE_TIME = 3000;
const int PLUCK_DELAY = 550;

// Margin of error allowed for a string to be deemed "in tune"
const double TOLERANCE = 3;

#include <arduinoFFT.h>

// Initializing expected frequencies and names for "in tune" guitar strings
// *currently designed only for standard tuning (EADGBE)
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


void setup() 
{
  Serial.begin(115200);

  // Establish LED pins as outputs
  pinMode(leftRedLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(rightRedLED, OUTPUT);
}

// Initializing FFT parameters
// Larger number of samples improves accuracy, but takes longer to record and process
const uint16_t samples = 512;
const double samplingFrequency = 2000;

double vReal[samples];
double vImag[samples];

// Initialize FFT object for conversion of raw data to frequency information
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

  // Determine plucked guitar string based on closest standard frequency
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


    // Block unrealistic/invalid frequency recordings
    if (freq > 3000 || freq < 20) 
    {
        freq = 0;
    }

    // Calculate error of recorded frequency compared to ideal frequency of a note
    double error = freq - notes[closest];
    if(error < -TOLERANCE)
    {
        // Frequency is lower than ideal (flat)
        // Left red LED is activated
        digitalWrite(leftRedLED, HIGH);
        digitalWrite(greenLED, LOW);
        digitalWrite(rightRedLED, LOW);
    }
    else if(error > TOLERANCE)
    {
        // Frequency is higher than ideal (sharp)
        // Right red LED is activated
        digitalWrite(leftRedLED, LOW);
        digitalWrite(greenLED, LOW);
        digitalWrite(rightRedLED, HIGH);
    }
    else
    {
        // Frequency is within tolerance of ideal value (in tune)
        // Green LED is activated
        digitalWrite(leftRedLED, LOW);
        digitalWrite(greenLED, HIGH);
        digitalWrite(rightRedLED, LOW);
    }

    // Outputs detected frequency and identified string (based on recorded vs nominal frequency values)
    Serial.print("Detected Frequency: ");
    Serial.println(freq);
    Serial.print("Closest String: ");
    Serial.println(names[closest]);

}





