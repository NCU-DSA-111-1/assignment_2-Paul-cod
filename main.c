#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"func.h"
#include"struct.h"
#define 紅方 1
#define 藍方 2

int main(int argc, char *argv[]){

    if(strcmp(argv[1],"-n")==0){
        int board[10][10];
        init_board(board);
        //-n+紀錄檔名
        int row1,row2,col1,col2;
        char c_input[1];
        int num_input=0;
        int player=紅方;
        while (true)
        {   
            /// 解決紅方的時候可以移動蘭芳棋字的問題
            show(board);
            printf("Now is %s turn\n",(player==紅方)?"red's":"blue's");
            printf("Enter 's'for saving game, '0' for go back to the last move, or the row of the piece you want to move : ");
            if (scanf("%d",&num_input)==1)  //  若輸入符號為數字（可能為0或row1座標）
            {   
                if (num_input==0)   {    //悔棋  
                    if(!undo(board)){
                        printf("No previous move\n");
                    }
                    else{
                        printf("Successfully undo the move!, plaese enter position again.\n");
                        player=(player==紅方)?藍方:紅方;
                    }
                }
                else{ //不為0的數字輸入
                    row1=num_input;
                    printf("Enter the column of the piece you want to move: ");
                    scanf("%d",&col1);
                    printf("Enter the row of the destination : ");
                    scanf("%d",&row2);
                    printf("Enter the column of the destination : ");
                    scanf("%d",&col2);
                    if (!move(board,row1,col1,row2,col2,player))//移動不合法，函式回傳false
                    {
                        printf("The move is illegal, please enter the position again.\n");
                    }
                    else{   //執行move之後board已更改
                        /*走棋合法執行*/
                        record_move(row1,col1,row2,col2);//紀錄此次走棋
                        player=(player==紅方)?藍方:紅方; //讓player變數交替
                        switch_side(board);//藍紅方換邊顯示
                    }
                }
            }
            /*寫入檔案*/
            else if(scanf("%s",c_input)==1&&c_input[0]=='s'){ 
                saveMoves(argv[2]);
                break;
            }
            else{
                printf("Wrong input!\n");
            }
        }
 
    }
    else if(strcmp(argv[1],"-l")==0){
        int board[10][10];
        init_board(board);
        loadMoves(argv[2]);//argv[2]為要讀取的檔案名稱
        char c_input[10];
        show(board);
        while (true)
        {   
            printf("Enter 'f' to review the next move or 'b' to review the previous move: ");
            scanf("%s",c_input);
            if(strcmp(c_input,"f")==0){
                review_forward(board);
            }
            else if (strcmp(c_input,"b")==0)
            {
               review_backward(board);
            }
            else{
                printf("Wrong input!Please enter again.\n");
            }
        }

    }
    else{
        printf("Wrong input! Please choose a mode.\n");
    }

    return 0;
}