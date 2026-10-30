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

/* Reads a whole number. Returns -1 if the user did not type one. */
int read_number(const char *prompt)
{
	char line[32];
	int number;

	read_line(prompt, line, sizeof(line));
	if (sscanf(line, "%d", &number) != 1)
		return -1;
	return number;
}

/* Returns 1 (and says so) when there are no devices. */
int is_empty(struct Network *n)
{
	if (n->count == 0) {
		printf("No devices registered.\n");
		return 1;
	}
	return 0;
}

char *type_name(int type)
{
	if (type == TEMPERATURE)
		return "Temperature";
	if (type == PRESSURE)
		return "Pressure";
	return "Battery";
}

/* Writes the reading with its unit into text (20 characters at most), using the type's union member. */
void format_value(struct Device *d, char *text)
{
	if (d->status == STATUS_NO_READING)
		snprintf(text, 20, "-");
	else if (d->type == TEMPERATURE)
		snprintf(text, 20, "%.1f C", d->reading.temperature);
	else if (d->type == PRESSURE)
		snprintf(text, 20, "%d kPa", d->reading.pressure);
	else
		snprintf(text, 20, "%.2f V", d->reading.voltage);
}

/* Prints every device by moving a pointer through the array. */
void display_devices(struct Network *n)
{
	char *status_names[4] = {"No reading", "Waiting", "Normal", "Alert"};
	struct Device *p;
	char value[20];

	if (is_empty(n))
		return;
	printf("%-4s %-14s %-12s %-9s %s\n", "ID", "Name", "Type", "Reading", "Status");
	for (p = n->list; p < n->list + n->count; p++) {
		format_value(p, value);
		printf("%-4d %-14s %-12s %-9s %s\n", p->id, p->name, type_name(p->type), value, status_names[p->status]);
	}
	printf("Total devices: %d\n", n->count);
}

/* Moves a pointer through the array. Returns the device, or NULL. */
struct Device *find_device(struct Network *n, int id)
{
	struct Device *p;

	for (p = n->list; p < n->list + n->count; p++) {
		if (p->id == id)
			return p;
	}
	return NULL;
}

void register_device(struct Network *n)
{
	struct Device d;
	struct Device *bigger;

	d.id = read_number("Device id: ");
	if (d.id <= 0) {
		printf("Rejected: the id must be a number above 0.\n");
		return;
	}
	if (find_device(n, d.id) != NULL) {
		printf("Rejected: id %d already exists.\n", d.id);
		return;
	}
	read_line("Name: ", d.name, sizeof(d.name));
	if (strlen(d.name) == 0) {
		printf("Rejected: the name cannot be empty.\n");
		return;
	}
	d.type = read_number("Type 1) Temperature  2) Pressure  3) Battery: ");
	if (d.type < TEMPERATURE || d.type > BATTERY) {
		printf("Rejected: the type must be 1, 2 or 3.\n");
		return;
	}
	memset(&d.reading, 0, sizeof(d.reading));	/* no reading yet */
	d.status = STATUS_NO_READING;
	d.previous = 0;
	d.processed = 0;

	/* Make the array one bigger and put the new device at the end. */
	bigger = realloc(n->list, (n->count + 1) * sizeof(struct Device));
	if (bigger == NULL) {
		printf("Out of memory: device not registered.\n");
		return;	/* the old array is unchanged */
	}
	n->list = bigger;
	n->list[n->count] = d;
	n->count = n->count + 1;
	printf("Registered %s. Devices: %d\n", d.name, n->count);
}

void remove_device(struct Network *n)
{
	struct Device *target;
	struct Device *p;
	struct Device *smaller;
	int id;

	if (is_empty(n))
		return;
	id = read_number("Id to remove: ");
	target = find_device(n, id);
	if (target == NULL) {
		printf("No device with that id.\n");
		return;
	}

	/* Move every device after the removed one down one place. */
	for (p = target; p < n->list + n->count - 1; p++)
		*p = *(p + 1);
	n->count = n->count - 1;

	/* Make the array one smaller, or free it when it is empty. */
	if (n->count == 0) {
		free(n->list);
		n->list = NULL;
	} else {
		smaller = realloc(n->list, n->count * sizeof(struct Device));
		if (smaller != NULL)
			n->list = smaller;	/* if this fails, the old array still works */
	}
	printf("Removed device %d. Devices: %d\n", id, n->count);
}

/*
 * Every device gets one new random reading, written to the union member of its type.
 * rand() % (steps + 1) picks a whole number of steps above the minimum.
 * Option 5 processes them.
 */
void generate_readings(struct Network *n)
{
	struct Device *p;
	int made = 0;

	if (is_empty(n))
		return;
	for (p = n->list; p < n->list + n->count; p++) {
		if (p->status == STATUS_WAITING)
			continue;	/* keep the reading that is not processed yet */
		if (p->type == TEMPERATURE)	/* 10.0 to 50.0 in steps of 0.1 */
			p->reading.temperature = TEMP_SIM_MIN + (rand() % 401) / 10.0f;
		else if (p->type == PRESSURE)	/* 80 to 120 */
			p->reading.pressure = PRESSURE_SIM_MIN + rand() % (PRESSURE_SIM_MAX - PRESSURE_SIM_MIN + 1);
		else				/* 3.00 to 4.40 in steps of 0.01 */
			p->reading.voltage = VOLTAGE_SIM_MIN + (rand() % 141) / 100.0f;
		p->status = STATUS_WAITING;
		made = made + 1;
	}
	if (made == 0)
		printf("Every reading is still waiting. Choose 5 to process them first.\n");
	else
		printf("Generated %d new readings. Choose 5 to process them.\n", made);
}

/* The alert log, kept in memory from realloc. */
struct Alert *alerts = NULL;

int alert_count = 0;

/*
 * The reading as a float, read from the union member that matches the type.
 * Only the member of the device's own type holds a valid value.
 */
float reading_value(struct Device *d)
{
	if (d->type == TEMPERATURE)
		return d->reading.temperature;
	if (d->type == PRESSURE)
		return d->reading.pressure;
	return d->reading.voltage;
}

/* Marks the device as an alert and adds a copy of the details to the log. */
void raise_alert(struct Device *d, char *limit, char *reason, char *callback)
{
	struct Alert *bigger;
	struct Alert *a;

	d->status = STATUS_ALERT;
	bigger = realloc(alerts, (alert_count + 1) * sizeof(struct Alert));
	if (bigger == NULL) {
		printf("Out of memory: alert not logged.\n");
		return;
	}
	alerts = bigger;
	a = &alerts[alert_count];
	a->device_id = d->id;
	strcpy(a->name, d->name);
	a->type = d->type;
	a->value = reading_value(d);
	strcpy(a->limit, limit);
	strcpy(a->reason, reason);
	strcpy(a->callback, callback);
	alert_count = alert_count + 1;
}

/* Callbacks: each one checks one type of reading against its limits. */
void temperature_monitor(struct Device *d)
{
	float t = d->reading.temperature;

	if (t > TEMP_HIGH)
		raise_alert(d, "15.0 to 45.0 C", "too hot", "temperature_monitor");
	else if (t < TEMP_LOW)
		raise_alert(d, "15.0 to 45.0 C", "too cold", "temperature_monitor");
}

void pressure_monitor(struct Device *d)
{
	int p = d->reading.pressure;

	if (p > PRESSURE_HIGH)
		raise_alert(d, "90 to 110 kPa", "too high", "pressure_monitor");
	else if (p < PRESSURE_LOW)
		raise_alert(d, "90 to 110 kPa", "too low", "pressure_monitor");
}

void battery_monitor(struct Device *d)
{
	float v = d->reading.voltage;

	if (v > VOLTAGE_HIGH)
		raise_alert(d, "3.30 to 4.20 V", "overcharged", "battery_monitor");
	else if (v < VOLTAGE_LOW)
		raise_alert(d, "3.30 to 4.20 V", "too low", "battery_monitor");
}

/*
 * Custom callback: Sensor Drift Detector.
 * Compares the new reading with the device's previous one. A jump bigger
 * than the drift limit for its type is an alert, even inside the normal range.
 */
void drift_detector(struct Device *d)
{
	float change;
	float limit[3] = {TEMP_DRIFT, PRESSURE_DRIFT, VOLTAGE_DRIFT};
	char *limit_text[3] = {"change up to 15.0 C", "change up to 15 kPa", "change up to 0.50 V"};

	if (d->processed == 0)
		return;		/* the first reading has nothing to compare with */
	change = reading_value(d) - d->previous;
	if (change < 0)
		change = -change;
	/* + 0.001 so float rounding never turns a change equal to the limit into an alert */
	if (change > limit[d->type - 1] + 0.001f)
		raise_alert(d, limit_text[d->type - 1], "sudden jump", "drift_detector");
}

/* Runs one callback on one device, through a function pointer. */
void process_device(struct Device *device, void (*callback)(struct Device *))
{
	callback(device);
}
