#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"func.h"
#include"struct.h"
#include<math.h>
#define EMPTY 0
#define 王 1
#define 金 2
#define 銀 3
#define 桂 4
#define 香 5
#define 飛 6
#define 角 7
#define 步 8

int n_eaten_piece=0;//開啟新局模式時讓其他function知道並紀錄吃子的變數
int l_eaten_piece=0;//讀檔模式時用來紀錄吃子的變數（load moves）

/*照使用者輸入更新board陣列*/
bool move(int arr[10][10],int r1, int c1, int r2, int c2,int player){

    if(!is_valid_move(arr,r1,c1,r2,c2,player))return false;

    if (arr[r2][c2]!=0)n_eaten_piece=arr[r2][c2];//若這次的move有吃掉棋子 紀錄被吃掉的棋的編號
    arr[r2][c2]=arr[r1][c1];
    arr[r1][c1]=0;
    return true;
}

/*判斷各類棋子走法是否合格*/
bool is_valid_move(int arr[10][10],int row1, int col1,int row2, int col2,int player){ 

    // 檢查使用者輸入的起點和終點是否在棋盤上
    if (col1 < 0 || col1 >8 || col2 < 0 || col2 >8 || row1 < 1 || row1 >9 || row2 < 1 || row2 >9) return false;

    //檢查使用者是否移動對方棋子
    if (player==1&&arr[row1][col1]>10||player==2&&arr[row1][col1]<10){
        printf("You can't move the other player's piece.\n");
        return false;
    }

    //檢查起點是否有棋子，或者若目的點有棋子，檢查該點是否為己方的旗子
    if (arr[row1][col1]==0||(arr[row2][col2]!=0&&(arr[row1][col1]/10==arr[row2][col2]/10))) return false;
    
    /*判斷各種棋類走法*/
    switch (arr[row1][col1]%10)
    {
        int fixed;
    //記得y座標是由上到下遞增
    case 王:

        return abs(row1-row2)+abs(col1-col2)<3;//移動格數只可以是1或2
    case 金:

        if (row1-row2==-1&&col1!=col2) return false;//不能走斜後方的左右兩格
        return abs(row1-row2)+abs(col1-col2)<3;
    case 銀:

        if(row1-row2==1) return abs(col1-col2)<2;//前方那一排
        else if(row1-row2==-1) return abs(col1-col2)!=0;//不能走正後面
    case 桂:

        return row1-row2==2&&abs(col1-col2)==1; //只能往右前或左前
    case 香:
        
        if (col1!=col2) return false; //香車只能在同一欄上移動

        fixed=col1=col2;
        for (int i = row2+1; i <row1; i++)//從第二座標一路檢查
        {   
            if (arr[i][fixed]!=0)return false;
        }
       
    case 飛://十字走法

        if (row1!=row2&&col1!=col2)return false; //走法不是十字就不合格

        if(row1==row2){//若選擇橫移則固定的是row
            fixed=row1=row2;
            for (int i = 1; i < abs(col2-col1); i++)
            {   
                if(arr[fixed][col1+i*((col2>col1)?1:-1)]!=EMPTY)return false;
            }
        }
        else{                       //反之則為co相等，上下垂直移動
            fixed=col1=col2;
            for (int i = 1; i < abs(row2-row1); i++)
            {   
                if(arr[row1+i*((row2>row1)?1:-1)][fixed]!=EMPTY)return false;
            }
        }
        return true;
    case 角:
        
        if (abs(row2 - row1) != abs(col2 - col1)) return false;
        
        for (int i = 1; i < abs(row2 - row1); i++)
        {
            int row = row1+i*(row2<row1?-1:1);
            int col = col1+i*(col2<col1?-1:1);
            if (arr[row][col] != EMPTY) return false;
        }
        
        return true;
    case 步:
        
        return col1==col2&&row1-row2==1;

    }

}

/*宣告指標變數*/
struct Move *head,*tail=NULL,*current=NULL,*tmp;

void record_move(int r1,int c1,int r2,int c2) {
    
    struct Move *latest_move=malloc(sizeof(struct Move));

    /*紀錄數據*/
    latest_move->start_row=r1;
    latest_move->start_col=c1;
    latest_move->end_row=r2;
    latest_move->end_col=c2;
    latest_move->eaten_number=(l_eaten_piece!=0)?l_eaten_piece:n_eaten_piece;
    n_eaten_piece=0;//每一move紀錄在節點完畢後初始化eaten_piece
    if(tail==NULL){//第一次建立節點
        tail=latest_move;
        tail->previous_move=NULL;
        tail->next_move=NULL;
        head=tail;
    }
    else{  //增加節點
        tail->next_move=latest_move;
        latest_move->previous_move=tail;
        tail=latest_move;
        tail->next_move=NULL;
    } 

}

bool undo(int arr[10][10]){
    
    switch_side(arr);
    if (tail==NULL)
    {   
        switch_side(arr);
        return false;
    }
    else{
        arr[tail->start_row][tail->start_col]=arr[tail->end_row][tail->end_col];
        arr[tail->end_row][tail->end_col]=tail->eaten_number;//回復被吃掉的棋子的編號
        tmp=tail;
        tail=tail->previous_move;
        if(tail==NULL)head=tail;
        free(tmp);//free了之後curremt->next自動變為NULL
        return true;
    }
    
}

void init_board(int arr[10][10]){

    /*初始化棋盤（清空）*/  
    for (int i = 0; i < 10; i++)
    {
        for(int j=0;j<10;j++){

            arr[i][j]=0;
        }

    }
    /*開始擺放棋子*/ //記得第一列跟最後一欄是使用者座標顯示
    /*以我方角度*/
    /*敵方棋子數值+10*/
    //部署步兵
    for (int col = 0; col < 9; col++)
    {   
        //步兵位置
        arr[7][col]=步;
        arr[3][8-col]=步+10;
    }
        //角
        arr[8][1]=角;
        arr[2][7]=角+10;
        //飛
        arr[8][7]=飛;
        arr[2][1]=飛+10;
        //香
        arr[9][0]=arr[9][8]=香;
        arr[1][0]=arr[1][8]=香+10;
        //桂
        arr[9][1]=arr[9][7]=桂;
        arr[1][1]=arr[1][7]=桂+10;
        //銀
        arr[9][2]=arr[9][6]=銀;
        arr[1][6]=arr[1][2]=銀+10;
        //金
        arr[9][3]=arr[9][5]=金;
        arr[1][5]=arr[1][3]=金+10;
        //王
        arr[9][4]=王;
        arr[1][4]=王+10;

    /*顯示給使用者座標位置*/
    /* x axis*/
    for ( int col = 0; col < 9; col++)
    {
        arr[0][col]=col;
    }
     /* y axis*/
      for ( int row = 1; row < 10; row++)
    {
        arr[row][9]=row;
    }
}

/*印出棋盤*/
void show(int array[10][10])
{
    printf("\n");
    for (int row = 0; row < 10; row++)
    {
        for (int col = 0; col < 10; col++)
        {   
            
            if (row>0&&col<9)
            {
                /*color為顏色編號，bias為分別敵我*/
                /*敵方棋子數值+10*/
                unsigned int color,bias;
                if (array[row][col]>10){
                    color=34,bias=10; //敵方
                } 
                else{
                    color=31,bias=0; //我方
                }
                switch (array[row][col]-bias)
                {
                    //將字元陣列中的英文字轉換為中文字印出來
                    case 王:
                        printf("\033[%dm王\033[0m",color);
                        break;
                    case 金:
                        printf("\033[%dm金\033[0m",color);
                        break;
                    case 銀:
                        printf("\033[%dm銀\033[0m",color);
                        break; 
                    case 桂:
                        printf("\033[%dm桂\033[0m",color);
                        break;
                    case 香:
                        printf("\033[%dm香\033[0m",color);
                        break;
                    case 飛:
                        printf("\033[%dm飛\033[0m",color);
                        break;
                    case 角:
                        printf("\033[%dm角\033[0m",color);
                        break; 
                    case 步:
                        printf("\033[%dm步\033[0m",color);
                        break;
                    default:
                        printf("");
                        break;
                }
            }
            else{
                printf("%d",array[row][col]);
            } 
            printf("\t");
        }
        printf("\n");
    }
    printf("\n");
    
}

void switch_side(int arr[10][10]){
    //先將原本內容複製到暫時的陣列
    int temp[10][10];
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {  
            temp[i][j]=arr[i][j];
        }
        
    }
    //  反轉陣列
    for (int i = 1; i <10; i++)
    {
        for (int j = 0; j < 9; j++)
        {  
            arr[10-i][8-j]=temp[i][j];
        }
        
    }
}

void saveMoves(const char* filename) {

    FILE* fp = fopen(filename, "w");
    if (fp == NULL)  {
        printf("Can not write the file。\n");
    }
    else{
        int player=1;
        for (current=head; current!=NULL; current=current->next_move ) {
            
            fprintf(fp, "%d. %d %d %d %d %d\n",player, current->start_row, current->start_col, current->end_row, current->end_col,current->eaten_number);
            player=(player==1)?2:1;
        }
        fclose(fp);
        printf("Successfully saved\n");
    }
    
}



void loadMoves(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Can not access the file.\n");
        exit(0);
    }
     else if (ftell(fp)==0) //若文件為空
    {
        printf("The file is empty.\n");
        exit(0);
    }
    else{
        int row1,col1,row2,col2;
        while (fscanf(fp, "%*d. %d %d %d %d %d", &row1, &col1, &row2, &col2, &l_eaten_piece)!=EOF)
        {
            record_move(row1,col1,row2,col2);
        }     

        fclose(fp);  
        printf("Successfully load the shogi notation!\n");
    }
}



bool current_initialized=false;
int player=1;

void review_forward(int arr[10][10]){

    if (!current_initialized)
    {
        current=head;
        current_initialized=true;
        player=1;
    }
    if(current==NULL){ //如果current已經走到NULL 先把current弄到tail1並將其反轉

        printf("No furhter move\n");
        current=tail;
        if(player==1)switch_side(arr);
        arr[current->start_row][current->start_col]=arr[current->end_row][current->end_col];
        arr[current->end_row][current->end_col]=current->eaten_number;
        if(player==1)switch_side(arr);
        player=(player==1)?2:1;
    }
    if(player==2)switch_side(arr);
    arr[current->end_row][current->end_col]=arr[current->start_row][current->start_col];
    arr[current->start_row][current->start_col]=0;
    current=current->next_move;
    if(player==2)switch_side(arr);
    player=(player==1)?2:1;
    show(arr);
    
}

void review_backward(int arr[10][10]){

    if(current==NULL){  //若current已經在最尾部
            current=tail;
            if(player==1)switch_side(arr);
            arr[current->start_row][current->start_col]=arr[current->end_row][current->end_col];
            arr[current->end_row][current->end_col]=current->eaten_number;
            if(player==1)switch_side(arr);
            player=(player==1)?2:1;
            show(arr);
        }
    else{
        if(current->previous_move==NULL){ //檢查是否已經在頭部
            printf("No previous move\n");
        }
        else{ //正常狀況
            if(player==1)switch_side(arr);
            current=current->previous_move;
            arr[current->start_row][current->start_col]=arr[current->end_row][current->end_col];
            arr[current->end_row][current->end_col]=current->eaten_number;
            if(player==1)switch_side(arr);
            player=(player==1)?2:1;
            show(arr);
        }
    }
}