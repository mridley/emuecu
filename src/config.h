#ifndef CONFIG_H
#define CONFIG_H

#ifndef F_CPU
#define F_CPU 			(16000000UL)       	// Mhz
#endif

//#define PWM_MIN    800
#define PWM_MIN    1170
//#define PWM_MAX    2400
#define PWM_MAX    2040
#define PWM_LIMIT  2500
#define RPM_LIMIT  17000
#define DWELL_TIME_MS 2000

#endif
