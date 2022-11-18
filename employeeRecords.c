#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<stdio.h>
#include<string.h>
#include "hashmap.h"

unsigned int IDNUM = 1000;         // current employee ID number for new employees

// adds employee to employee array
void addEmployee(char name[], float wage){
    employee *emp = malloc(sizeof(employee));
    strcpy(emp->name, name);
    emp->wage = wage;
    emp->number = IDNUM;
    emp->clockIn = 0;
    emp->clockOut = 0;
    
    insert(emp);

    IDNUM++;
}

// deletes employee from employee array
void deleteEmployee(int ID){
    removeEmp(ID);
}

// gets the wage for an employee
float getWage(int ID){
    
}

// updates clockin time for an employee
void clockIn(int ID, int time){
    
}

// updates clockout time for an employee
void clockOut(int ID, int time){

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

    sortByName();

    sortByWage();

    deleteEmployee(1009);

    sortByID();

    freeMem();
}
