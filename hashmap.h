#include<stdlib.h>

typedef struct employee{
    char name[25];      // name
    float wage;         // hourly wage
    int number;         // employee ID
    char clockIn[9];        // clockin time
    char clockOut[9];       // clockout time
} employee;

unsigned int size = 5;   // size of hashmap
unsigned int curr = 0;     // current number of employees

employee *table[5];      // array used as hashmap data structure

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

// initialize the table elements as null
void initTable(){
    for(int i = 0; i < size; i++){
        table[i] = NULL;
    }
}

// print the employees
void printArr(employee arr[]){
    printf("___________________________________________________________________________________________________________________________________\n");
    for(int i = 0; i < curr; i++){
        printf("| Employee ID: %-4d | Employee Name: %-22s | Employee Wage: $%-7.2f | Clock In: %-9s | Clock Out: %-9s|\n", arr[i].number, arr[i].name, arr[i].wage, arr[i].clockIn, arr[i].clockOut);
    }
    printf("___________________________________________________________________________________________________________________________________\n");

}

// finds employees currently in the hashmap and passes them to printArr to print the employees
void printTable(){
    employee temp[curr];
    int j = 0;
    for (int i = 0; i < size; i++){
        if(table[i] != NULL){
            temp[j] = *table[i];
            j++;
        }
    }
    printArr(temp);
}

// hash function
unsigned int hash(int ID){
    unsigned int value = 0;
    for(int i = 0; i < 5; i++){
        value += (ID-1)*57;
        value += (value*1093)%size;
        value %= size;
    }
    return value;
}

// finds index of a given employee, returns -1 if the employee is not in the hashmap
int ind(int ID){
    unsigned int hashVal = hash(ID);
    if (ID == table[hashVal]->number){
        return hashVal;
    }
    int seen = 1;
    while((ID != table[hashVal]->number) && (seen <= size)){
        hashVal ++;
        hashVal %= size;
        seen++;
    }
    if(seen > size){
        return -1;
    }
    return hashVal;
}

// inserts employee into hashmap
int insert(employee *emp){
    if(emp == NULL) return 1;
    

    if(curr == size){
        employee *temp[curr];

        for (int i = 0; i < size; i++){
            temp[i] = table[i];
        }

        size = size * 2;
        employee *newTable[size];

        for(int i = 0; i < size; i++){
            newTable[i] = NULL;
        }

        for(int i = 0; i < curr; i++){
            int index = hash(temp[i]->number);
            if(newTable[index] != NULL){
                while(newTable[index] != NULL){
                    index++;
                    index %= size;
                }
            }
            newTable[index] = temp[i];
        }

        memcpy(table, newTable, sizeof(newTable));
    }
    
    int index = hash(emp->number);
    
    if(table[index] != NULL){
        while(table[index] != NULL){
            index++;
            index %= size;
        }
    }
    
    table[index] = emp;

    curr++;

    return 0;
}

// removes employee from hashmap
int removeEmp(int ID){
    if(ID < 1000) return 1;
    if(ind(ID) == -1) return 1;
    
    curr--;

    int index = ind(ID);
    free(table[index]);
    table[index] = NULL;

    if(curr <= 0.5*size){ 
        employee *temp[curr];
        
        int j = 0;
        for (int i = 0; i < size; i++){
            if(table[i] != NULL){
                temp[j] = table[i];
                j++;
            }
        }
        
        size = size * 0.75;
        employee *newTable[size];
        
        for(int i = 0; i < size; i++){
            newTable[i] = NULL;
        }
        
        for(int i = 0; i < curr; i++){
            int index = hash(temp[i]->number);
            if(newTable[index] != NULL){
                while(newTable[index] != NULL){
                    index++;
                    index %= size;
                }
            }
            newTable[index] = temp[i];
        }

        memcpy(table, newTable, sizeof(newTable));
    } 
    
    return 0;
}   

// sets clockin time for an employee
int setClockIn(int ID, char time[]){
    if(ID < 1000) return 1;
    if(ind(ID) == -1) return 1;

    int index = ind(ID);
    strcpy(table[index]->clockIn, time);
    return 0;
}

// sets clockout time for an employee
int setClockOut(int ID, char time[]){
    if(ID < 1000) return 1;
    if(ind(ID) == -1) return 1;

    int index = ind(ID);
    strcpy(table[index]->clockOut, time);
    return 0;
}

// returns time as float equivalent
float timeToFloat(char time[]){
    float hour = atof(strtok(time, ":"));
    float minute = atof(strtok(NULL, " "));
    char *check = strtok(NULL, "M");

    if(strcmp(check, "P") == 0){
        hour += 12;
    }

    return (hour + (minute/60));
}

// returns pay for employee
float pay(int ID){
    int index = ind(ID);
    float inTime = timeToFloat(table[index]->clockIn);
    float outTime = timeToFloat(table[index]->clockOut);
    return(table[index]->wage*(outTime-inTime));
}


// sorts employees by name
void sortName(){
    employee temp[curr];
    int j = 0;
    for (int i = 0; i < size; i++){
        if(table[i] != NULL){
            temp[j] = *table[i];
            j++;
        }
    }
    qsort(temp, curr, sizeof(employee), cmpName);
    printArr(temp);
}

// sorts employees by wage
void sortWage(){
    employee temp[curr];
    int j = 0;
    for (int i = 0; i < size; i++){
        if(table[i] != NULL){
            temp[j] = *table[i];
            j++;
        }
    }
    qsort(temp, curr, sizeof(employee), cmpWage);
    printArr(temp);
}

// sorts employees by id
void sortID(){
    employee temp[curr];
    int j = 0;
    for (int i = 0; i < size; i++){
        if(table[i] != NULL){
            temp[j] = *table[i];
            j++;
        }
    }
    qsort(temp, curr, sizeof(employee), cmpID);
    printArr(temp);
}

// frees memory used to hashmap
void freeMem(){
    for(int i = 0; i<size; i++){
        if(table[i] != NULL){
            free(table[i]);
        }
    }
}
