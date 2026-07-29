
const int mic = A0;
const int leftRedLED = 5;
const int greenLED = 4;
const int rightRedLED = 3;

int lowVol = 300;
int midVol = 480;
int highVol = 650;



const int micPin = A0;

void setup() {
  Serial.begin(115200);
}

int previous = 0;
unsigned long prevTime = 0;
float frequencies[5];
int frequencyCount = 0;

void loop() {
  int sample = analogRead(micPin) - 335;
  if (previous < -20 && sample > 20)
  {
    unsigned long time = micros();
    if (prevTime != 0)
    {
      unsigned long period = time - prevTime;
      if (period > 100000)   // 100 ms
      {
          frequencyCount = 0;
      }
      float frequency = 1000000.0 / period;
      Serial.println(period);
      if (frequency > 70 && frequency < 350 && frequencyCount < 5)
      {
        frequencies[frequencyCount] = frequency;
        frequencyCount++;
        if (frequencyCount == 5)
        {
          for (int i = 0; i < 4; i++)
          {
              for (int j = 0; j < 4 - i; j++)
              {
                  if (frequencies[j] > frequencies[j + 1])
                  {
                      float temp = frequencies[j];
                      frequencies[j] = frequencies[j + 1];
                      frequencies[j + 1] = temp;
                  }
              }
          }
          float medianFrequency = frequencies[2];
          frequencyCount = 0;
          Serial.print("Frequency: ");
          Serial.println(medianFrequency);
        }
      }
     }
    prevTime = time;
  }
  previous = sample;
}


void setup() 
{
  Serial.begin(9600);

  pinMode(leftRedLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(rightRedLED, OUTPUT);

}

void loop() 
{
  int soundValue = analogRead(mic);
  Serial.println(soundValue);
  digitalWrite(leftRedLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(rightRedLED, LOW);

  if (soundValue > lowVol)
  {
    digitalWrite(leftRedLED, HIGH);
  }

  if (soundValue > midVol)
  {
    digitalWrite(leftRedLED, HIGH);
    digitalWrite(greenLED, HIGH);
  }

  if (soundValue > highVol) 
  {
    digitalWrite(leftRedLED, HIGH);
    digitalWrite(rightRedLED, HIGH);
    digitalWrite(greenLED, HIGH);
  }
}





