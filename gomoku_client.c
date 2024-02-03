#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>

#define BOARD_SIZE 15
#define WIN_LENGTH 5

char board[BOARD_SIZE][BOARD_SIZE]; // 盤面

int final_row;
int final_col;
int row = 7;
int col = 7;
int attack_count=0;
unsigned int milliseconds = 5000;

// 盤面を初期化する関数
void initialize_board() {
    int i,j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }
}

// 盤面を表示する関数
void display_board() {
    printf("   ");
    int i,j;
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%2d", i + 1);
    }
    printf("\n");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%2d ", i + 1);
        for (j = 0; j < BOARD_SIZE; j++) {
            printf(" %c", board[i][j]);
        }
        printf("\n");
    }
}

// 空白かどうかを判定
int check_space(int r, int c){
    if(board[r][c] == '-'){
        return 1;
    }
    return 0;
}

// 勝利条件をチェックする関数
int check_winner() {
    int i, j, k;

    // 横方向のチェック
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j <= BOARD_SIZE - WIN_LENGTH; j++) {
            char current = board[i][j];
            if (current == '-' || current == 'x') continue;

            int count = 1;
            for (k = 1; k < WIN_LENGTH; k++) {
                if (board[i][j + k] == current) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == WIN_LENGTH) return 1; // 自分が勝利条件を満たした
        }
    }

    // 縦方向のチェック
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j <= BOARD_SIZE - WIN_LENGTH; j++) {
            char current = board[j][i];
            if (current == '-' || current == 'x') continue;

            int count = 1;
            for (k = 1; k < WIN_LENGTH; k++) {
                if (board[j + k][i] == current) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == WIN_LENGTH) return 1; // 自分が勝利条件を満たした
        }
    }

    // 右斜め方向のチェック
    for (i = 0; i <= BOARD_SIZE - WIN_LENGTH; i++) {
        for (j = 0; j <= BOARD_SIZE - WIN_LENGTH; j++) {
            char current = board[i][j];
            if (current == '-' || current == 'x') continue;

            int count = 1;
            for (k = 1; k < WIN_LENGTH; k++) {
                if (board[i + k][j + k] == current) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == WIN_LENGTH) return 1; // 自分が勝利条件を満たした
        }
    }

    // 左斜め方向のチェック
    for (i = 0; i <= BOARD_SIZE - WIN_LENGTH; i++) {
        for (j = WIN_LENGTH ; j < BOARD_SIZE; j++) {
            char current = board[i][j];
            if (current == '-' || current == 'x') continue;

            int count = 1;
            for (k = 1; k < WIN_LENGTH; k++) {
                if (board[i + k][j - k] == current) {
                    count++;
                } else {
                    break;
                }
            }
            if (count == WIN_LENGTH) return 1; // 自分が勝利条件を満たした
        }
    }

    return 0; // 勝利条件を満たさない
}

// 相手の駒が1指定された数連続して並んでいるかを判定
// 並んでいたら、その前後が空いているのならその座標を返す
int check_bother(char opponent, int num) {
    int i, j, k;
    int BOTHER_LENGTH = num;

    // 横方向のチェック
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j <= BOARD_SIZE - BOTHER_LENGTH; j++) {
            char current = board[i][j];
            if (current == opponent){
                int count = 1;
                for (k = 1; k < BOTHER_LENGTH; k++) {
                    if (board[i][j + k] == current) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count == BOTHER_LENGTH) {
                    if(board[i][j+BOTHER_LENGTH] == '-'){
                        row = i;
                        col = j+BOTHER_LENGTH;

                        printf("横方向に%d連を発見、左を阻止\n", num);
                        printf("駒を置く場所→%d, %d\n", row, col);
                        return 1; // 相手の駒が指定された数そろっていた
                    }
                    if(j>=1){
                        if(board[i][j-1] == '-'){
                        row = i;
                        col = j-1;

                        printf("横方向に%d連を発見、右を阻止\n", num);
                        printf("駒を置く場所→%d, %d\n", row, col);
                        return 1; // 相手の駒が指定された数そろっていた
                        }
                    }
                }
            } 
        }
    }

    // 縦方向のチェック
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j <= BOARD_SIZE - BOTHER_LENGTH; j++) {
            char current = board[j][i];
            if (current == opponent){
                int count = 1;
                for (k = 1; k < BOTHER_LENGTH; k++) {
                    if (board[j + k][i] == current) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count == BOTHER_LENGTH) {
                    if(board[j+BOTHER_LENGTH][i] == '-'){
                        row = j+BOTHER_LENGTH;
                        col = i;

                        printf("縦方向に%d連を発見、上を阻止\n", num);
                        printf("駒を置く場所→%d, %d\n", row, col);
                        return 1; // 相手の駒が指定された数そろっていた
                    }
                    if(j>=1){
                        if(board[j-1][i] == '-'){
                            row = j-1;
                            col = i;

                            printf("縦方向に%d連を発見、下を阻止\n", num);
                            printf("駒を置く場所→%d, %d\n", row, col);
                            return 1; // 相手の駒が指定された数そろっていた
                        }
                    }
                }
            }
        }
    }

    // 右斜め方向のチェック
    for (i = 0; i <= BOARD_SIZE - BOTHER_LENGTH; i++) {
        for (j = 0; j <= BOARD_SIZE - BOTHER_LENGTH; j++) {
            char current = board[i][j];
            if (current == opponent){
                int count = 1;
                for (k = 1; k < BOTHER_LENGTH; k++) {
                    if (board[i + k][j + k] == current) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count == BOTHER_LENGTH) {
                    if(board[i+BOTHER_LENGTH][j+BOTHER_LENGTH] == '-'){
                        row = i+BOTHER_LENGTH;
                        col = j+BOTHER_LENGTH;

                        printf("右斜め方向に%d連を発見、斜め下を阻止\n", num);
                        printf("駒を置く場所→%d, %d\n", row, col);
                        return 1; // 相手の駒が指定された数そろっていた
                    }
                    if(j-1 >= 0 && i-1 >= 0){
                        if(board[i-1][j-1] == '-'){
                            row = i-1;
                            col = j-1;

                            printf("右斜め方向に%d連を発見、斜め上を阻止\n", num);
                            printf("駒を置く場所→%d, %d\n", row, col);
                            return 1; // 相手の駒が指定された数そろっていた
                        }
                    }
                }
            }
        }
    }

    // 左斜め方向のチェック
    for (i = 0; i <= BOARD_SIZE - BOTHER_LENGTH; i++) {
        for (j = BOTHER_LENGTH ; j < BOARD_SIZE; j++) {
            char current = board[i][j];
            if (current == opponent){
                int count = 1;
                for (k = 1; k < BOTHER_LENGTH; k++) {
                    if (board[i + k][j - k] == current) {
                        count++;
                    } else {
                        break;
                    }
                }
                if (count == BOTHER_LENGTH) {
                    if(board[i+BOTHER_LENGTH][j-BOTHER_LENGTH] == '-'){
                        row = i+BOTHER_LENGTH;
                        col = j-BOTHER_LENGTH;

                        printf("左斜め方向に%d連を発見、斜め下を阻止\n", num);
                        printf("駒を置く場所→%d, %d\n", row, col);
                        return 1; // 相手の駒が指定された数そろっていた
                    }
                    if(i-1 >= 0 && j+1 < BOARD_SIZE){
                        if(board[i-1][j+1] == '-'){
                            row = i-1;
                            col = j+1;

                            printf("左斜め方向に%d連を発見、斜め上を阻止\n", num);
                            printf("駒を置く場所→%d, %d\n", row, col);
                            return 1; // 相手の駒が指定された数そろっていた
                        }
                    }
                }
            }
        }
    }
    return 0; // 指定された数分駒は連続していなかった
}

// 相手の駒が飛び３、飛び４で並んでいないかチェック
// ３か４かはint numで指定
// 並んでいたら、その間が空いているならその座標を返す
int check_tobi(char opponent, int num) {
    int i, j, k;
    int BOTHER_LENGTH = num;
    int CHECK_LENGTH = num+1;

    int temp_row, temp_col; // 間の空いている座標を記録

    // 横方向のチェック
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j <= BOARD_SIZE - CHECK_LENGTH; j++) {
            char current = board[i][j];
            if (current == opponent && board[i][j+BOTHER_LENGTH] == current){
                int count = 2;
                temp_row = -1, temp_col = -1;

                for(k = 1; k < BOTHER_LENGTH; k++) {
                    if(board[i][j + k] == current) {
                        count++;
                    }else if(board[i][j + k] == '-'){
                        temp_row = i;
                        temp_col = j+k;
                    }else{
                        break;
                    }
                }

                if(count == BOTHER_LENGTH && temp_row != -1) {
                    row = temp_row;
                    col = temp_col;

                    printf("横方向に飛び%dを発見\n", num);
                    printf("駒を置く場所→%d, %d\n", row, col);
                    return 1; // 相手の駒が指定された数そろっていた
                }  
            } 
        }
    }

    // 縦方向のチェック
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j <= BOARD_SIZE - BOTHER_LENGTH; j++) {
            char current = board[j][i];
            if (current == opponent && board[j+BOTHER_LENGTH][i] == current){
                int count = 2;
                temp_row = -1, temp_col = -1;

                for(k = 1; k < BOTHER_LENGTH; k++) {
                    if(board[j+k][i] == current) {
                        count++;
                    }else if(board[j+k][i] == '-'){
                        temp_row = j+k;
                        temp_col = i;
                    }else{
                        break;
                    }
                }

                if(count == BOTHER_LENGTH && temp_row != -1) {
                    row = temp_row;
                    col = temp_col;

                    printf("縦方向に飛び%dを発見\n", num);
                    printf("駒を置く場所→%d, %d\n", row, col);
                    return 1; // 相手の駒が指定された数そろっていた
                }  
            } 
        }
    }

    // 右斜め方向のチェック
    for (i = 0; i <= BOARD_SIZE - BOTHER_LENGTH; i++) {
        for (j = 0; j <= BOARD_SIZE - BOTHER_LENGTH; j++) {
            char current = board[i][j];
            if (current == opponent && board[i+BOTHER_LENGTH][j+BOTHER_LENGTH] == current){
                int count = 2;
                temp_row = -1, temp_col = -1;

                for(k = 1; k < BOTHER_LENGTH; k++) {
                    if(board[i+k][j+k] == current) {
                        count++;
                    }else if(board[i+k][j+k] == '-'){
                        temp_row = i+k;
                        temp_col = j+k;
                    }else{
                        break;
                    }
                }

                if(count == BOTHER_LENGTH && temp_row != -1) {
                    row = temp_row;
                    col = temp_col;

                    printf("右斜め方向に飛び%dを発見\n", num);
                    printf("駒を置く場所→%d, %d\n", row, col);
                    return 1; // 相手の駒が指定された数そろっていた
                }  
            }
        }
    }

    // 左斜め方向のチェック
    for (i = 0; i <= BOARD_SIZE - BOTHER_LENGTH; i++) {
        for (j = BOTHER_LENGTH ; j < BOARD_SIZE; j++) {
            char current = board[i][j];
            if (current == opponent && board[i+BOTHER_LENGTH][j-BOTHER_LENGTH] == current){
                int count = 2;
                temp_row = -1, temp_col = -1;

                for(k = 1; k < BOTHER_LENGTH; k++) {
                    if(board[i+k][j-k] == current) {
                        count++;
                    }else if(board[i+k][j-k] == '-'){
                        temp_row = i+k;
                        temp_col = j-k;
                    }else{
                        break;
                    }
                }

                if(count == BOTHER_LENGTH && temp_row != -1) {
                    row = temp_row;
                    col = temp_col;

                    printf("左斜め方向に飛び%dを発見\n", num);
                    printf("駒を置く場所→%d, %d\n", row, col);
                    return 1; // 相手の駒が指定された数そろっていた
                }  
            }
        }
    }

    return 0; // 指定された数分駒は連続していなかった
}

// 守りの手の関数
// 相手の３連、飛び３、４連、飛び４を発見したら阻止する手を打つ
int defence(){
    // ４連の探索
    int check_four = check_bother('x', 4);
    printf("%d, %d\n", row, col);
    if(check_four == 1){
        return 1;
    }

    // 飛び４の探索
    int check_four_space = check_tobi('x', 4);
    printf("%d, %d\n", row,col);
    if(check_four_space == 1){
        return 1;
    }

    // 3連の探索
    int check_three = check_bother('x', 3);
    printf("%d, %d\n", row, col);
    if(check_three == 1){
        return 1;
    }
    
    // 飛び３の探索
    int check_three_space = check_tobi('x', 3);
    printf("%d, %d\n", row,col);
    if(check_three_space == 1){
        return 1;
    }

    return 0; // 防ぐべき場所は見つからなかった
}

// 攻めの手を考える関数
int offence(){
    // 飛び４の探索
    int check_four_space = check_tobi('o', 4);
    printf("%d, %d\n", row,col);
    if(check_four_space == 1){
        return 1;
    }

    // 3連の探索
    int check_three = check_bother('o', 3);
    printf("%d, %d\n", row, col);
    if(check_three == 1){
        return 1;
    }
    
    // 飛び３の探索
    int check_three_space = check_tobi('o', 3);
    printf("%d, %d\n", row,col);
    if(check_three_space == 1){
        return 1;
    }

    return 0;
}

// コンピューターの手番を処理する関数
void computer_turn() {
    // ここにコンピューターの手を決定するコードを挿入する

    // ４連の探索(勝ちの手）
    int check_four = check_bother('o', 4);
    printf("%d, %d\n", row, col);
    if(check_four != 1){
        int defence_flag = defence();

        // 守るべき場所が見つからなかった場合
        if(defence_flag != 1){
            printf("攻めます\n");
            int offence_flag = offence();

            if(offence_flag != 1){
                do {
                    //ここを攻めに変える
                    if(attack_count==0){
                        row += 1;
                        col += 1;
                        attack_count=1;
                    }else if(attack_count==1){
                        row -= 2;
                        attack_count=0;
                    }

                    // 場内に収める
                    if(row > 14) row = 0;
                    if(col > 14) col = 0;
                    if(row<0) row=14;
                    if(col<0) col=14;
                } while (board[row][col] != '-');
            }
        }else{  
            while(check_space(row, col) != 1){
                col += 1;
                row-=1;

                // 場内に収める
                if(row > 14) row = 0;
                if(col > 14) col = 0;
                if(row<0) row=14;
                if(col<0) col=14;
            }
        }
    }

    printf("%d, %d\n", row, col);
    board[row][col] = 'o';
    final_row=row+1;
    final_col=col+1;
}



int main(void) {
    //接続するサーバの情報の構造体を用意

    struct sockaddr_in dest;

    memset(&dest, 0, sizeof(dest));


    //アドレスとポート番号を入力してもらう
    char destination[20]; //アドレス
    int portNum; //ポート番号

    printf("アドレスを入力してください\n");
    scanf("%s", destination);

    printf("ポート番号を入力してください\n");
    scanf("%d", &portNum);
    dest.sin_port = htons(portNum);

    //サーバの情報を入力

    dest.sin_family = AF_INET;

    dest.sin_addr.s_addr = inet_addr(destination);



    //ソケット通信の準備・生成

    WSADATA data;

    WSAStartup(MAKEWORD(2, 0), &data);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);



    //サーバへの接続

    if (connect(s, (struct sockaddr*)&dest, sizeof(dest))) {

        printf("%sに接続できませんでした\n", destination);

        return -1;

    }

    printf("%sに接続しました\n", destination);

    char buffer[1024];
    char name[1024];
    char msg[1024];
    char start[] = "start";
    char end[] = "end";

    initialize_board();
    srand(time(NULL)); // 乱数のシードを設定
    int game_over = 0;
    int a, b;

    //サーバからデータを受信
    recv(s, buffer, 1024, 0);

    //名前の送信
    sprintf(name, "gomagoma");
    printf("送信→%s\n\n", name);
    send(s, name, strlen(name), 0);

    //メッセージの送受信のループ
    while (game_over != 1) {

        // バッファをゼロで初期化
        memset(buffer, 0, 1024);

        //サーバからデータを受信
        recv(s, buffer, 1024, 0);

        if (strncmp(buffer, start, strlen(start)) != 0) {
            // 敵の一手を表示
            char* token = strtok(buffer, ",");
            if (token != NULL) {
                // 最初の数字をintに変換してaに格納
                a = atoi(token);

                // 次の数字をintに変換してbに格納
                token = strtok(NULL, ",");
                if (token != NULL) {
                    b = atoi(token);
                }
            }
            board[b-1][a-1] = 'x';
            printf("受信→ %d,%d\n\n", a, b);
            display_board();
        }


        //接続の終了
        if (strcmp(buffer, end) == 0) {
            printf("接続が終了しました。\n");
            break;
        }

        //サーバにデータを送信
        printf("\n\n入力：\n");
        //こちらの一手を考える
        computer_turn();
        display_board();

        // 勝利判定をして、aとbを文字列に変換してmsgに代入する
        // メッセージをサーバに送信
        game_over = check_winner();
        if (game_over == 1) {
            sprintf(msg, "%d,%d,win", final_col, final_row);
            printf("送信→%s\n\n", msg);
            send(s, msg, strlen(msg), 0);
            break;
        }
        else {
            sprintf(msg, "%d,%d", final_col, final_row);
            printf("送信→%s\n\n", msg);
            send(s, msg, strlen(msg), 0);
        }
    }

    display_board();
    printf("ゲーム終了です。\n");

    // Windows でのソケットの終了

    closesocket(s);
    WSACleanup();

	return 0;

}
