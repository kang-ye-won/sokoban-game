#include <stdio.h>
#include <conio.h>
#include "cursor.h"
#include <time.h>

enum { ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, BACKSPACE = 8 };
#define putchxy(x, y, c) {gotoxy(x, y);putch(c);}
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
char arStage[][18][21] = {
	{
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111000111111111111",
		"11111200111111111111",
		"11111002111111111111",
		"11100202011111111111",
		"11101011011111111111",
		"10001011011111003311",
		"10200200040000003311",
		"11111011101011003311",
		"11111000001111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
	},
	{
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11113300100000111111",
		"11113300102002001111",
		"11113300121111001111",
		"11113300004011001111",
		"11113300101002011111",
		"11111111101120201111",
		"11111102002020201111",
		"11111100001000001111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
	},
	{
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111110000041111",
		"11111111110212011111",
		"11111111110200211111",
		"11111111111202011111",
		"11111111110201011111",
		"11333300110200200111",
		"11133300002002000111",
		"11333300111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
		"11111111111111111111",
	},
};
char ns[18][21];
int nx, ny;
char* arTile[] = { "  ", "��", "��", "��", "��" };
enum { EMPTY = 48, WALL, PACK, DEPOT, MAN };
int stage = 0;

int main()
{
	int ch;
	int x, y;
	int dx, dy;
	BOOL bEnd;
	int move = 0;
	int start_time, end_time;
	double gametime = 0;
	int bex, bey, re;

	showcursor(FALSE);
	time_t start = time(NULL);
	// ��ü ���� ����
	for (;;) {
		// ������ �ʱ�ȭ
		memcpy(ns, arStage[stage], sizeof(ns));
		for (y = 0; y < 18; y++) {
			for (x = 0; x < 20; x++) {
				// ���ΰ��� ��ġ ã�� ���´�.
				if (ns[y][x] == MAN) {
					nx = x;
					ny = y;
					ns[y][x] = EMPTY;
				}
			}
		}
		clrscr();
		putsxy(45, 2, "SOKOBAN");
		putsxy(45, 4, "ESC:����");
		putsxy(45, 6, "1, 2, 3:�������� �̵�");
		putsxy(45, 12, "���Ű : BACKSPACE");

		// ���� ���� ����
		for (;;) {
			// ȭ���� �׸���.
			for (y = 0; y < 18; y++) {
				for (x = 0; x < 20; x++) {
					putsxy(x * 2, y, arTile[ns[y][x] - '0']);
				}
			}
			putsxy(nx * 2, ny, arTile[MAN - '0']);

			gotoxy(45, 8);
			printf("������ Ƚ�� : %d", move);
			gotoxy(45, 10);
			printf("��� �ð� : %lf", gametime);

			// ���ΰ� �̵�
			ch = getch();
			if (ch == 0xE0 || ch == 0) {
				ch = getch();
				dx = dy = 0;
				switch (ch) {
				case LEFT:
					dx = -1;
					break;
				case RIGHT:
					dx = 1;
					break;
				case UP:
					dy = -1;
					break;
				case DOWN:
					dy = 1;
					break;
				}
				time_t end = time(NULL);
				gametime = (double)(end - start);

				// ���� �ƴϾ�� �Ѵ�.
				if (ns[ny + dy][nx + dx] != WALL) {
					// ���� �̴� ���
					if (ns[ny + dy][nx + dx] == PACK) {
						// �� ���� ĭ�� ��� �ְų� â���� �Ѵ�. 
						if (ns[ny + dy * 2][nx + dx * 2] == EMPTY ||
							ns[ny + dy * 2][nx + dx * 2] == DEPOT) {
							if (arStage[stage][ny + dy][nx + dx] == DEPOT) {
								ns[ny + dy][nx + dx] = DEPOT;
							}
							else {
								ns[ny + dy][nx + dx] = EMPTY;
							}
							ns[ny + dy * 2][nx + dx * 2] = PACK;
						}
						else {
							dx = dy = 0;
							move--;
						}
					}
					// �� ��ġ�� �̵�
					re = 1;
					bex = nx;
					bey = ny;
					nx += dx;
					ny += dy;
					move++;
				}
				else {
					re = 0;
				}
			}
			else {
				if (ch == BACKSPACE && re != 0) {
					re--;
					if (ns[ny + dy][nx + dx] == PACK) {
						ns[bey + dy * 2][bex + dx * 2] = EMPTY;
						ns[bey + dy][bex + dx] = PACK;
					}
					nx = bex;
					ny = bey;
				}
				if (ch == ESC) {
					showcursor(TRUE);
					return;
				}
				if (ch == '1') {
					stage = 0;
					break;
				}
				if (ch == '2') {
					stage = 1;
					break;
				}
				if (ch == '3') {
					stage = 2;
					break;
				}
			}
			// ���� �� ó��
			bEnd = TRUE;
			for (y = 0; y < 18; y++) {
				for (x = 0; x < 20; x++) {
					if (ns[y][x] == DEPOT) {
						bEnd = FALSE;
					}
				}
			}

			if (bEnd) {
				clrscr();
				putsxy(10, 10, "�� ���߾��. ���� ���������� �̵��մϴ�.");
				delay(2000);
				stage++;
				if (stage == 3) stage = 0;
				break;
			}
		}
	}
}

