/*
* Vous n'avez pas besoin de lire/comprendre ce fichier.
* 
*	Sources :
*		libserialport examples
*/


#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoComP1DEFI.h"
#include "libserialport.h"

// Global variable :( for error handling
int abortCurrentOperation = 0;

typedef struct ArduinoComData {
	char portName[20];
	struct sp_port* port;
	struct sp_port_config* config;
	int baudrate;
	int ok;
	char* receiveBuffer;
	char* temporaryReceiveBuffer;
	int bufferSize;
	int posInTempBuffer;
} ArduinoComData;

/* Helper function to give a name for each parity mode. */
const char* parity_name(enum sp_parity parity)
{
	switch (parity) {
	case SP_PARITY_INVALID:
		return "(Invalid)";
	case SP_PARITY_NONE:
		return "None";
	case SP_PARITY_ODD:
		return "Odd";
	case SP_PARITY_EVEN:
		return "Even";
	case SP_PARITY_MARK:
		return "Mark";
	case SP_PARITY_SPACE:
		return "Space";
	default:
		return NULL;
	}
}

/* Helper function for error handling. */
int check(enum sp_return result)
{
	/* For this example we'll just exit on any error by calling abort(). */
	char* error_message;

	switch (result) {
	case SP_ERR_ARG:
		printf("Error: Invalid argument.\n");
		abortCurrentOperation = 1;
		return -1;
	case SP_ERR_FAIL:
		error_message = sp_last_error_message();
		printf("Error: Failed: %s\n", error_message);
		sp_free_error_message(error_message);
		abortCurrentOperation = 1;
		return -1;
	case SP_ERR_SUPP:
		printf("Error: Not supported.\n");
		abortCurrentOperation = 1;
		return -1;
	case SP_ERR_MEM:
		printf("Error: Couldn't allocate memory.\n");
		abortCurrentOperation = 1;
		return -1;
	case SP_OK:
	default:
		return result;
	}
}


/* Helper function for error signaling */
ArduinoComData errorDemarrageConnection(ArduinoComData aCom) {
	aCom.ok = 0;
	printf("Erreur lors du démarrage de la connection\n");
	return aCom;
}

/* Print the config state of the port */
void printConfigState(ArduinoComData aCom) {
	int baudrate, bits, stopbits;
	enum sp_parity parity;
	check(sp_get_config_baudrate(aCom.config, &baudrate));
	check(sp_get_config_bits(aCom.config, &bits));
	check(sp_get_config_stopbits(aCom.config, &stopbits));
	check(sp_get_config_parity(aCom.config, &parity));
	printf("Baudrate: %d, data bits: %d, parity: %s, stop bits: %d\n",
		baudrate, bits, parity_name(parity), stopbits);
}

/* Given the information of the connection, open it if it is shutdown */
ArduinoComData restoreArduinoConnection(ArduinoComData aCom) {
	abortCurrentOperation = 0;
	if (aCom.ok) return aCom;

	if (aCom.port) {
		sp_free_port(aCom.port); 
		aCom.port = NULL;
	}
	if (aCom.config) {
		sp_free_config(aCom.config);
		aCom.config = NULL;
	}

	printf("Looking for port %s.\n", aCom.portName);

	/* Call sp_get_port_by_name() to find the port. The port
	 * pointer will be updated to refer to the port found. */
	check(sp_get_port_by_name(aCom.portName, &aCom.port));

	if (abortCurrentOperation) return errorDemarrageConnection(aCom);

	/* Display some basic information about the port. */
	printf("Port name: %s\n", sp_get_port_name(aCom.port));
	printf("Description: %s\n", sp_get_port_description(aCom.port));

	/* The port must be open to access its configuration. */
	printf("Opening port.\n");
	check(sp_open(aCom.port, SP_MODE_READ_WRITE));

	if (abortCurrentOperation) return errorDemarrageConnection(aCom);

	check(sp_set_baudrate(aCom.port, aCom.baudrate));
	check(sp_set_bits(aCom.port, 8));
	check(sp_set_parity(aCom.port, SP_PARITY_NONE));
	check(sp_set_stopbits(aCom.port, 1));
	check(sp_set_flowcontrol(aCom.port, SP_FLOWCONTROL_NONE));

	if (abortCurrentOperation) return errorDemarrageConnection(aCom);

	check(sp_new_config(&aCom.config));

	if (abortCurrentOperation) return errorDemarrageConnection(aCom);

	/* Read the current config from the port into that configuration. */
	check(sp_get_config(aCom.port, aCom.config));

	if (abortCurrentOperation) return errorDemarrageConnection(aCom);

	/* Display some of the settings read back from the port. */
	printConfigState(aCom);

	if (abortCurrentOperation) return errorDemarrageConnection(aCom);

	aCom.ok = 1;
	return aCom;
}

/* Create the connection object and open the connection */
SerialCom startArduinoConnection(const char* portName, int baudrate) {
	ArduinoComData aCom;
	strcpy(aCom.portName, portName);
	aCom.port = NULL;
	aCom.config = NULL;
	aCom.baudrate = baudrate;
	aCom.ok = 0;
	aCom.temporaryReceiveBuffer = malloc(200 * sizeof(char));
	aCom.receiveBuffer = malloc(200 * sizeof(char));
	aCom.bufferSize = 200;
	aCom.posInTempBuffer = 0;

	SerialCom sCom;
	sCom.p_aCom = malloc(sizeof(ArduinoComData));

	if (sCom.p_aCom) {
		*sCom.p_aCom = restoreArduinoConnection(aCom);
	}
	else {
		printf("Erreur allocation mémoire !\n");
		abort();
	}

	return sCom;
}

/* Close the connection */
int closeArduinoConnection(SerialCom sCom) {
	if (!sCom.p_aCom) return 0;

	if (sCom.p_aCom->port) {
		if (check(sp_close(sCom.p_aCom->port)) == -1) {
			printf("Failure to close the port\n");
		}
		sp_free_port(sCom.p_aCom->port);
		sCom.p_aCom->port = NULL;
	}
	if (sCom.p_aCom->config) {
		sp_free_config(sCom.p_aCom->config);
		sCom.p_aCom->config = NULL;
	}

	free(sCom.p_aCom->receiveBuffer);
	free(sCom.p_aCom->temporaryReceiveBuffer);

	free(sCom.p_aCom);
	return 0;
}

int checkAndRestoreConnectionIfDown(ArduinoComData* aCom) {
	if (!aCom->ok) {
		*aCom = restoreArduinoConnection(*aCom);
		if (!aCom->ok) return -1;
	}
	return 0;
}

int receiveMsgHelper(ArduinoComData* p_aCom, char start, char end) {
	if (checkAndRestoreConnectionIfDown(p_aCom) == -1) return -1;

	/* We'll allow a 3ms second timeout for send and receive. */
	unsigned int timeout = 3;
	unsigned int chunkReadSize = 1;

	

	while (1) {
		/* Try to receive the data on the other port. */
		char readingResult;
		/* On success, sp_blocking_write() and sp_blocking_read()
		 * return the number of bytes sent/received before the
		 * timeout expired. */
		int nbBytesRead = check(sp_blocking_read(p_aCom->port, &readingResult, chunkReadSize, timeout));

		/* Check whether we received the number of bytes we wanted. */
		if (nbBytesRead != chunkReadSize) {
			return nbBytesRead;
		}

		if (p_aCom->posInTempBuffer + 1 >= p_aCom->bufferSize) p_aCom->posInTempBuffer = 0;

		if (readingResult == start) {
			p_aCom->posInTempBuffer = 0;
		}

		p_aCom->temporaryReceiveBuffer[p_aCom->posInTempBuffer++] = readingResult;

		if (readingResult == end) {
			for (int i = 0; i < p_aCom->posInTempBuffer; i++) {
				p_aCom->receiveBuffer[i] = p_aCom->temporaryReceiveBuffer[i];
			}
			p_aCom->receiveBuffer[p_aCom->posInTempBuffer] = 0;
			return p_aCom->posInTempBuffer;
		}

	}
}

char* arduinoReceiveMsg(SerialCom sCom, char start, char end) {
	int status = receiveMsgHelper(sCom.p_aCom, start, end);
	if (status > 0) {
		return sCom.p_aCom->receiveBuffer;
	}
	else if (status == 0) {
		return NULL;
	}
	else {
		sCom.p_aCom->ok = 0;
		return NULL;
	}
}

char* arduinoReceiveLatestMsg(SerialCom sCom, char start, char end) {
	int status = 0;
	int atLeastOneMsgReceived = 0;

	do {
		status = receiveMsgHelper(sCom.p_aCom, start, end);
		//printf("%s", sCom.p_aCom->receiveBuffer);
		if (status > 0) {
			atLeastOneMsgReceived = 1;
		}
		else if (status == -1) {
			sCom.p_aCom->ok = 0;
			return NULL;
		}
	} while (status > 0);

	if (atLeastOneMsgReceived) {
		return sCom.p_aCom->receiveBuffer;
	}

	return NULL;
}

int arduinoSendMsg(SerialCom sCom, char* msg) {
	if (checkAndRestoreConnectionIfDown(sCom.p_aCom) == -1) return -1;

	unsigned int timeout = 3;

	size_t msgLength = strlen(msg);
	int nbBytesSent = check(sp_blocking_write(sCom.p_aCom->port, msg, msgLength, timeout));

	if (nbBytesSent != msgLength) return -1;

	return 0;
}