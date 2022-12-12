#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>




/*print_board*/

void show(int array[10][10]);

void init_board(int arr[10][10]);

void switch_side(int arr[10][10]);

bool is_valid_move(int arr[10][10],int row1, int col1,int row2, int col2,int player);

bool move(int arr[10][10],int r1, int c1, int r2, int c2, int player);

bool undo(int arr[10][10]);

void record_move(int r1,int c1,int r2,int c2);

void saveMoves(const char* filename);

void loadMoves(const char* filename);



void review_forward(int arr[10][10]);
void review_backward(int arr[10][10]);