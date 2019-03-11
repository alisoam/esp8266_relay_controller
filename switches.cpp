bool switches[SWITCHES_NUMBER] = {false};
const uint8_t switchOut[SWITCHES_NUMBER] = {15, 10, 12, 16};
const uint8_t switchFeedbackIn[SWITCHES_NUMBER] = {0, 2, 13, 14};

void setupSwitch()
{
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
  {
    digitalWrite(switchOut[index], LOW);
    pinMode(switchOut[index], OUTPUT);
  }
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
  {
    pinMode(switchFeedbackIn[index], INPUT);
  }
}


void switchSet()
{
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
    digitalWrite(switchOut[index], switches[index]);
}

void switchStatusCheck()
{
  switchSet();
}