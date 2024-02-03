#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>

#define BOARD_SIZE 15
#define WIN_LENGTH 5

char board[BOARD_SIZE][BOARD_SIZE]; // �Ֆ�

int final_row;
int final_col;
int row = 7;
int col = 7;
int attack_count=0;
unsigned int milliseconds = 5000;

// �Ֆʂ�����������֐�
void initialize_board() {
    int i,j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '-';
        }
    }
}

// �Ֆʂ�\������֐�
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

// �󔒂��ǂ����𔻒�
int check_space(int r, int c){
    if(board[r][c] == '-'){
        return 1;
    }
    return 0;
}

// �����������`�F�b�N����֐�
int check_winner() {
    int i, j, k;

    // �������̃`�F�b�N
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
            if (count == WIN_LENGTH) return 1; // ���������������𖞂�����
        }
    }

    // �c�����̃`�F�b�N
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
            if (count == WIN_LENGTH) return 1; // ���������������𖞂�����
        }
    }

    // �E�΂ߕ����̃`�F�b�N
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
            if (count == WIN_LENGTH) return 1; // ���������������𖞂�����
        }
    }

    // ���΂ߕ����̃`�F�b�N
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
            if (count == WIN_LENGTH) return 1; // ���������������𖞂�����
        }
    }

    return 0; // ���������𖞂����Ȃ�
}

// ����̋1�w�肳�ꂽ���A�����ĕ���ł��邩�𔻒�
// ����ł�����A���̑O�オ�󂢂Ă���̂Ȃ炻�̍��W��Ԃ�
int check_bother(char opponent, int num) {
    int i, j, k;
    int BOTHER_LENGTH = num;

    // �������̃`�F�b�N
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

                        printf("��������%d�A�𔭌��A����j�~\n", num);
                        printf("���u���ꏊ��%d, %d\n", row, col);
                        return 1; // ����̋�w�肳�ꂽ��������Ă���
                    }
                    if(j>=1){
                        if(board[i][j-1] == '-'){
                        row = i;
                        col = j-1;

                        printf("��������%d�A�𔭌��A�E��j�~\n", num);
                        printf("���u���ꏊ��%d, %d\n", row, col);
                        return 1; // ����̋�w�肳�ꂽ��������Ă���
                        }
                    }
                }
            } 
        }
    }

    // �c�����̃`�F�b�N
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

                        printf("�c������%d�A�𔭌��A���j�~\n", num);
                        printf("���u���ꏊ��%d, %d\n", row, col);
                        return 1; // ����̋�w�肳�ꂽ��������Ă���
                    }
                    if(j>=1){
                        if(board[j-1][i] == '-'){
                            row = j-1;
                            col = i;

                            printf("�c������%d�A�𔭌��A����j�~\n", num);
                            printf("���u���ꏊ��%d, %d\n", row, col);
                            return 1; // ����̋�w�肳�ꂽ��������Ă���
                        }
                    }
                }
            }
        }
    }

    // �E�΂ߕ����̃`�F�b�N
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

                        printf("�E�΂ߕ�����%d�A�𔭌��A�΂߉���j�~\n", num);
                        printf("���u���ꏊ��%d, %d\n", row, col);
                        return 1; // ����̋�w�肳�ꂽ��������Ă���
                    }
                    if(j-1 >= 0 && i-1 >= 0){
                        if(board[i-1][j-1] == '-'){
                            row = i-1;
                            col = j-1;

                            printf("�E�΂ߕ�����%d�A�𔭌��A�΂ߏ��j�~\n", num);
                            printf("���u���ꏊ��%d, %d\n", row, col);
                            return 1; // ����̋�w�肳�ꂽ��������Ă���
                        }
                    }
                }
            }
        }
    }

    // ���΂ߕ����̃`�F�b�N
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

                        printf("���΂ߕ�����%d�A�𔭌��A�΂߉���j�~\n", num);
                        printf("���u���ꏊ��%d, %d\n", row, col);
                        return 1; // ����̋�w�肳�ꂽ��������Ă���
                    }
                    if(i-1 >= 0 && j+1 < BOARD_SIZE){
                        if(board[i-1][j+1] == '-'){
                            row = i-1;
                            col = j+1;

                            printf("���΂ߕ�����%d�A�𔭌��A�΂ߏ��j�~\n", num);
                            printf("���u���ꏊ��%d, %d\n", row, col);
                            return 1; // ����̋�w�肳�ꂽ��������Ă���
                        }
                    }
                }
            }
        }
    }
    return 0; // �w�肳�ꂽ������͘A�����Ă��Ȃ�����
}

// ����̋��тR�A��тS�ŕ���ł��Ȃ����`�F�b�N
// �R���S����int num�Ŏw��
// ����ł�����A���̊Ԃ��󂢂Ă���Ȃ炻�̍��W��Ԃ�
int check_tobi(char opponent, int num) {
    int i, j, k;
    int BOTHER_LENGTH = num;
    int CHECK_LENGTH = num+1;

    int temp_row, temp_col; // �Ԃ̋󂢂Ă�����W���L�^

    // �������̃`�F�b�N
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

                    printf("�������ɔ��%d�𔭌�\n", num);
                    printf("���u���ꏊ��%d, %d\n", row, col);
                    return 1; // ����̋�w�肳�ꂽ��������Ă���
                }  
            } 
        }
    }

    // �c�����̃`�F�b�N
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

                    printf("�c�����ɔ��%d�𔭌�\n", num);
                    printf("���u���ꏊ��%d, %d\n", row, col);
                    return 1; // ����̋�w�肳�ꂽ��������Ă���
                }  
            } 
        }
    }

    // �E�΂ߕ����̃`�F�b�N
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

                    printf("�E�΂ߕ����ɔ��%d�𔭌�\n", num);
                    printf("���u���ꏊ��%d, %d\n", row, col);
                    return 1; // ����̋�w�肳�ꂽ��������Ă���
                }  
            }
        }
    }

    // ���΂ߕ����̃`�F�b�N
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

                    printf("���΂ߕ����ɔ��%d�𔭌�\n", num);
                    printf("���u���ꏊ��%d, %d\n", row, col);
                    return 1; // ����̋�w�肳�ꂽ��������Ă���
                }  
            }
        }
    }

    return 0; // �w�肳�ꂽ������͘A�����Ă��Ȃ�����
}

// ���̎�̊֐�
// ����̂R�A�A��тR�A�S�A�A��тS�𔭌�������j�~������ł�
int defence(){
    // �S�A�̒T��
    int check_four = check_bother('x', 4);
    printf("%d, %d\n", row, col);
    if(check_four == 1){
        return 1;
    }

    // ��тS�̒T��
    int check_four_space = check_tobi('x', 4);
    printf("%d, %d\n", row,col);
    if(check_four_space == 1){
        return 1;
    }

    // 3�A�̒T��
    int check_three = check_bother('x', 3);
    printf("%d, %d\n", row, col);
    if(check_three == 1){
        return 1;
    }
    
    // ��тR�̒T��
    int check_three_space = check_tobi('x', 3);
    printf("%d, %d\n", row,col);
    if(check_three_space == 1){
        return 1;
    }

    return 0; // �h���ׂ��ꏊ�͌�����Ȃ�����
}

// �U�߂̎���l����֐�
int offence(){
    // ��тS�̒T��
    int check_four_space = check_tobi('o', 4);
    printf("%d, %d\n", row,col);
    if(check_four_space == 1){
        return 1;
    }

    // 3�A�̒T��
    int check_three = check_bother('o', 3);
    printf("%d, %d\n", row, col);
    if(check_three == 1){
        return 1;
    }
    
    // ��тR�̒T��
    int check_three_space = check_tobi('o', 3);
    printf("%d, %d\n", row,col);
    if(check_three_space == 1){
        return 1;
    }

    return 0;
}

// �R���s���[�^�[�̎�Ԃ���������֐�
void computer_turn() {
    // �����ɃR���s���[�^�[�̎�����肷��R�[�h��}������

    // �S�A�̒T��(�����̎�j
    int check_four = check_bother('o', 4);
    printf("%d, %d\n", row, col);
    if(check_four != 1){
        int defence_flag = defence();

        // ���ׂ��ꏊ��������Ȃ������ꍇ
        if(defence_flag != 1){
            printf("�U�߂܂�\n");
            int offence_flag = offence();

            if(offence_flag != 1){
                do {
                    //�������U�߂ɕς���
                    if(attack_count==0){
                        row += 1;
                        col += 1;
                        attack_count=1;
                    }else if(attack_count==1){
                        row -= 2;
                        attack_count=0;
                    }

                    // ����Ɏ��߂�
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

                // ����Ɏ��߂�
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
    //�ڑ�����T�[�o�̏��̍\���̂�p��

    struct sockaddr_in dest;

    memset(&dest, 0, sizeof(dest));


    //�A�h���X�ƃ|�[�g�ԍ�����͂��Ă��炤
    char destination[20]; //�A�h���X
    int portNum; //�|�[�g�ԍ�

    printf("�A�h���X����͂��Ă�������\n");
    scanf("%s", destination);

    printf("�|�[�g�ԍ�����͂��Ă�������\n");
    scanf("%d", &portNum);
    dest.sin_port = htons(portNum);

    //�T�[�o�̏������

    dest.sin_family = AF_INET;

    dest.sin_addr.s_addr = inet_addr(destination);



    //�\�P�b�g�ʐM�̏����E����

    WSADATA data;

    WSAStartup(MAKEWORD(2, 0), &data);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);



    //�T�[�o�ւ̐ڑ�

    if (connect(s, (struct sockaddr*)&dest, sizeof(dest))) {

        printf("%s�ɐڑ��ł��܂���ł���\n", destination);

        return -1;

    }

    printf("%s�ɐڑ����܂���\n", destination);

    char buffer[1024];
    char name[1024];
    char msg[1024];
    char start[] = "start";
    char end[] = "end";

    initialize_board();
    srand(time(NULL)); // �����̃V�[�h��ݒ�
    int game_over = 0;
    int a, b;

    //�T�[�o����f�[�^����M
    recv(s, buffer, 1024, 0);

    //���O�̑��M
    sprintf(name, "gomagoma");
    printf("���M��%s\n\n", name);
    send(s, name, strlen(name), 0);

    //���b�Z�[�W�̑���M�̃��[�v
    while (game_over != 1) {

        // �o�b�t�@���[���ŏ�����
        memset(buffer, 0, 1024);

        //�T�[�o����f�[�^����M
        recv(s, buffer, 1024, 0);

        if (strncmp(buffer, start, strlen(start)) != 0) {
            // �G�̈���\��
            char* token = strtok(buffer, ",");
            if (token != NULL) {
                // �ŏ��̐�����int�ɕϊ�����a�Ɋi�[
                a = atoi(token);

                // ���̐�����int�ɕϊ�����b�Ɋi�[
                token = strtok(NULL, ",");
                if (token != NULL) {
                    b = atoi(token);
                }
            }
            board[b-1][a-1] = 'x';
            printf("��M�� %d,%d\n\n", a, b);
            display_board();
        }


        //�ڑ��̏I��
        if (strcmp(buffer, end) == 0) {
            printf("�ڑ����I�����܂����B\n");
            break;
        }

        //�T�[�o�Ƀf�[�^�𑗐M
        printf("\n\n���́F\n");
        //������̈����l����
        computer_turn();
        display_board();

        // ������������āAa��b�𕶎���ɕϊ�����msg�ɑ������
        // ���b�Z�[�W���T�[�o�ɑ��M
        game_over = check_winner();
        if (game_over == 1) {
            sprintf(msg, "%d,%d,win", final_col, final_row);
            printf("���M��%s\n\n", msg);
            send(s, msg, strlen(msg), 0);
            break;
        }
        else {
            sprintf(msg, "%d,%d", final_col, final_row);
            printf("���M��%s\n\n", msg);
            send(s, msg, strlen(msg), 0);
        }
    }

    display_board();
    printf("�Q�[���I���ł��B\n");

    // Windows �ł̃\�P�b�g�̏I��

    closesocket(s);
    WSACleanup();

	return 0;

}
