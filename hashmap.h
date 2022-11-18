#include<stdlib.h>

typedef struct employee{
    char name[50];      // name
    float wage;         // hourly wage
    int number;         // employee ID
    int clockIn;        // clockin time
    int clockOut;       // clockout time
} employee;

unsigned int size = 100;   // size of hashmap
unsigned int curr = 0;     // current number of employees

employee *table[100];      // array used as hashmap data structure

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
    printf("___________________________________________________________________________________________________________________\n");
    for(int i = 0; i < curr; i++){
        printf("| Employee ID: %-4d | Employee Name: %-50s | Employee Wage: $%-7.2f|\n", arr[i].number, arr[i].name, arr[i].wage);
    }
    printf("___________________________________________________________________________________________________________________\n");

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
unsigned int ind(int ID){
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
    curr++;
    
    int index = hash(emp->number);
    
    if(table[index] != NULL){
        while(table[index] != NULL){
            index++;
            index %= size;
        }
    }
    
    table[index] = emp;

    return 0;
}

// removes employee from hashmap
int removeEmp(int ID){
    if(ID < 1000) return 1;
    if(ind(ID) == -1) return 1;
    int index = ind(ID);

    table[index] = NULL; 
    
    curr--;
    return 0;
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