#include <stdio.h>
#include <stdlib.h>
#include "platform.h"

#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <locale.h>

/* 타이머  */
#define CCHAR 0
#ifdef CTIME
#undef CTIME
#endif
#define CTIME 1

/* 왼쪽, 오른쪽, 아래, 회전  */
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define ROTATE 3

/* 블록 모양 */
#define I_BLOCK 0
#define	T_BLOCK 1
#define S_BLOCK 2
#define Z_BLOCK 3
#define L_BLOCK 4
#define J_BLOCK 5
#define O_BLOCK 6

/* 게임 시작, 게임 종료 */
#define GAME_START 1
#define GAME_END 0

#define MAX_TOP 3
#define MAX_QUEUE 20

typedef struct {
	char name[30];
	long point;
	int year, month, day;
	int hour, min;
} Record;

static int cmp_desc(const void *a, const void *b) {
    const Record *A = a, *B = b;
    return (B->point - A->point) > 0 ? 1 : (B->point - A->point) < 0 ? -1 : 0;
}

int game = GAME_END; /*게임 시작, 게임 종료*/

int display_menu(); /* 메뉴 표시*/
int game_start();
void draw_block();
int check_collision (int nx, int ny, int nrot);
void move_block (int dir);
void fix_block();
void clear_lines();
int spawn_new_block();
long get_point();
void save_result(int point);
void print_result();
void search_result();
void clear_records();

char blocks[7][4][4][4] = {
	{
    		{{1, 1, 1, 1}, 	 {0, 0, 0, 0},  {0, 0, 0, 0},  {0, 0, 0, 0}},
		 	{{1, 0, 0, 0},   {1, 0, 0, 0},  {1, 0, 0, 0},  {1, 0, 0, 0} },
			{{1, 1, 1, 1}, 	 {0, 0, 0, 0},  {0, 0, 0, 0},  {0, 0, 0, 0} },
			{{1, 0, 0, 0}, 	 {1, 0, 0, 0},  {1, 0, 0, 0},  {1, 0, 0, 0} }
	},

	{
			{{1, 0, 0, 0},   {1, 1, 0, 0},   {1, 0, 0, 0},   {0, 0, 0, 0}},
			{{1, 1, 1, 0},   {0, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
			{{0, 0, 1, 0},   {0, 1, 1, 0},   {0, 0, 1, 0},   {0, 0, 0, 0}},
			{{0, 0, 0, 0},   {0, 1, 0, 0},   {1, 1, 1, 0},   {0, 0, 0, 0}}
	},

	{
			{{1, 0, 0, 0},   {1, 1, 0, 0},   {0, 1, 0, 0},   {0, 0, 0, 0}},
			{{0, 1, 1, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
			{{0, 1, 0, 0},   {0, 1, 1, 0},   {0, 0, 1, 0},   {0, 0, 0, 0}},
			{{0, 0, 0, 0},   {0, 1, 1, 0},   {1, 1, 0, 0},   {0, 0, 0, 0}}

	},

	{
			{{0, 1, 0, 0},   {1, 1, 0, 0},   {1, 0, 0, 0},   {0, 0, 0, 0}},
			{{1, 1, 0, 0},   {0, 1, 1, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
			{{0, 0, 1, 0},   {0, 1, 1, 0},   {0, 1, 0, 0},   {0, 0, 0, 0}},
			{{0, 0, 0, 0},   {1, 1, 0, 0},   {0, 1, 1, 0},   {0, 0, 0, 0}}

	},

	{
			{{1, 0, 0, 0},   {1, 0, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0}},
			{{1, 1, 1, 0},   {1, 0, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
			{{0, 1, 1, 0},   {0, 0, 1, 0},   {0, 0, 1, 0},   {0, 0, 0, 0}},
			{{0, 0, 0, 0},   {0, 0, 1, 0},   {1, 1, 1, 0},   {0, 0, 0, 0}}

	},

	{
			{{0, 1, 0, 0},   {0, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0}},
			{{1, 0, 0, 0},   {1, 1, 1, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
			{{1, 1, 0, 0},   {1, 0, 0, 0},   {1, 0, 0, 0},   {0, 0, 0, 0}},
			{{1, 1, 1, 0},   {0, 0, 1, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}}
	},

	{
			{{1, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
			{{1, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
			{{1, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}},
			{{1, 1, 0, 0},   {1, 1, 0, 0},   {0, 0, 0, 0},   {0, 0, 0, 0}}
	},
}; 
// 블럭 7종류

#define HEIGHT 21
#define WIDTH 10
int tetris_table[HEIGHT][WIDTH] = {0};

int block_number = 0;  /*블록 번호*/
int next_block_number = 0; /*다음 블록 번호 */
int block_state = 0; /*블록 상태, 왼쪽, 오른쪽, 아래, 회전*/
int x = 3, y = 2; /*블록의 최초 위치*/
int point = 0; /* 현재 점수*/
int best_point = 0; /* 최고 점수*/

#if defined(_WIN32) || defined(_WIN64)
	#define BOARD_BLOCK "[]"
	#define FALLING_BLOCK "##"
	#define FIXED_BLOCK "**"
#else
	#define BORAD_BLOCK "🟩"
	#define FALLING_BLOCK "🔳"
	#define FIXED_BLOCK "🟥"
#endif

#define BLANK "  "
/* 메뉴 표시*/
int display_menu() {
	int menu = 0;

	while(1) {
		clear_screen();
		printf("\n\n\t\t\t\tText Tetris");
		printf("\n\t\t\t============================");
		printf("\n\t\t\t\tGAME MENU\t\n");
		printf("\n\t\t\t============================");
		printf("\n\t\t\t   1) Game Start");
		printf("\n\t\t\t   2) Search history");
		printf("\n\t\t\t   3) Record Output");
		printf("\n\t\t\t   4) QUIT");
		printf("\n\t\t\t   5) Record Reset");
		printf("\n\t\t\t============================");
		printf("\n\t\t\t\t\t SELECT : ");
		scanf("%d",&menu);
		if(menu < 1 || menu > 4) {
			continue;
		}
		else {
			return menu;
		}
	}
	return 0;
}

int game_start() {
	srand(time(NULL));
	next_block_number = rand() % 7;
	block_state = 0;
	x = 3; y = 2;
	spawn_new_block();
	int drop_timer = 0;

	clear_screen();
	hide_cursor();

	while (1) {
		draw_block();
		fflush(stdout);
	
		if (my_kbhit()) {
			char key = my_getch();
			fflush(stdout);

			if(key == 'p' || key == 'P') break;
			else if (key == 'j' || key == 'J') move_block(LEFT);
			else if (key == 'l' || key == 'L') move_block(RIGHT);
			else if (key == 'k' || key == 'K') move_block(DOWN);
			else if (key == 'i' || key == 'I') move_block(ROTATE);
			else if (key == 'a' || key == 'A') {
				while (!check_collision(x, y + 1, block_state)) y++;
			}
		}
	// 일정 시간마다 자동 하강
	drop_timer++;
	if (drop_timer > 5) { // 약 0.5초
		drop_timer = 0;
		if (!check_collision(x, y + 1, block_state)) {
			y++;
		} else {
			fix_block();
			clear_lines();
			if (!spawn_new_block()) break;
		}
	}
	sleep_ms(100); // 잠깐 rest
	}
	show_cursor();
	save_result(point);
	return 1; // 다시 메뉴로
}

void draw_block() {

	printf("\033[H");
	int i, j, bi, bj, b = 0;

	printf("\n\n SCORE: %d\n", point);
	printf("NEXT BLOCK: \n");

	for(i = 0; i < 4; i++) {
		printf("  ");
		for(j = 0; j < 4; j++) {
			if(blocks[next_block_number][0][i][j]) printf(BOARD_BLOCK); //■■
			else printf("  ");
		}
		printf("\n");
	}
	printf(BOARD_BLOCK);
	for(b = 1; b < WIDTH; b++) printf("🟩"); // 🔳 ⬛ 🟪 🟩
	printf(BOARD_BLOCK BOARD_BLOCK"\n");
	
	for(i = 1; i < HEIGHT; i++) { // 1~20
		printf(BOARD_BLOCK); // ■ ■ ⬜️
		for (j = 0; j < WIDTH; j++) { // 0~9
			int is_block = 0;

			for (bi = 0; bi < 4; bi++) {
				for (bj = 0; bj < 4; bj++) {
					if(blocks[block_number][block_state][bi][bj]) {
						if (i == y + bi && j == x + bj) {
							is_block = 1;
						}
					}
				}
			}
			if (is_block) printf(FALLING_BLOCK); // █ 떨어지는 블럭 🔳 \033[36m██\033[0m
			else if (tetris_table[i][j]) printf(FIXED_BLOCK); // 고정 블럭 ⬛ \033[90m██\033[0m
			else printf("  ");
		}
		printf(BOARD_BLOCK"\n"); // 🟪 ██
	}
	printf(BOARD_BLOCK);
	for(b = 0; b < WIDTH; b++) printf(BOARD_BLOCK);
	printf(BOARD_BLOCK);
	printf("\n=========\t[I]: ROTATE\t=========");
	printf("\n[J]: Left\t[K]: Down\t[L]: Right\t[A]: Fix\n");
}

int check_collision (int nx, int ny, int nrot) {
	int i, j = 0;
	for(i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[block_number][nrot][i][j]) {
				int tx = nx + j;
				int ty = ny + i;

				if (tx < 0 || tx >= WIDTH || ty < 0 || ty >= HEIGHT)
					return 1;
				if (tetris_table[ty][tx])
					return 1;
			}
		}
	}
	return 0;
}

void move_block(int dir) {
	int nx = x;
	int ny = y;
	int nrot = block_state;

	if(dir == LEFT) nx--;
	else if (dir == RIGHT) nx++;
	else if (dir == DOWN) ny++;
	else if (dir == ROTATE)  nrot = (nrot + 1) % 4;

	if (!check_collision(nx, ny, nrot)) {
		x = nx;
		y = ny;
		block_state = nrot;
	}
}

void fix_block() {
	int i, j = 0;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			if(blocks[block_number][block_state][i][j]) {
				int ty = y + i;
				int tx = x + j;
				if (ty >= 0 && ty < HEIGHT && tx >= 0 && tx < WIDTH) tetris_table[ty][tx] = 1;
			}
		}
	}
}

void clear_lines() {
	int i, j, k = 0;
	for(i = 0; i < HEIGHT; i++) {
		int full = 1;
		for(j = 0; j < WIDTH; j++) {
			if (tetris_table[i][j] == 0) {
				full = 0;
				break;
			}
		}
		if(full) {
			get_point();
			// 아래로 한 줄씩 내리기
			for(k = i; k > 0; k--) {
				for(j = 0; j < WIDTH; j++) {
					tetris_table[k][j] = tetris_table[k-1][j];
				}
			} // 맨 윗줄 지우기
			for(j=0; j < WIDTH; j++) {
				tetris_table[0][j] = 0;
			}
		}
	}
}

int spawn_new_block() {
	block_number = next_block_number;
	next_block_number = rand() % 7;
	block_state = 0;
	x = 3; y = 2;
	
	if(check_collision(x, y, block_state)) return 0;
	
	return 1;
}

long get_point() {
	point += 100;
	return point;
}

void save_result(int score) {
    Record top[MAX_TOP];
    Record queue[MAX_QUEUE];
    int tn = 0, qn = 0;
    Record temp[MAX_TOP + MAX_QUEUE];
    int total = 0;

    // 1) 기존 파일에서 전체 읽어오기
    FILE *fp = fopen("records.txt", "r");
    if (fp) {
        while (total < MAX_TOP + MAX_QUEUE &&
               fscanf(fp, "%29s %ld %d-%d-%d %d:%d\n",
                      temp[total].name,
                      &temp[total].point,
                      &temp[total].year,
                      &temp[total].month,
                      &temp[total].day,
                      &temp[total].hour,
                      &temp[total].min) == 7) {
            total++;
        }
        fclose(fp);
    }

    // 2) temp 배열을 점수 내림차순으로 정렬
    qsort(temp, total, sizeof(Record), cmp_desc);

    // 3) 나눠담기: 앞의 MAX_TOP개 → top, 나머지 → queue
    for (int i = 0; i < total; i++) {
        if (i < MAX_TOP) {
            top[tn++] = temp[i];
        } else if (qn < MAX_QUEUE) {
            queue[qn++] = temp[i];
        }
    }

    // 4) 새 레코드 만들기
    Record newr;
    printf("\nGame over! Enter your name: ");
    scanf("%29s", newr.name);
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    newr.point = score;
    newr.year  = tm->tm_year + 1900;
    newr.month = tm->tm_mon + 1;
    newr.day   = tm->tm_mday;
    newr.hour  = tm->tm_hour;
    newr.min   = tm->tm_min;

    // 5) 새 레코드를 temp에 집어넣고 다시 정렬 & 분리
    if (total < MAX_TOP + MAX_QUEUE) {
        temp[total++] = newr;
    } else {
        // 배열이 가득 차 있으면 마지막(가장 낮은 점수) 쪽을 덮어씌움
        temp[total - 1] = newr;
    }
    qsort(temp, total, sizeof(Record), cmp_desc);

    // 재분리
    tn = qn = 0;
    for (int i = 0; i < total; i++) {
        if (i < MAX_TOP) {
            top[tn++] = temp[i];
        } else if (qn < MAX_QUEUE) {
            queue[qn++] = temp[i];
        }
    }

    // 6) 파일에 덮어쓰기
    fp = fopen("records.txt", "w");
    if (!fp) {
        perror("records.txt");
        return;
    }
    // TOP 3 먼저
    for (int i = 0; i < tn; i++) {
        fprintf(fp, "%s %ld %04d-%02d-%02d %02d:%02d\n",
                top[i].name,
                top[i].point,
                top[i].year,
                top[i].month,
                top[i].day,
                top[i].hour,
                top[i].min);
    }
    // 그 다음 큐
    for (int i = 0; i < qn; i++) {
        fprintf(fp, "%s %ld %04d-%02d-%02d %02d:%02d\n",
                queue[i].name,
                queue[i].point,
                queue[i].year,
                queue[i].month,
                queue[i].day,
                queue[i].hour,
                queue[i].min);
    }
    fclose(fp);
}

void print_result() {
    Record recs[MAX_TOP + MAX_QUEUE];
    int count = 0;

    FILE *fp = fopen("records.txt", "r");
    if (!fp) {
        printf("\n NO history. \n");
        printf("If you wanna continue Enter...");
        getchar();
        return;
    }

    // 파일에서 “이름 점수 yyyy-mm-dd hh:mm” 형식으로 최대 MAX_TOP+MAX_QUEUE개 읽음
    while (count < MAX_TOP + MAX_QUEUE &&
           fscanf(fp,
                  "%29s %ld %d-%d-%d %d:%d\n",
                  recs[count].name,
                  &recs[count].point,
                  &recs[count].year,
                  &recs[count].month,
                  &recs[count].day,
                  &recs[count].hour,
                  &recs[count].min) == 7) {
        count++;
    }
    fclose(fp);

    // 1) TOP 3 출력
    int topN = count < MAX_TOP ? count : MAX_TOP;
    printf("\n===== TOP %d HIGHSCORES =====\n\n", topN);
    for (int i = 0; i < topN; i++) {
        printf("[%d]\t%-10s\t| %6ld pts\t| %04d-%02d-%02d | %02d:%02d\n",
               i+1,
               recs[i].name,
               recs[i].point,
               recs[i].year,
               recs[i].month,
               recs[i].day,
               recs[i].hour,
               recs[i].min);
    }

    // 2) 나머지 큐 레코드 출력
    if (count > topN) {
        printf("\n===== OTHER RECORDS =====\n\n");
        for (int i = topN; i < count; i++) {
            printf("[%d]\t%-10s\t| %6ld pts\t| %04d-%02d-%02d | %02d:%02d\n",
                   i+1,
                   recs[i].name,
                   recs[i].point,
                   recs[i].year,
                   recs[i].month,
                   recs[i].day,
                   recs[i].hour,
                   recs[i].min);
        }
    }

    printf("\nIf you wanna continue Enter...");
    getchar();
}

void search_result() {
	Record r;
	FILE *fp = fopen("records.txt", "r");
	if(!fp) {
		printf("\n No history \n");
		getchar();
		return;
	}

	char target_name[30];
	printf("\n Enter the name of the player: ");
	fgets(target_name, sizeof(target_name), stdin);
	target_name[strcspn(target_name, "\n")] = '\0';
	// scanf("%s", target_name);
	printf("\n\t===== %s's record =====\t\n", target_name);

	int found = 0;
	
	while (fscanf(fp, "%s %ld %d-%d-%d %d:%d",
			r.name, &r.point, &r.year, &r.month,
			&r.day, &r.hour, &r.min) == 7) {
				if (strcmp(r.name, target_name) == 0) {
					printf(" %s\t | %ldpoint\t | %04d-%02d-%02d %02d:%02d\n", 
					r.name, r.point, r.year, r.month,
					r.day, r.hour, r.min);

					found = 1;
				}
	}
	if(!found) {
		printf(" No player histoty \n");
	}

	fclose(fp);
	printf("\nIf you wanna continue Enter...");
	getchar();
}

void clear_records() {
    FILE *fp = fopen("records.txt", "w");
    if (!fp) {
        perror("records.txt Reset Fail");
        return;
    }
    // w 모드로 열면 파일의 모든 내용이 삭제(truncate) 됩니다.
    fclose(fp);
    printf("▶ records.txt Reset Success \n");
}

/// 테트리스 게임 메인 함수
/// 메뉴를 표시하고 사용자의 선택에 따라 게임을 시작하거나 결과를 검색하거나 종료합니다.
/// @param  
/// @return 
int main(void) {
	int menu = 1;
	setlocale(LC_ALL, "");
	while(menu) {
		menu = display_menu();

		if(menu == 1)
		{
			game = GAME_START;
			menu = game_start();
		} else if(menu == 2) {
			getchar();
			search_result();
		} else if(menu == 3) {
			getchar();
			print_result();
		}
		else if(menu == 4) {
			exit(0);
		} else if (menu == 5) {
			clear_records();
		}
	}
	clear_screen();
	return 0;
}