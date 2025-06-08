#include <stdio.h>
#include <stdlib.h>
#include "platform.h"

#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

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

int game = GAME_END; /*게임 시작, 게임 종료*/

int display_menu(); /* 메뉴 표시*/
int game_start();
int check_collision (int nx, int ny, int nrot);
void move_block (int dir);
void fix_block();
void clear_lines();
int spawn_new_block();

void search_result() {
	printf("결과 찾기 미구현");
}
void print_result() {
	printf("결과 출력 미구현");
}

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

#define HEIGHT 20
#define WIDTH 10
int tetris_table[HEIGHT][WIDTH] = {0};



int block_number = 0;  /*블록 번호*/
int next_block_number = 0; /*다음 블록 번호 */
int block_state = 0; /*블록 상태, 왼쪽, 오른쪽, 아래, 회전*/
int x = 3, y = 0; /*블록의 최초 위치*/

void draw_block() {
	printf("\033[H");
	int i, j, bi, bj, b = 0;
	for(i = 0; i < HEIGHT; i++) { // 0~19
		printf("│");
		for (j = 0; j < WIDTH; j++) { // 0~9
			int is_block = 0;

			for (bi = 0; bi < 4; bi++) {
				for (bj = 0; bj < 4; bj++) {
					if(blocks[block_number][block_state][bi][bj]) {
						if (i == x + bi && j == x +bj) {
							is_block = 1;
						}
					}
				}
			}
			if (is_block) printf("🟥"); // ■
			else if (tetris_table[i][j]) printf("🟥"); // □
			else printf("  ");
		}
		printf("│\n");
	}
	printf("└");
	for(b = 0; b < WIDTH; b++) printf("──");
	printf("┘");
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

void fix_block() {
	int i, j = 0;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			if(blocks[block_number][block_state][i][j]) {
				int tx = y + 1;
				int ty = x + j;
				if (tx >= 0 && tx < HEIGHT && ty >= 0 && ty < WIDTH) tetris_table[tx][ty] = 1;
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
	block_number = rand() % 7;
	block_state = 0;
	x = 3; y = 0;
	
	if(check_collision(x, y, block_state)) return 0;
	
	return 1;
}

/* 게임 종료 때마다
 * 이름과 득점수와 
 * 날짜와 시간과 순위를 저장
 * */
static struct result
{
	char name[30];
	long point;
	int year;
	int month;
	int day;
	int hour;
	int min;
	int rank;
} temp_result;
long point = 0; /* 현재 점수*/
int best_point = 0; /* 최고 점수*/

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
	block_number = rand() % 7;
	block_state = 0;
	x = 3; y = 0;
	int drop_timer = 0;

	clear_screen();
	hide_cursor();

	while (1) {
		draw_block();
		fflush(stdout);
	
		if (my_kbhit()) {
			char key = my_getch();
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
	return 1; // 다시 메뉴로
}


/// 테트리스 게임 메인 함수
/// 메뉴를 표시하고 사용자의 선택에 따라 게임을 시작하거나 결과를 검색하거나 종료합니다.
/// @param  
/// @return 
int main(void) {
	int menu = 1;

	while(menu) {
		menu = display_menu();

		if(menu == 1)
		{
			game = GAME_START;
			menu = game_start();
		} else if(menu == 2) {
			printf("▶ [기록보기] 기능은 아직 미구현입니다.\n");
			getchar(); getchar();
			search_result();
		} else if(menu == 3) {
			printf("▶ [결과 출력] 기능은 아직 미구현입니다.\n");
			getchar(); getchar();
			print_result();
		}
		else if(menu == 4) {
			exit(0);
		}
	}
	return 0;
}