// Programming Project 4
// Project: For this project you develop an application related to avoiding deadlock. Use either
// C / C++ or Java, on any platform(Linux, macOS, Windows, etc.
// Students: Nikolaos Grammatopoulos, Fawzi Nastas
// Section: COMP - 354 (Section 01) Operating Systems

#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>

#define customer_number 5  //defining customer_number
#define resource_number 3   //defining resource number

//initializing variables
int need[customer_number][resource_number]; // resource that needs
int available_metrices[resource_number]; //available metrics
int max_metrices[customer_number][resource_number]; // maximum metrics
int total[resource_number]={0}; // total number of resources
int available[resource_number]; // available resources
int maximum[customer_number][resource_number]; // maximum resources
int allocation[customer_number][resource_number]={0}; // allocated resources
int allocated[customer_number][resource_number];
int needed[customer_number][resource_number];
int Finish[customer_number]={0};

void *thread_func(void* customer_numt);
int bankerAlgorithm(int customer_num,int request[]); // initializing bankerAlgorithm
int release_resources(int customer_num); // release resources
int request_resources(int customer_num, int request[]); // request resources
void printingState(); // prinnt func
pthread_mutex_t mutex;
char string[resource_number*2]={0};
int safeSequence[customer_number]={0}; // safe sequence

int main(int argc, const char * argv[]) {

  // Matrices initialization

int i=0 ;
 while (i<argc-1) {
      available[i]=atoi(argv[i+1]);
        total[i]=
            available[i];  // At start resources available = to total resources
    ++i;
  }
    for(int i=0;i< resource_number;i++){
        for(int j=0;j<customer_number;j++){
            maximum[j][i]=rand()%(total[i]+1);  //Max must be less from total
            need[j][i]=maximum[j][i];    // Need = max-allocation
        }
    }
 while (i<resource_number) {
       string[i*2]=i+'A';
        string[i*2+1]=' ';
    ++i;
  }

    printf("System resources in total:\n");
    printf("%s\n",string);
   while (i<resource_number) {
      printf("%d ",total[i]);
    ++i;
  }

    printf("\n\nProcesses utilizing max resources:\n");
    printf("   %s\n",string);
    for(int i=0; i<customer_number;i++){
        printf("P%d ",i+1);
        for(int j=0;j<resource_number;j++){
            printf("%d ",maximum[i][j]);
        }
        printf("\n");
    }

    printingState();
   // Print state of process

   pthread_mutex_init(&mutex, NULL);    // Initialize mutex
   pthread_t p1,p2,p3,p4,p5;

   int a=0,b=1,c=2,d=3,e=4;

//creating threads

   pthread_create(&p1,NULL,thread_func,&a);
   pthread_create(&p2,NULL,thread_func,&b);
   pthread_create(&p3,NULL,thread_func,&c);
   pthread_create(&p4,NULL,thread_func,&d);
   pthread_create(&p5,NULL,thread_func,&e); // Create 5 threads
   char *threadReturnedValue;
   pthread_join(p1,(void**)&threadReturnedValue);
   pthread_join(p2,(void**)&threadReturnedValue);
   pthread_join(p3,(void**)&threadReturnedValue);
   pthread_join(p4,(void**)&threadReturnedValue);
   pthread_join(p5,(void**)&threadReturnedValue); // Wait all five threads to be terminate

    return 0;
}
void *thread_func(void* Tcustomer_num){

    int *c=(int*)Tcustomer_num;
    int customer_num= *c;
    int requestSum=0;
    while(!Finish[customer_num]){   // Whilie loop does stop when thread is finished and need becomes 0
        requestSum=0;
        int request[resource_number]={0};
        for(int i=0;i<resource_number;i++){

            request[i]=rand()%(need[customer_num][i]+1);
            requestSum=requestSum+request[i];
        }
        if(requestSum!=0)  // Make sure it does not requst 0 resources
            while(request_resources(customer_num,request)==-1);
    }

    return 0;
}
int request_resources(int customer_num, int request[]){


    int returnValue=-1;

    // I need the process to print the result and run without race condition. The part
    // involing printing and accesing global variable is critical

    pthread_mutex_lock(&mutex);

    printf("\nP%d requests for ",customer_num+1);
    int i = 0;
    while(i<resource_number){
  printf("%d ",request[i]);
  i++;
    }
    printf("\n");
    i = 0;
          while (i<resource_number) {
   if(request[i]>available[i]){ // Check if request<= availabe
                                                  // if not then return -1
                printf("P%d is waiting for the reaources...\n",customer_num+1);
                pthread_mutex_unlock(&mutex);   // Before thread return, it needs to unlock the mutex
                return -1;
            }
    ++i;
  }

    returnValue=bankerAlgorithm(customer_num,request);  // Execute bankers algorithm

    if(returnValue==0){
        int needIsZero=1;
        printf("Safe sequence is found: ");
        int i = 0;
        while(i<customer_number){
             printf("P%d ",safeSequence[i]+1 );
             i++;
        };
        printf("\nP%d's request has been granted\n",customer_num+1);

        int j = 0;
        while(j<resource_number){
    allocation[customer_num][j]=allocation[customer_num][j]+request[j];
            available[j]=available[j]-request[j];
            need[customer_num][j]=need[customer_num][j]-request[j];
            if(need[customer_num][j]!=0){// Check if need is zero
                needIsZero=0;
            }
            j++;
        }
        if(needIsZero){

            Finish[customer_num]=1; // If need is zero mark the thread "finish"
            release_resources(customer_num); // resources are released when a thread finishes

        }
        printingState();
    }
    else{
        printf("Safe sequesnce cannot be found\n");

    }

    pthread_mutex_unlock(&mutex); // Mutex unlock
    return returnValue;
}
int release_resources(int customer_num){

    printf("P%d Releases all the resources\n",customer_num+1);

    int j = 0;
    while(j<resource_number){
    available[j]=available[j]+allocation[customer_num][j]; // Resources release
      allocation[customer_num][j]=0;
      j++;
    }

    return 0;
}
int bankerAlgorithm(int customer_num,int request[]){
  int finish[customer_number]={0};

    for(int i=0;i<resource_number;i++){ // Matrices copy
        available_metrices[i]=available[i]; // Available matrices
        for(int j=0;j<customer_number;j++){
          allocated[j][i]=allocation[j][i];
          max_metrices[j][i]=maximum[j][i];
          needed[j][i]=need[j][i];
        }
    }
   for(int i=0;i<resource_number;i++){ // Pretend to give the resource to the thread
        available_metrices[i]=available_metrices[i]-request[i];
        allocated[customer_num][i]=allocated[customer_num][i]+request[i];
        needed[customer_num][i]=needed[customer_num][i]-request[i];
        }

// Safety Algorithm
    int count=0;
    while(1){
        int I=-1;
        for(int i=0;i<customer_number;i++){ // Find a thread that its need is less than or equal to available
            int nLessThanA=1;
            for(int j=0; j<resource_number;j++){
                if(needed[i][j]>available_metrices[j] || finish[i]==1){
                    nLessThanA=0;
                    break;
                }
            }
            if(nLessThanA){ // If thread is found record its thread number
                I=i;
                break;
            }
        }
        if(I!=-1){
            safeSequence[count]=I; // Record the sequence
            count++;
            finish[I]=1; // Mark the thread "finish"
            for(int k=0;k<resource_number;k++){
                available_metrices[k]=available_metrices[k]+allocated[I][k];
            }
        }
        else{ // If no threads are found that its need is less than or equal to available.

            for(int i=0;i<customer_number;i++){
                if(finish[i]==0){ // If there is any thread not found that means it cannot find a safe sequence
                    return -1;
                }
            }
            return 0;  // All threads are found

        }
    }

}
void printingState(){

    printf("Resource allocated proceses\n"); // Printing the processes with currently allocated resources
    printf("   %s\n",string);
    for(int i=0; i<customer_number;i++){
        printf("P%d ",i+1);
        for(int j=0;j<resource_number;j++){
            printf("%d ",allocation[i][j]);
        }
        printf("\n");
    }
    printf("The processes that could need resources:\n"); // Printing the processes that might need resources
    printf("   %s\n",string);
    for(int i=0; i<customer_number;i++){
        printf("P%d ",i+1);
        for(int j=0;j<resource_number;j++){
            printf("%d ",need[i][j]);
        }
        printf("\n");
    }
    printf("System resources that are available:\n"); // Printing available system resources
    printf("%s\n",string);
    for (int i=0; i<resource_number; i++) {
        printf("%d ",available[i]);
    }
    printf("\n");
}


