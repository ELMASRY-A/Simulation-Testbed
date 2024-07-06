/*
 * goose_publisher_example.c
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "mms_value.h"
#include "goose_publisher.h"
#include "hal_thread.h"

/* has to be executed as root! */
int
main(int argc, char **argv)
{
    char *interface;

/* GOOSE DATA [begin] */
    int x = 0;
    int CBstval [4] = {0,0,0,0};
    int OriCBstval [4] = {0,0,0,0};
    int PreCBstval [4] = {0,0,0,0};

	double power(int x, int y) {
	    int result = 1;
	    for (int i=0; i<y; i++)
	    	result=result*x;
	    	return result;
	}
/* GOOSE DATA [end] */
    if (argc > 1)
        interface = argv[1];
    else
        interface = "enp0s3";

    printf("Using interface %s\n", interface);
/* OPT-OUT

    LinkedList dataSetValues = LinkedList_create();

    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
    LinkedList_add(dataSetValues, MmsValue_newBinaryTime(false));
    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

*/
    CommParameters gooseCommParameters;

    gooseCommParameters.appId = 0x8004;
    gooseCommParameters.dstAddress[0] = 0x01;
    gooseCommParameters.dstAddress[1] = 0x0c;
    gooseCommParameters.dstAddress[2] = 0xcd;
    gooseCommParameters.dstAddress[3] = 0x01;
    gooseCommParameters.dstAddress[4] = 0x00;
    gooseCommParameters.dstAddress[5] = 0x01;
    gooseCommParameters.vlanId = 0;
    gooseCommParameters.vlanPriority = 4;

    /*
     * Create a new GOOSE publisher instance. As the second parameter the interface
     * name can be provided (e.g. "eth0" on a Linux system). If the second parameter
     * is NULL the interface name as defined with CONFIG_ETHERNET_INTERFACE_ID in
     * stack_config.h is used.
     */
    GoosePublisher publisher = GoosePublisher_create(&gooseCommParameters, interface);

    if (publisher) {
        GoosePublisher_setGoCbRef(publisher, "OpenPLC/PR$CB$CBStaval");
        GoosePublisher_setConfRev(publisher, 1);
        GoosePublisher_setDataSetRef(publisher, "OpenPLC/PRCB$CBStaval");
        GoosePublisher_setTimeAllowedToLive(publisher, 2000);
        GoosePublisher_setGoID(publisher , "PRCB_CBStaval");

        int i = 0;

/* READ CSV and SEND GOOSE [begin] */
	    while (1) 
		{
	    	LinkedList dataSetValues = LinkedList_create();
	    	
	    	FILE *fp;
	    	int i;
 
			//Open File//
	    	fp=fopen("TripFromPLC.csv","r");
	    	if(fp==NULL) 
	    	{
				printf("File cannot open! " );
				exit(0);
	     	}
			//Read values from file//
			for (i=0; i<4; i++)
			{
				fscanf(fp,"%d,", &CBstval[i]);
				//Debug//
				//printf("%d\n",CBstval[i]);
			}
			//Close file//
				fclose(fp);
																	
			for (int i = 0; i <4; i++ ) 
				LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(CBstval[i]));

				
			if (memcmp(PreCBstval,CBstval,sizeof(CBstval)) != 0) 
			{
				GoosePublisher_increaseStNum(publisher);
				memcpy(PreCBstval,CBstval,sizeof(CBstval));
			}
			
			   
			if (memcmp(PreCBstval,OriCBstval,sizeof(PreCBstval)) == 0) 
			{
				Thread_sleep(1000);
				x = 0;
			}
			else 
			{
				if (x < 8) 
				{
					Thread_sleep(4*power(2,x));
					x++;
				}
				else 
				{
					Thread_sleep(1000);
				}		    
			}
/* READ CSV and SEND GOOSE [end] */

/* OPT-OUT
        for (i = 0; i < 4; i++) {
            Thread_sleep(1000);

            if (i == 3) {
                /* now change dataset to send an invalid GOOSE message */
*/
/* OPT-OUT           
				LinkedList_add(dataSetValues, MmsValue_newBoolean(true));
                GoosePublisher_publish(publisher, dataSetValues);
            }
            else {
*/
                if (GoosePublisher_publish(publisher, dataSetValues) == -1) {
                    printf("Error sending message!\n");
                }
/* OPT-OUT
            }
        }
*/

		LinkedList_destroyDeep(dataSetValues, (LinkedListValueDeleteFunction) MmsValue_delete);

	   }
	   
        GoosePublisher_destroy(publisher);
    }
    else {
        printf("Failed to create GOOSE publisher. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
    }

/* OPT-OUT
    LinkedList_destroyDeep(dataSetValues, (LinkedListValueDeleteFunction) MmsValue_delete);
*/

    return 0;
}




