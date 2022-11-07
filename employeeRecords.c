#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// employee data structure
typedef struct employee{
    char name[50];      // name
    float wage;         // hourly wage
    int number;         // employee ID
    int clockIn;        // clockin time
    int clockOut;       // clockout time
} employee;

// initalizing global variables
unsigned int max = 10;           // predetermined max number of employees
employee *empArr;       // employee array
unsigned int numEmp = 0;         // current employee ID number for new employees
unsigned int currEmp = 0;        // current number of employees

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

// adds employee to employee array
void addEmployee(char name[], float wage){
    employee emp;
    strcpy(emp.name, name);
    emp.wage = wage;
    emp.number = numEmp;
    
    if(currEmp == max - 1){
        max = max*2;
        empArr = realloc(empArr, max * sizeof(employee));
    }
    empArr[currEmp] = emp;

    numEmp++;
    currEmp++;
}

// delets employee from employee array
void deleteEmployee(unsigned int ID){
    int index;
    for(int i = 0; i < currEmp; i++){
        if(empArr[i].number == ID){
            index = i;
        }
    }

    for(int i = index; i < currEmp; i++){
        empArr[i] = empArr[i+1];
    }

    currEmp--;

    if(currEmp <= max/2){
        max = (0.75)*max;
        empArr = realloc(empArr, max * sizeof(employee));
    }
}

// gets the wage for an employee
float getWage(unsigned int ID){
    float pay = 0;
    for(int i = 0; i < currEmp; i++){
        if(empArr[i].number == ID){
            int time = empArr[i].clockOut - empArr[i].clockIn;
            pay = empArr[i].wage * (float) time;
            break;
        }
    }
    return pay;
}

// updates clockin time for an employee
void clockIn(unsigned int ID, int time){
    int newTime;
    for(int i = 0; i < currEmp; i++){
        if(empArr[i].number = ID){
            empArr[i].clockIn = newTime;
            break;
        }
    }
}

// updates clockout time for an employee
void clockOut(unsigned int ID, int time){
    int newTime;
    for(int i = 0; i < currEmp; i++){
        if(empArr[i].number = ID){
            empArr[i].clockOut = newTime;
            break;
        }
    }
}

// sorts employee array by name
void sortByName(){
    qsort(empArr, currEmp, sizeof(employee), cmpName);

}

// sorts employee array by wage
void sortByWage(){
    qsort(empArr, currEmp, sizeof(employee), cmpWage);
}

// sorts employee array by ID
void sortByID(){
    qsort(empArr, currEmp, sizeof(employee), cmpID);
}

// prints employee array
void printEmployees(){
    printf("___________________________________________________________________________________________________________________\n");
    for(int i = 0; i < currEmp; i++){
        printf("| Employee ID: %-5d | Employee Name: %-50s | Employee Wage: $%-7.2f|\n", empArr[i].number, empArr[i].name, empArr[i].wage);
    }
    printf("___________________________________________________________________________________________________________________\n");
}

void main(){
    empArr = calloc(max, sizeof(employee));

    addEmployee("Jim Bob", 100);
    addEmployee("Kai Scarpato", 200);
    addEmployee("Alex Mark", 300);
    addEmployee("Alicent Hightower", 400);
    addEmployee("Bill Huff", 500);
    addEmployee("Isaiah Rashad", 600);
    addEmployee("Walter White", 700);
    addEmployee("Walter Whitman", 800);
    addEmployee("Big Dog", 200);
    addEmployee("Small Dog", 1000);
    addEmployee("Medium Dog", 1100);

    sortByWage();

    printEmployees();
    
    sortByName();

    printEmployees();

    sortByID();

    printEmployees();

    deleteEmployee(10);

    deleteEmployee(2);

    addEmployee("Bob Jim", 1200);

    printEmployees();

    free(empArr);
}