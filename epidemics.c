#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define INFECTED_DURATION 20
#define IMMUNE_DURATION 1

pthread_mutex_t lock;
pthread_barrier_t barrier;

int NP, TN, MXC, MYC, TST;

// status: 0-infected, 1-susceptible, 2-immune
// direction N-0 S-1 E-2 W-3

typedef struct Person{
    int id;
    int x, y;
    int status;
    int direction;
    int amplitude;
    int future_status;
    int time_infected;
    int time_immuned;
    int count_infected;
}Person;

Person *p;

Person* read_data_from_inputFile(char *fileName, int *MAX_X_COORD, int *MAX_Y_COORD, int *NUMBER_OF_PERSONS){
    FILE *f = fopen(fileName, "r");

    if(f == NULL){
        printf("The file cannot be opened\n");
        exit(-4);
    }

    char buffer[1000];
    if(fgets(buffer, 1000, f) == NULL){
        printf("The MAX COORDINATES cannot be readed\n");
        exit(-5);
    }
    sscanf(buffer, "%d %d", MAX_X_COORD, MAX_Y_COORD);

    if(fgets(buffer, 1000, f) == NULL){
        printf("The NUMBER OF PERSONS cannot be readed\n");
        exit(-6);
    }
    sscanf(buffer, "%d", NUMBER_OF_PERSONS);
    Person *result = (Person*)malloc(*NUMBER_OF_PERSONS * sizeof(Person));
    if(result == NULL){
        printf("Insufficient space for memory allocation!\n");
        exit(-7);
    }

    for(int i = 0; i < *NUMBER_OF_PERSONS; i++){
        if(fgets(buffer, 1000, f) == NULL){
            printf("A person data cannot be readed\n");
            exit(-8);
        }
        sscanf(buffer, "%d %d %d %d %d %d", &result[i].id, &result[i].x, &result[i].y, &result[i].status, &result[i].direction, &result[i].amplitude);
        if(result[i].x > *MAX_X_COORD || result[i].y > *MAX_Y_COORD || result[i].x < 0 || result[i].y < 0){
            printf("Persons coodonates are not ok!\n");
            exit(-9);
        }
        result[i].time_infected = 0;
        result[i].count_infected = 0;
        result[i].time_immuned = 0;
        result[i].future_status = result[i].status;
    }

    fclose(f);
    return result;
}

void print_persons(Person *persons, int NUMBER_OF_PERSONS){
    for(int i = 0; i < NUMBER_OF_PERSONS; i++){
        printf("Person %d\nCoordinates: (%d %d)\nStatus: %d\nDirection: %d\nAmplitude: %d\n\n", persons[i].id, persons[i].x, persons[i].y, persons[i].status, persons[i].direction, persons[i].amplitude);
    }
}

void Simulation_Sequential(int TOTAL_SIMULATION_TIME, Person *persons, int MAX_X_COORD, int MAX_Y_COORD, int NUMBER_OF_PERSONS){
    int all_infections = 0;
    for(int i = 0; i < TOTAL_SIMULATION_TIME; i++){
        // Movement logic
        for(int j = 0; j < NUMBER_OF_PERSONS; j++){
            if(persons[j].direction == 0){
                if(persons[j].y + persons[j].amplitude > MAX_Y_COORD){
                    persons[j].direction = 1;
                    persons[j].y -= persons[j].amplitude;
                }else{
                    persons[j].y += persons[j].amplitude;
                }
            }
            else if(persons[j].direction == 1){
                if(persons[j].y - persons[j].amplitude < 0){
                    persons[j].direction = 0;
                    persons[j].y += persons[j].amplitude;
                }else{
                    persons[j].y -= persons[j].amplitude;
                }
            }
            else if(persons[j].direction == 2){
                if(persons[j].x + persons[j].amplitude > MAX_X_COORD){
                    persons[j].direction = 3;
                    persons[j].x -= persons[j].amplitude;
                }else{
                    persons[j].x += persons[j].amplitude;
                }
            }
            else if(persons[j].direction == 3){
                if(persons[j].x - persons[j].amplitude < 0){
                    persons[j].direction = 2;
                    persons[j].x += persons[j].amplitude;
                }else{
                    persons[j].x -= persons[j].amplitude;
                }
            }
            else{
                printf("Why are you here?\n");
                exit(-12);
            }
        }
        // Check if 2 Persons are in the same locations, if there is an infected people he will infect other persons
        for(int j = 0; j < NUMBER_OF_PERSONS; j++){
            for(int k = j + 1; k < NUMBER_OF_PERSONS; k++){
                if((persons[j].x == persons[k].x) && (persons[j].y == persons[k].y)){
                    if(persons[j].status == 0 && persons[k].status == 1){
                        persons[k].future_status = 0;
                        persons[k].count_infected++;
                        all_infections++;
                    }
                    else if(persons[j].status == 1 && persons[k].status == 0){
                        persons[j].future_status = 0;
                        persons[j].count_infected++;
                        all_infections++;
                    }
                    //printf("(%d %d)-status:%d (%d %d)-status:%d\n", persons[j].x, persons[j].y, persons[j].status, persons[k].x, persons[k].y, persons[k].status);
                }
            }
        }
        // check if the infected become immune and the immune become susceptible
        for(int j = 0; j < NUMBER_OF_PERSONS; j++){
            if((persons[j].status == 0) && (persons[j].time_infected < INFECTED_DURATION)){
                persons[j].time_infected++;
            }
            else if((persons[j].status == 0) && (persons[j].time_infected == INFECTED_DURATION)){
                persons[j].time_infected = 0;
                persons[j].future_status = 2;
            }
            else if((persons[j].status == 2) && (persons[j].time_immuned < IMMUNE_DURATION)){
                persons[j].time_immuned++;
            }
            else if((persons[j].status == 2) && (persons[j].time_immuned == IMMUNE_DURATION)){
                persons[j].time_immuned = 0;
                persons[j].future_status = 1;
            }
        }
        // Apply status updates
        for(int j = 0; j < NUMBER_OF_PERSONS; j++){
            if(persons[j].future_status != persons[j].status){
                persons[j].status = persons[j].future_status;
            }
        }
    }
    printf("all infections: %d\n", all_infections);
}

void *function_logic(void *t){
    int my_id = *(int *)t;
    int persons_per_thread = NP / TN;
    int end = (my_id + 1) * persons_per_thread;
    int start = persons_per_thread * my_id;
    if(my_id == TN - 1) end = NP;

        // Movement logic
        for(int j = start; j < end; j++){
            if(p[j].direction == 0){
                if(p[j].y + p[j].amplitude > MYC){
                    p[j].direction = 1;
                    p[j].y -= p[j].amplitude;
                }else{
                    p[j].y += p[j].amplitude;
                }
            }
            else if(p[j].direction == 1){
                if(p[j].y - p[j].amplitude < 0){
                    p[j].direction = 0;
                    p[j].y += p[j].amplitude;
                }else{
                    p[j].y -= p[j].amplitude;
                }
            }
            else if(p[j].direction == 2){
                if(p[j].x + p[j].amplitude > MXC){
                    p[j].direction = 3;
                    p[j].x -= p[j].amplitude;
                }else{
                    p[j].x += p[j].amplitude;
                }
            }
            else if(p[j].direction == 3){
                if(p[j].x - p[j].amplitude < 0){
                    p[j].direction = 2;
                    p[j].x += p[j].amplitude;
                }else{
                    p[j].x -= p[j].amplitude;
                }
            }
        }
        pthread_barrier_wait(&barrier);
        // Check if 2 Persons are in the same locations, if there is an infected people he will infect other persons
        for(int j = start; j < end; j++){
            for(int k = j + 1; k < NP; k++){
                if((p[j].x == p[k].x) && (p[j].y == p[k].y)){
                    //printf("Thread %d x: %d, y: %d time infected: %d time immuned: %d\n",my_id, p[j].x, p[j].y, p[j].time_infected, p[j].time_immuned);
                    if(p[j].status == 0 && p[k].status == 1){
                        pthread_mutex_lock(&lock);
                        p[k].future_status = 0;
                        p[k].count_infected++;
                        pthread_mutex_unlock(&lock);
                    }
                    else if(p[j].status == 1 && p[k].status == 0){
                        pthread_mutex_lock(&lock);
                        p[j].future_status = 0;
                        p[j].count_infected++;
                        pthread_mutex_unlock(&lock);
                    }
                    //printf("(%d %d)-status:%d (%d %d)-status:%d\n", persons[j].x, persons[j].y, persons[j].status, persons[k].x, persons[k].y, persons[k].status);
                }
            }
        }
        pthread_barrier_wait(&barrier);
        // check if the infected become immune and the immune become susceptible
        for(int j = start; j < end; j++){
            if((p[j].status == 0) && (p[j].time_infected < INFECTED_DURATION)){
                p[j].time_infected++;
            }
            else if((p[j].status == 0) && (p[j].time_infected == INFECTED_DURATION)){
                p[j].time_infected = 0;
                p[j].future_status = 2;
            }
            else if((p[j].status == 2) && (p[j].time_immuned < IMMUNE_DURATION)){
                p[j].time_immuned++;
            }
            else if((p[j].status == 2) && (p[j].time_immuned == IMMUNE_DURATION)){
                p[j].time_immuned = 0;
                p[j].future_status = 1;
            }
        }
        // Apply status updates
        for(int j = start; j < end; j++){
            if(p[j].future_status != p[j].status){
                p[j].status = p[j].future_status;
            }
        }

    return NULL;
}

void Simulation_Parallel(int ThreadsNumber){
    pthread_t threads[ThreadsNumber];

    for(int i = 0; i < TST; i++){
        pthread_mutex_init(&lock, NULL);
        pthread_barrier_init(&barrier, NULL, ThreadsNumber);
        for (int i = 0; i < ThreadsNumber; i++)
        {
            int *id = malloc(sizeof(int));
            *id = i;
            pthread_create(&threads[i], NULL, function_logic, (void *)id);
        }
        for (int i = 0; i < ThreadsNumber; i++)
        {
            pthread_join(threads[i], NULL);
        }
        pthread_mutex_destroy(&lock);
        pthread_barrier_destroy(&barrier);
    }
}

char *createOutputFileName(const char *InputFileName, char *endEtxt) {
    char *outputFileName = (char*)malloc(50 * sizeof(char));
    if (outputFileName == NULL) {
        printf("Insufficient memory for allocation\n");
        exit(-10);
    }

    strcpy(outputFileName, InputFileName);

    char *dotPosition = strrchr(outputFileName, '.');
    if (dotPosition != NULL) {
        *dotPosition = '\0';
    }

    strcat(outputFileName, endEtxt);

    return outputFileName;
}

void write_in_outputFile(char *fileName, int NUMBER_OF_PERSONS, Person *persons){
    FILE *f = fopen(fileName, "w");
    if(f == NULL){
        printf("The file cannot be opened\n");
        exit(-11);
    }
    for(int i = 0; i < NUMBER_OF_PERSONS; i++){
        fprintf(f, "Final Coodinates: (%d %d)\nFinal status: %d\nInfection counter: %d\n\n", persons[i].x, persons[i].y, persons[i].status, persons[i].count_infected);
    }
    fclose(f);
}

int main(int argc, char *argv[]){
    if(argc != 4){
        printf("The number of arguments is wrong!\n");
        exit(-1);
    }

    struct timespec start, finish;
    double elapsed_serial, elapsed_parallel;

    int TOTAL_SIMULATION_TIME = atoi(argv[1]), ThreadsNumber = atoi(argv[3]);
    char *InputFileName = argv[2];

    if(TOTAL_SIMULATION_TIME == 0){
        printf("Total simulation time cannot be 0!\n");
        exit(-2);
    }else if(ThreadsNumber == 0){
        printf("Number of threads cannot be 0\n");
        exit(-3);
    }
    printf("TOTAL_SIMULATION_TIME: %d\nInputFileName: %s\nThreadsNumber: %d\n\n", TOTAL_SIMULATION_TIME, InputFileName, ThreadsNumber);

    int MAX_X_COORD, MAX_Y_COORD, NUMBER_OF_PERSONS;

    Person *persons = read_data_from_inputFile(InputFileName, &MAX_X_COORD, &MAX_Y_COORD, &NUMBER_OF_PERSONS);
    //printf("MAX_X_COORD: %d\nMAX_Y_COORD: %d\nNUMER_OF_PERSONS: %d\n", MAX_X_COORD, MAX_Y_COORD, NUMBER_OF_PERSONS);
    if(NUMBER_OF_PERSONS % ThreadsNumber != 0){
        printf("The number of threads is not divisble with the number of persons\n");
        exit(-13);
    }

    char *outputFileNameSeq = createOutputFileName(InputFileName, "_serial_out.txt"); 
    printf("serial output file name: %s\n", outputFileNameSeq);
    clock_gettime(CLOCK_MONOTONIC, &start); // measure wall clock time!
    Simulation_Sequential(TOTAL_SIMULATION_TIME, persons, MAX_X_COORD, MAX_Y_COORD, NUMBER_OF_PERSONS);

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed_serial = (finish.tv_sec - start.tv_sec);
    elapsed_serial += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("time serial =%lf \n", elapsed_serial);

    write_in_outputFile(outputFileNameSeq, NUMBER_OF_PERSONS, persons);

    char *outputFileParallel = createOutputFileName(InputFileName, "_parallel_out.txt");
    printf("parallel output file name: %s\n", outputFileParallel);
    p = read_data_from_inputFile(InputFileName, &MAX_X_COORD, &MAX_Y_COORD, &NUMBER_OF_PERSONS);
    MXC = MAX_X_COORD;
    MYC = MAX_Y_COORD;
    TN = ThreadsNumber;
    NP = NUMBER_OF_PERSONS;
    TST = TOTAL_SIMULATION_TIME;
    clock_gettime(CLOCK_MONOTONIC, &start); // measure wall clock time!
    Simulation_Parallel(ThreadsNumber);

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed_parallel = (finish.tv_sec - start.tv_sec);
    elapsed_parallel += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("time prallel =%lf \n", elapsed_parallel);

    printf("Speedup = %lf\n", elapsed_serial / elapsed_parallel);

    write_in_outputFile(outputFileParallel, NUMBER_OF_PERSONS, p);

    return 0;
}