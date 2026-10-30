/*
 * Dynamic IoT Event Monitoring and Callback Engine
 * Student ID: 993
 */
#include <time.h>
#include "iot_monitor.h"

/* My devices: 2 of each type to start with. {0} means no reading yet. */
struct Device seed[] = {
	{101, "Server Room", TEMPERATURE, {0}, STATUS_NO_READING, 0, 0},
	{102, "Boiler Room", TEMPERATURE, {0}, STATUS_NO_READING, 0, 0},
	{201, "Water Pump", PRESSURE, {0}, STATUS_NO_READING, 0, 0},
	{202, "Air Tank", PRESSURE, {0}, STATUS_NO_READING, 0, 0},
	{301, "Gate Sensor", BATTERY, {0}, STATUS_NO_READING, 0, 0},
	{302, "Door Lock", BATTERY, {0}, STATUS_NO_READING, 0, 0}
};

int main(void)
{
	/* Menu choice 1 runs actions[0], choice 2 runs actions[1], and so on. */
	MenuAction actions[7] = {
		register_device, display_devices, remove_device, generate_readings,
		process_readings, display_alerts, monitoring_summary
	};
	struct Network n = {NULL, 0, 0, 0, 0};
	int choice;

	srand(time(NULL));	/* different random readings on every run */

	n.count = 6;
	n.list = malloc(n.count * sizeof(struct Device));
	if (n.list == NULL) {
		printf("Out of memory.\n");
		return 1;
	}
	memcpy(n.list, seed, n.count * sizeof(struct Device));

	printf("Student ID: %s\n", STUDENT_ID);
	printf("Project: Dynamic IoT Event Monitoring and Callback Engine\n");
	printf("Custom Callback: Sensor Drift Detector\n");

	while (1) {
		printf("\n1.Register  2.Display  3.Remove  4.Generate readings\n");
		printf("5.Process readings  6.Alerts  7.Summary  8.Exit\n");
		choice = read_number("Choice: ");
		if (choice == 8 || feof(stdin))
			break;
		if (choice >= 1 && choice <= 7)
			actions[choice - 1](&n);
		else
			printf("Invalid choice. Enter 1 to 8.\n");
	}

	free(n.list);
	free_alerts();
	printf("Memory freed. Goodbye.\n");
	return 0;
}
