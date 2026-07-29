const int mic = A0;
const int leftRedLED = 5;
const int greenLED = 4;
const int rightRedLED = 3;


int lowVol = 200;
int midVol = 400;
int highVol = 650;


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
