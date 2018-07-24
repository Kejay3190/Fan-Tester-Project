

// INI the Variables
String sCommand   = "H00";
String sFrequency = "H";
String sDutyCycle = "00";
int delimIndex = 0;
int nOutputPin = 9;
int nLED = 13;
int nFrequency = 100;
long lDutyCycle = 100;
float fDutyCycle = 1;
float fCycleTime = 10000;
float fDelayOn  = 10000;
float fDelayOff = 0;

// Setup 
void setup() 
{
  pinMode(nOutputPin, OUTPUT);
  pinMode(nLED, OUTPUT);
  Serial.begin(115200);
}

// Main Loop...
void loop() 
{
  
  // If a new command is given switch the timers.
  if (Serial.available())
  {
    readNewCommand();
  }

  if (nFrequency == 10)
  {
  
    // Send a pulse.
    digitalWrite(nOutputPin, HIGH);
    digitalWrite(nLED, HIGH);
    delay(fDelayOn);
    digitalWrite(nOutputPin, LOW);
    digitalWrite(nLED, LOW);
    delay(fDelayOff);

  }
  else
  {

    // Send a pulse.
    digitalWrite(nOutputPin, HIGH);
    digitalWrite(nLED, HIGH);
    delayMicroseconds(fDelayOn);
    digitalWrite(nOutputPin, LOW);
    digitalWrite(nLED, LOW);
    delayMicroseconds(fDelayOff);

  }
}

// When the system needs to read the new command from the port.
void readNewCommand()
{

  // Read the new command
  sCommand = Serial.readStringUntil('\n');
  delimIndex = sCommand.indexOf('\n');
  sFrequency = sCommand.substring(0,1);
  sDutyCycle = sCommand.substring(1,delimIndex - 1);
  
  //Determine the frequency
  if (sFrequency == "T")
  {
    nFrequency = 10;
  }
  if (sFrequency == "H")
  {
    nFrequency = 100;
  }
  if (sFrequency == "I")
  {
    nFrequency = 140;
  }
 
  
  // Determine the duty cycle
  lDutyCycle = sDutyCycle.toInt();
  if (lDutyCycle == 0)
  {
    lDutyCycle = 100;
  }

  // Calculate the duty cycle
  fDutyCycle = (float)lDutyCycle / (float)100;

  // Determine the cycle on and off times
  if (nFrequency == 10)
  {
    fCycleTime = 100;
    fDelayOff  = fCycleTime * fDutyCycle;
    fDelayOn = fCycleTime - fDelayOff;
  }
  if (nFrequency == 100)
  {
    fCycleTime = 10000 - (float)90;
    fDelayOn  = fCycleTime * fDutyCycle;
    fDelayOff = fCycleTime - fDelayOn;  
  }
  if (nFrequency == 140)
  {
    fCycleTime = 7142;
    fDelayOn = fCycleTime * fDutyCycle;
    fDelayOff = fCycleTime - fDelayOn;
  } 
}

