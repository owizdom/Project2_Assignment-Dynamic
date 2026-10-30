#ifndef IOT_MONITOR_H
#define IOT_MONITOR_H

/* The structs, the alert list and every function the program uses, except main. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_ID "993"

/* Device types */
#define TEMPERATURE 1
#define PRESSURE 2
#define BATTERY 3

/* Device status */
#define STATUS_NO_READING 0
#define STATUS_WAITING 1	/* has a new reading that is not processed yet */
#define STATUS_NORMAL 2
#define STATUS_ALERT 3

/* Normal operating limits. A reading outside them is an alert. */
#define TEMP_LOW 15.0f		/* C */
#define TEMP_HIGH 45.0f
#define PRESSURE_LOW 90		/* kPa */
#define PRESSURE_HIGH 110
#define VOLTAGE_LOW 3.3f	/* V */
#define VOLTAGE_HIGH 4.2f

/* Simulated readings are random values in these ranges. */
#define TEMP_SIM_MIN 10.0f
#define TEMP_SIM_MAX 50.0f
#define PRESSURE_SIM_MIN 80
#define PRESSURE_SIM_MAX 120
#define VOLTAGE_SIM_MIN 3.0f
#define VOLTAGE_SIM_MAX 4.4f

/* Drift detector: the biggest normal change between two readings. */
#define TEMP_DRIFT 15.0f
#define PRESSURE_DRIFT 15
#define VOLTAGE_DRIFT 0.5f

struct Device {
	int id;
	char name[40];
	int type;
	union {
		float temperature;	/* used when type is TEMPERATURE */
		int pressure;		/* used when type is PRESSURE */
		float voltage;		/* used when type is BATTERY */
	} reading;
	int status;
	float previous;		/* the last processed reading, for the drift detector */
	int processed;		/* how many readings this device has had processed */
};

/* All the devices, kept in memory from malloc, and the reading counts. */
struct Network {
	struct Device *list;
	int count;
	int readings;
	int normal;
	int abnormal;
};

/* One alert. It keeps copies, so it stays valid after the device is removed. */
struct Alert {
	int device_id;
	char name[40];
	int type;
	float value;
	char limit[30];
	char reason[30];
	char callback[24];
};

/* A menu option: a function that takes the network. */
typedef void (*MenuAction)(struct Network *n);

/* A callback: a function that checks one device's reading. */
typedef void (*Callback)(struct Device *d);

/*
 * Shows a prompt and reads one line of text.
 * fgets stops at size - 1 characters, so the text never overflows.
 */
void read_line(const char *prompt, char *text, int size)
{
	int length;
	int c;

	printf("%s", prompt);
	fflush(stdout);	/* show the prompt now, before waiting for input */
	if (fgets(text, size, stdin) == NULL) {
		text[0] = '\0';		/* no more input */
		return;
	}
	length = strlen(text);
	if (length > 0 && text[length - 1] == '\n') {
		text[length - 1] = '\0';	/* remove the Enter key */
	} else {
		/* The line was too long: skip the rest of it. */
		c = getchar();
		while (c != '\n' && c != EOF)
			c = getchar();
	}
}
