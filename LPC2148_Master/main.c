#include "system.h"
#include "LPC214X.h"
#include <stdio.h>
#include "demo.h"
#include "serial.h"
#include "delay.h"
#include "i2c.h"
#include "irq.h"
#include <string.h>
int main(void)
{
	i2c_eeprom_test();
}
