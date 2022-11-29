#include<stdio.h>
#include<string.h>
#include<time.h>
#include "hashmap.h"

unsigned int IDNUM = 1000;         // current employee ID number for new employees

// adds employee to employee array
void addEmployee(char name[], float wage){
    employee *emp = malloc(sizeof(employee));
    strcpy(emp->name, name);
    emp->wage = wage;
    emp->number = IDNUM;
    strcpy(emp->clockIn, "0");
    strcpy(emp->clockOut, "0");
    
    insert(emp);

    IDNUM++;
}

// deletes employee from employee array
void deleteEmployee(int ID){
    removeEmp(ID);
}

// gets the wage for an employee
float getWage(int ID){
    return pay(ID);
}

// gets the current time as a formatted string
void getTime(char* tme){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int iHour = 0;
    int iMinute = 0;
    if(tm.tm_hour < 12){
        iHour = tm.tm_hour;
        iMinute = tm.tm_min;
    }
    else if(tm.tm_hour == 12 || tm.tm_hour == 24){
        iHour = 12;
        iMinute = tm.tm_min;
    }
    else{
        iHour = tm.tm_hour % 12;
        iMinute = tm.tm_min;   
    }
    char hour[3];
    char minute[3];
    itoa(iHour, hour, 10);
    itoa(iMinute, minute, 10);
    strcat(tme, hour);
    strcat(tme, ":");
    if(tm.tm_min < 10){
        strcat(tme, "0");
    }
    strcat(tme, minute);
    if(tm.tm_hour < 12 || tm.tm_hour == 24){
        strcat(tme, " AM");
    }
    else{
        strcat(tme," PM");
    }
}

// updates clockin time for an employee
void clockIn(int ID){
    char tme[9] = "";
    getTime(tme);
    setClockIn(ID, tme);
}

// updates clockout time for an employee
void clockOut(int ID){
    char tme[9] = "";
    getTime(tme);
    setClockOut(ID, tme);
}

// sorts employee array by name
void sortByName(){
    sortName();
}

// sorts employee array by wage
void sortByWage(){
    sortWage();
}

// sorts employee array by ID
void sortByID(){
    sortID();
}

// prints employee array
void printEmployees(){
    printTable();
}

void main(){
    initTable();

    FILE *f;
    
    f = fopen("employees.txt", "r");
    
    if(f == NULL){
        printf("File could not be opened.");
    }

    char name[25];
    float wage;
    char clockInTime[9];
    char clockOutTime[9];
    char line[100];
    int id = 1000;

    while(fgets(line, 100, f)){
        char*piece;
        
        piece = strtok(line, "|");
        strncpy(name, piece, 25);
        
        piece = strtok(NULL, "|");
        wage = atof(piece);
        
        piece = strtok(NULL, "|");
        strncpy(clockInTime, piece, 9);
        
        piece = strtok(NULL, "|");
        strncpy(clockOutTime, piece, 9);
        
        addEmployee(name, wage);
        setClockIn(id, clockInTime);
        setClockOut(id, clockOutTime);
        
        id++;
    }

    fclose(f);


    sortByName();

    sortByWage();

    deleteEmployee(1009);

    sortByID();

    printf("Employee ID 1000 made $%.2f", getWage(1000));

    freeMem();
}
