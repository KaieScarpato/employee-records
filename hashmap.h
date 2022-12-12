#include<stdlib.h>
#include<string.h>
#include<stdio.h>

typedef struct employee{
    char name[25];          // name
    float wage;             // hourly wage
    int number;             // employee ID
    char clockIn[9];        // clockin time
    char clockOut[9];       // clockout time
} employee;

unsigned int size = 100;      // size of hashmap
unsigned int curr = 0;        // current number of employees

employee *table[100];        // array used as hashmap data structure


/*-----------------------Helper functions: Shouldn't be accessed by user---------------------------*/
// name comparator
int cmpName(const void *a, const void *b){
    const employee *emp1 = (employee *) a;
    const employee *emp2 = (employee *) b;
    return strcmp(emp1->name, emp2->name);
}

// wage comparator
int cmpWage(const void *a, const void *b){
    const employee *emp1 = (employee *) a;
    const employee *emp2 = (employee *) b;
    return(emp1->wage - emp2->wage);
}

// id comparator
int cmpID(const void *a, const void *b){
    const employee *emp1 = (employee *) a;
    const employee *emp2 = (employee *) b;
    return(emp1->number - emp2->number);
}

// returns time as float equivalent, used for employee pay
float timeToFloat(char time[]){
    float hour = atof(strtok(time, ":"));           // gets hour as float equivalent
    float minute = atof(strtok(NULL, " "));         // gets minutes as float equivalent
    char *check = strtok(NULL, "M");                // either A or P

    if((strcmp(check, "P") == 0) && (hour != 12)){  // check if hour is in the PM
        hour += 12;                                 // add 12 to hour if hour != 12
    }

    if((strcmp(check, "A") == 0) && (hour == 12)){  // check if hour is in the AM
        hour -= 12;                                 // subtract 12 from hour if hour == 12
    }

    return (hour + (minute/60));                    // return float equivalent of hour
}

// print the employees and writes data into a empData.txt file
void printArr(employee arr[]){
    FILE *f;
    f = fopen("empData.txt", "w");

    printf("______________________________________________________________________________________________________________________________________\n");
    fprintf(f,"______________________________________________________________________________________________________________________________________\n");
    for(int i = 0; i < curr; i++){
        printf("| Employee ID: %-4d | Employee Name: %-25s | Employee Wage: $%-7.2f | Clock In: %-9s | Clock Out: %-9s|\n", arr[i].number, arr[i].name, arr[i].wage, arr[i].clockIn, arr[i].clockOut);
        fprintf(f,"| Employee ID: %-4d | Employee Name: %-25s | Employee Wage: $%-7.2f | Clock In: %-9s | Clock Out: %-9s|\n", arr[i].number, arr[i].name, arr[i].wage, arr[i].clockIn, arr[i].clockOut);
    }
    printf("______________________________________________________________________________________________________________________________________\n");
    fprintf(f,"______________________________________________________________________________________________________________________________________\n");

    fclose(f);
}

// finds employees currently in the hashmap and passes them to printArr to print the employees
void printTable(){
    employee temp[curr];                // create tempory array
    int j = 0;                          // current index in temp
    for (int i = 0; i < size; i++){     // cyclye through the table
        if(table[i] != NULL){           // set temp[j] = table[i] if table[i] != NULL
            temp[j] = *table[i];
            j++;
        }
    }
    printArr(temp);                     // use above function to print temp array
}

// hash function, returns index within the size constraints of the table
unsigned int hash(int ID){
    unsigned int value = 0;
    for(int i = 0; i < 5; i++){
        value += (ID-1)*57;             
        value += (value*1093)%size;
        value %= size;
    }
    return value;
}

/*-----------------------------------End of helper functions---------------------------------*/

/*-----------------Lookup, Insert, and Delete functions: Can be used by User-----------------*/
// finds index of a given employee, returns -1 if the employee is not in the hashmap
int ind(int ID){
    if(ID < 1000) return -1;                                // checks if the ID is valid
    unsigned int hashVal = hash(ID);                        // get the hash value for the given ID number

    if (table[hashVal] != NULL){
        if(ID == table[hashVal]->number){                      // return the value if we are at the correct position
            return hashVal;
        }
    }

    int seen = 0;                                           // set seen to 0
    while(seen < size){                                    // cycle through table until we find the correct employee or until we've seen all the employee's
        seen++;                                             // update seen
        hashVal++;                                          // increment to the next value
        hashVal %= size;                                    // stays within size constraints of the table
        while(table[hashVal] == NULL){                      // increment further if the value is pointing to a null pointer in the table
            seen++;
            hashVal++;
            hashVal %= size;
        }
        if(table[hashVal]->number == ID){
            break;
        }
    }

    if(seen >= size){                                       // if we've seen all the positions and haven't found the employee
        return -1;
    }
    
    return hashVal;                                         // return the index
}

// inserts employee into hashmap
int insert(employee *emp){
    if(emp == NULL) return 1;                       // if the employee being inserted is null, return 1

    if(curr == size){                               // grow table if the table is currently filled
        int oSize = size;                           // old size of table
        size = size*2;                              // new size = 2 * old size
        employee *newTable[size];                   // create a new table of size = new size

        for(int i = 0; i < size; i++){              // initalize new table elements to null
            newTable[i] = NULL;
        }
        
        for(int i = 0; i <oSize; i++){              // cycle through the old table, update the new table with employees
            if(table[i] != NULL){                   // if the is an employee at location i in the table
                int index = hash(table[i]->number); // get the index of the employee for the new table
                if(newTable[index] != NULL){        // if there is already an employee at this position
                    while(newTable[index] != NULL){ // cycle until we find an empty position
                        index++;
                        index %= size;
                    }
                }
                newTable[index] = table[i];         // insert employee into new table at location of index
            }
        }
        memcpy(table, newTable, sizeof(newTable));  // copy data from newTable to table
    }   
    
    int index = hash(emp->number);                  // find index for new employee
    if(table[index] != NULL){                       // if there is already an employee at this position
        while(table[index] != NULL){                // cycle until we find an empty position
            index++;
            index %= size;
        }
    }
    table[index] = emp;                             // update the table
    
    curr++;                                         // update current number of employee

    return 0;                                       // return 0 for successful insertion
}

// removes employee from hashmap
int removeEmp(int ID){
    if(ID < 1000) return 1;                         // check for valid ID
    if(ind(ID) == -1) return 1;                     // check that the employee exists
    
    curr--;                                         // update current number of employees

    int index = ind(ID);                            // find the index of the employee
    free(table[index]);                             // free the memory at this location
    table[index] = NULL;                            // set the table[index] = NULL

    if(curr <= 0.5*size){                           // if we have half as many people as we can store, consolidate the table
        int oSize = size;                           // old size
        size = size * 0.75;                         // new size = 3/4 the old size
        employee *newTable[size];                   // temporary new table
        
        for(int i = 0; i < size; i++){              // initialize new table elements to null
            newTable[i] = NULL;
        }
        
        for(int i = 0; i < oSize; i++){             // cycle through old table
            if(table[i] != NULL){                   // if there exists an employee at this location
                int index = hash(table[i]->number); // get the index of this employee in the new table
                if(newTable[index] != NULL){        // if there exists an employee at this index
                    while(newTable[index] != NULL){ // cycle until we find an empty spot
                        index++;
                        index %= size;
                    }
                }
                newTable[index] = table[i];         // insert employee into new table at location of index
            }
        }
        memcpy(table, newTable, sizeof(newTable));  // copy data from new table to table
    }
    return 0;                                       // return 0 for successful deletion
}
/*---------------------End of Lookup, Insert and Delete Functions------------------------------*/

/*-------------------ClockIn and Pay Functions: Can be accessed by user-----------------------*/

// sets clockin time for an employee
int setClockIn(int ID, char time[]){
    if(ID < 1000) return 1;                 // check for valid ID
    if(ind(ID) == -1) return 1;             // check if employee exists in the table

    int index = ind(ID);                    // get the index of the employee
    strcpy(table[index]->clockIn, time);    // copy time to employee clockin time
    return 0;                               // return 0 for successful clockin
}

// sets clockout time for an employee
int setClockOut(int ID, char time[]){
    if(ID < 1000) return 1;                 // check for valid ID
    if(ind(ID) == -1) return 1;             // check if employee exists in the table

    int index = ind(ID);                    // get the index of the employee
    strcpy(table[index]->clockOut, time);   // copy time to employee clockout time
    return 0;                               // return 0 for successful clockout
}

// returns pay for employee
float pay(int ID){
    if(ID < 1000) return -1.0;                              // check for valid ID
    if(ind(ID) == -1) return -1.0;                          // check if employee exists in the table 
    
    int index = ind(ID);                                    // get the index of the employee
    char inTme[9];                                          // clockin time
    char outTme[9];                                         // clockout time
    
    strcpy(inTme, table[index]->clockIn);                   // copy data to inTme
    strcpy(outTme, table[index]->clockOut);                 // copy data to outTme
    
    float inTime = timeToFloat(inTme);                      // get the clockin time as a float equivelant
    float outTime = timeToFloat(outTme);                    // get the clockout time as a float equivelant
    return(table[index]->wage*(outTime-inTime));            // return the pay for the pay period
}
/*--------------------------End of ClockIn and Pay Functions-------------------------------*/

/*----------------Functions to display sorted data: Can be accessed by user----------------*/
// sorts employees by name
void sortName(){
    employee temp[curr];                            // create temporary array 
    int j = 0;                                      // current index in the temp array
    for (int i = 0; i < size; i++){                 // cycle through the table, put elements != null in the table 
        if(table[i] != NULL){
            temp[j] = *table[i];
            j++;                                    // increment index
        }
    }
    qsort(temp, curr, sizeof(employee), cmpName);   // sort the array by name
    printArr(temp);                                 // display the sorted data
}

// sorts employees by wage
void sortWage(){
    employee temp[curr];                            // create temporary array
    int j = 0;                                      // current index in the temp array
    for (int i = 0; i < size; i++){                 // cycle through the table, put elements != null in the table
        if(table[i] != NULL){
            temp[j] = *table[i];
            j++;                                    // increment index
        }
    }
    qsort(temp, curr, sizeof(employee), cmpWage);   // sort the array by wage
    printArr(temp);                                 // display the sorted data
}

// sorts employees by id
void sortID(){
    employee temp[curr];                            // create temporary array
    int j = 0;                                      // current index in the temp array
    for (int i = 0; i < size; i++){                 // cycle through the table, put elements != null in the table
        if(table[i] != NULL){
            temp[j] = *table[i];
            j++;                                    // increment index
        }
    }
    qsort(temp, curr, sizeof(employee), cmpID);     // sort the array by ID
    printArr(temp);                                 // display the sorted data
}
/*----------------------------End of functions to display sorted data---------------------------*/

/*---Functions to initialize the table and free table memory when the table is done being used---*/
// initialize the table elements as null, needs to be called before insertion begins
void initTable(){
    for(int i = 0; i < size; i++){
        table[i] = NULL;
    }
}

// frees memory used by the table, needs to be called when hashmap use is oover
void freeMem(){
    for(int i = 0; i<size; i++){
        if(table[i] != NULL){
            free(table[i]);
        }
    }
}
/*-------------------------End of table initialization memory functions------------------------*/
