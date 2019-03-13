#ifndef SWITCHES_HPP
#define SWITCHES_HPP

#include <stdbool.h>

#define SWITCHES_NUMBER 4

void setupSwitch();
void switchSet(unsigned int index, bool status);
bool switchGet(unsigned int index);
void switchStatusCheck();
void switchesLoop();

#endif
