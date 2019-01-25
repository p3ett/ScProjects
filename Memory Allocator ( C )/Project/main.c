#include "hashTable.h"
#include "functions.h"

int main(int argc, char* argv[]) {
	int i;
	size_t command;

	printf("Give the bytes you want to allocate: ");
	scanf("%zu",&command);
	printf("Size will be included in class of %zu bytes.\n",myround(command));
	
	/* test 1 */
	printf("\nTest 1: Adresses of %zu bytes.\n",myround(command));

	uintptr_t q1[command];
	for(i = 0; i < 4096/command; i++) {
		q1[i] = (uintptr_t) mymalloc(command);
		
		if( i == 0 )
            printf("%i : %lu \n", i, q1[i]);
        else
			printf("%i : %lu   diff = %d\n", i, q1[i], abs(q1[i] - q1[i - 1]));
	}

	printf("Free the table.\n");
	for(i = 0; i < 4096/command; i++)
		myfree((void*) q1[i]);
	/* ----------------------------------------------------------------*/
	
	fgetc(stdin);
	printf("\nHit enter for the next test.");
	while(true)
		if( getc(stdin) == '\n')
			break;

	/* test 2 */
	printf("\nTest 2: Take the same address after myfree.\n");\
	
	printf("Give the bytes you want to allocate: ");
	scanf("%zu",&command);
	printf("Size will be included in class of %zu bytes.\n\n",myround(command));

	uintptr_t q = (uintptr_t) mymalloc(command);
	printf("The address before myfree %lu \n", q);

	myfree((void*) q);
	
	uintptr_t d = (uintptr_t) mymalloc(command);
	printf("The address after myfree %lu \n", q);
	
	printf("Free the table.\n");
	myfree((void*) d);
	/* --------------------------------------------*/
	
	fgetc(stdin);
	printf("\nHit enter for the next test.");
	while(true)
		if( fgetc(stdin) == '\n')
			break;

	/* test3 */
	printf("\nTest 3: Allocate a new MB.\n");
	uintptr_t q2[257];
	int flag = false;

	for(i = 0; i < 257; i++) {
		q2[i] = (uintptr_t) mymalloc(4096);
		
		if( i < 3 )
			if( i == 0 )
        	    printf("%i : %lu \n", i, q2[i]);
        	else
				printf("%i : %lu   diff = %d\n", i, q2[i], abs(q2[i] - q2[i - 1]));
		else if( abs(q2[i] - q2[i - 1]) > 4096) {
			printf(".\n.\n.\n");
			printf("%i : %lu   diff = %d\n", i-1, q2[i-1], abs(q2[i-1] - q2[i - 2])); //print the last before new allocation
			
			printf("\nAllocate a new MB\n");
			printf("%i : %lu   diff = %d\n", i, q2[i], abs(q2[i] - q2[i - 1]));

			flag = true;
		}
		else if( flag == true )
			printf("%i : %lu   diff = %d\n", i, q2[i], abs(q2[i] - q2[i - 1]));
	}	
	printf("Free the table.\n");
	for(i = 0; i < 257; i++)
		myfree((void*) q2[i]);
	/* --------------------------------------------------------- */

	printf("\nHit enter for the next test.");
	while(true)
		if( fgetc(stdin) == '\n')
			break;

	/* test 4 */
	printf("\nTest 4: Multiple mymallocs\n");

	uintptr_t q3[24];
	int power = 5;

	for(i = 0; i < 24; i++) {
		if( power == 13 )
			power = 5;

		q3[i] = (uintptr_t) mymalloc(myPow(2,power));
		printf("%i : %lu   bytes = %d\n", i, q3[i], myPow(2,power));
	
		power++;
	}	

	printf("Free the table.\n");
	for(i = 0; i < 24; i++)
		myfree((void*) q3[i]);
	/* ------------------------------------------------------ */

    return 0;
}
