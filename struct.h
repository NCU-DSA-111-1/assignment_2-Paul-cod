

#ifndef STRUCT_H
#define STRUCT_H

// 定義 struct
struct Move {
    int start_row; 
    int start_col; 
    int end_row; 
    int end_col; 
    int eaten_number;
    struct Move *next_move; 
    struct Move *previous_move;
};


#endif
