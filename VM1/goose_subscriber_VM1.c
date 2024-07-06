/*
 * goose_subscriber_example.c
 *
 * This is an example for a standalone GOOSE subscriber
 *
 * Has to be started as root in Linux.
 */

#include "goose_receiver.h"
#include "goose_subscriber.h"
#include "hal_thread.h"
#include "linked_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

static int running = 1;

static void
sigint_handler(int signalId)
{
    running = 0;
}

static void
gooseListener(GooseSubscriber subscriber, void* parameter)
{
    printf("GOOSE event:\n");
    printf("  stNum: %u sqNum: %u\n", GooseSubscriber_getStNum(subscriber),
            GooseSubscriber_getSqNum(subscriber));
    printf("  timeToLive: %u\n", GooseSubscriber_getTimeAllowedToLive(subscriber));

    uint64_t timestamp = GooseSubscriber_getTimestamp(subscriber);

    printf("  timestamp: %u.%u\n", (uint32_t) (timestamp / 1000), (uint32_t) (timestamp % 1000));
    printf("  message is %s\n", GooseSubscriber_isValid(subscriber) ? "valid" : "INVALID");

    MmsValue* values = GooseSubscriber_getDataSetValues(subscriber);

    char buffer[1024];

    MmsValue_printToBuffer(values, buffer, 1024);

    printf("  allData: %s\n", buffer);
/* 
CSV Function [begin]
*/
	//Store the GOOSE message data into a file //
	// Declare a file pointer and an integer variable i
	FILE *fp;
    int i;
    
    // Declare an integer array named CBstval with 4 elements, all initialized to 0.
    int CBstval [4] = {0,0,0,0};
       
    // Iterate through the buffer and assign values to the CBstval array
    for (i=0; i<4; i++) {
    CBstval [i] = buffer [(i*2+1)] - 48;
    }
	    // Open the file in write mode and store its pointer in fp
    	fp=fopen("TripFromPRCB.csv","w");
    	
	    // Check if the file was opened successfully
    	if(fp==NULL) {
        printf("File cannot open! " );
        exit(0);
     	}
	 
	// Write values from CBstval array to the file
	for (i=0; i<4; i++) {
	fprintf(fp,"%d,", CBstval[i]);
	}
	     
    //Close file//
    	fclose(fp); 	
/* 
CSV Function [end]
*/
}

int
main(int argc, char** argv)
{
    GooseReceiver receiver = GooseReceiver_create();

    if (argc > 1) {
        printf("Set interface id: %s\n", argv[1]);
        GooseReceiver_setInterfaceId(receiver, argv[1]);
    }
    else {
        printf("Using interface enp0s3\n");
        GooseReceiver_setInterfaceId(receiver, "enp0s3");
    }

    GooseSubscriber subscriber = GooseSubscriber_create("OpenPLC/PR$CB$CBStaval", NULL);

/* OPTOUT
    uint8_t dstMac[6] = {0x01,0x0c,0xcd,0x01,0x00,0x01};
    GooseSubscriber_setDstMac(subscriber, dstMac);
*/
    GooseSubscriber_setAppId(subscriber, 0x8004);

    GooseSubscriber_setListener(subscriber, gooseListener, NULL);

    GooseReceiver_addSubscriber(receiver, subscriber);

    GooseReceiver_start(receiver);

    if (GooseReceiver_isRunning(receiver)) {
        signal(SIGINT, sigint_handler);

        while (running) {
            Thread_sleep(100);
        }
    }
    else {
        printf("Failed to start GOOSE subscriber. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
    }

    GooseReceiver_stop(receiver);

    GooseReceiver_destroy(receiver);

    return 0;
}
