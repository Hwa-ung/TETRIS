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
void search_result() {
	printf("결과 찾기 미구현");
}
void print_result() {
	printf("결과 출력 미구현");
}

/*

 * 블록 모양(I, T, S, Z, L, J, O) 
 * 4*4 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 블록의 모양을 표시
 * 왼쪽, 오른쪽, 아래, 회전 
 * 4*4 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 4*4*4 배열의 3차원 배열
 */

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
// 블럭 7종류류

/* 테트리스 판을 2차원 배열로 표현
 * 2차원 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 20*8 배열
 * 모든 블록의 모양을 표시
 * 모든 블록의 모양을 표시*/
#define HEIGHT 20
#define WIDTH 10
int tetris_table[HEIGHT][WIDTH] = {0};


int block_number = 0;  /*블록 번호*/
int next_block_number = 0; /*다음 블록 번호 */
int block_state = 0; /*블록 상태, 왼쪽, 오른쪽, 아래, 회전*/
int x = 3, y = 0; /*블록의 최초 위치*/

void draw_block() {
	clear_screen();
	int i, j, bi, bj = 0;
	for(i = 0; i < HEIGHT; i++) { // 0~19
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
			if (is_block) printf("■"); // ■
			else if (tetris_table[i][j]) printf("□"); // □
			else printf(" ");
		}
		printf("\n");
	}
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
}temp_result;
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
	
		while (1) {
			draw_block();
			fflush(stdout);
		
			if (my_kbhit()) {
				char key = my_getch();
				if(key == 'p' || key == 'P') {
					printf("\n▶ 게임 종료\n");
					break;
				}
			// 여기서 key에 따라 블럭 이동/회전 처리할 예정
			// printf("키 입력: %c\n", key);
		}
		// 향후 자동 하강 루틴, 충돌 처리 등 여기에 추가
		sleep_ms(100); // 잠깐 rest
	}
	
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