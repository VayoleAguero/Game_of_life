#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define W 80
#define H 25

#define SIZE 10000
void save_previous_state(char curs[H][W], char ps[H][W], char pps[H][W]);
void fillField(char curs[H][W], int rand_start_x, int rand_start_y, const int *start_life, int start_size);
void fillPreviousArrays(char pps[H][W], char ps[H][W], char curs[H][W]);
void drawCurrent(char curs[H][W]);
void changeState(char curs[H][W], char ps[H][W]);
void checkExitCondition(char curs[H][W], char ps[H][W], char pps[H][W], int *flag);

int main(void) {
    char curs[H][W];
    int n, count = 0;
    char c;
    int tmp[200];
    while (scanf("%d%c", &n, &c) == 2 && c == ',') {
        tmp[count] = n;
        count++;
    }
    tmp[count] = n;
    int start_life[count + 1];
    for (int i = 0; i < count + 1; i++) {
        start_life[i] = tmp[i];
    }
    // Pass data from file to game
    if (!freopen("/dev/tty", "r", stdin)) {
        perror("/dev/tty");
        exit(1);
    }
    int speed = 0;
    int check = 0;

    do {
        printf("\033[0d\033[2J");
        printf("Input speed (1 - slow 2 - fast  3 - step-by-step): ");
        check = scanf("%d", &speed);
        if ((speed < 1) || (speed) > 3) check = 0;

    } while (check != 1);

    int start_size = sizeof(start_life) / sizeof(*start_life);
    char ps[H][W];
    char pps[H][W];

    /*Fill with start values*/
    int rand_start_x = 40;
    int rand_start_y = 13;

    /*Fill field*/
    fillField(curs, rand_start_x, rand_start_y, start_life, start_size);

    /*Fill 2 previous arrays*/
    fillPreviousArrays(pps, ps, curs);
    int flag = 0;
    while (flag == 0) {
        /*Change speed*/
        if (speed == 1) usleep(500000);
        if (speed == 2) usleep(100000);
        if (speed == 3) getchar();

        printf("\033[0d\033[2J");  // clear screen

        /*Draw current*/
        drawCurrent(curs);

        /*Change state*/
        changeState(curs, ps);

        /*Exit condition*/
        checkExitCondition(curs, ps, pps, &flag);

        save_previous_state(curs, ps, pps);
    }

    return 0;
}

void fillField(char curs[H][W], int rand_start_x, int rand_start_y, const int *start_life, int start_size) {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++) {
            curs[i][j] = ' ';
            if ((i == rand_start_y) && (j == rand_start_x)) curs[i][j] = '0';
            for (int k = 2; k < start_size; k = k + 2) {
                if ((i == rand_start_y + start_life[k]) && (j == rand_start_x + start_life[k + 1]))
                    curs[i][j] = '0';
            }
        }
}

void fillPreviousArrays(char pps[H][W], char ps[H][W], char curs[H][W]) {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++) {
            pps[i][j] = curs[i][j];
            ps[i][j] = curs[i][j];
        }
}

void drawCurrent(char curs[H][W]) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            /*Field*/
            printf("%c", curs[i][j]);
            if (j == W - 1) printf("\n");
        }
    }
}

void changeState(char curs[H][W], char ps[H][W]) {
    int minus_i, plus_i, minus_j, plus_j;
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++) {
            int count = 0;

            if (i == 0)
                minus_i = H - 1;
            else
                minus_i = i - 1;
            if (i == H - 1)
                plus_i = 0;
            else
                plus_i = i + 1;
            if (j == 0)
                minus_j = W - 1;
            else
                minus_j = j - 1;
            if (j == W - 1)
                plus_j = 0;
            else
                plus_j = j + 1;

            if (ps[minus_i][minus_j] == '0') count = count + 1;
            if (ps[minus_i][j] == '0') count = count + 1;
            if (ps[minus_i][plus_j] == '0') count = count + 1;

            if (ps[i][minus_j] == '0') count = count + 1;
            if (ps[i][plus_j] == '0') count = count + 1;

            if (ps[plus_i][minus_j] == '0') count = count + 1;
            if (ps[plus_i][j] == '0') count = count + 1;
            if (ps[plus_i][plus_j] == '0') count = count + 1;

            if ((count < 2) || (count > 3))
                curs[i][j] = ' ';
            else if (count == 3)
                curs[i][j] = '0';
        }
}

void checkExitCondition(char curs[H][W], char ps[H][W], char pps[H][W], int *flag) {
    int check_exit_died = 0;
    int check_exit_const = 0;
    int check_exit_pps_const = 0;
    for (int i = 0; i < H - 1; i++)
        for (int j = 0; j < W - 1; j++) {
            if (curs[i][j] != '0') check_exit_died = check_exit_died + 1;
            if (curs[i][j] == ps[i][j]) check_exit_const = check_exit_const + 1;
            if (curs[i][j] == pps[i][j]) check_exit_pps_const = check_exit_pps_const + 1;
        }

    if ((check_exit_died >= 1896) || (check_exit_const >= 1896) || (check_exit_pps_const >= 1896)) *flag = 1;
}
void save_previous_state(char curs[H][W], char ps[H][W], char pps[H][W]) {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++) {
            pps[i][j] = ps[i][j];
            ps[i][j] = curs[i][j];
        }
}
