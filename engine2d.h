#ifndef __ENGINE_2D__
#define __ENGINE_2D__



/* 글자 속성 */
#define RESET 0
#define BRIGHT 1
#define DIM 2
#define UNDERSCORE 4
#define BLINK 5
#define REVERSE 7
#define HIDDNE 8

/* 글자색 */
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

/* 배경색 */
#define B_BLACK 40
#define B_RED 41
#define B_GREEN 42
#define B_YELLOW 43
#define B_BLUE 44
#define B_MAGENTA 45
#define B_CYAN 46
#define B_WHITE 47

char *pMapBuf;

/* 글자 색 바꾸기 */
void setColor(int fore_color, int backg_color)
{
    printf("%c[%d;%dm", 0x1b, fore_color, backg_color);
}

/* 글자 위치 바꾸기 */
void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1b, y, x);
}

/* 사각형 그리기 */
void drawMyBox(int nXpos, int nYpos, int nWidth, int nHeight, int nForColor, int nBack_Color)
{
    setColor(nForColor, nBack_Color);

    for(int ix = 1 ; ix <= nHeight ; ix++)
    {
        for(int iy = 1 ; iy <= nWidth ; iy++)
        {
            gotoxy(ix + nXpos, iy + nYpos);
            printf("  ");
        }
    }
    
    setColor(RESET, RESET);
}

/* 자동차 그리기 */
void drawMyCar(int nXpos, int nYpos)
{
    drawMyBox(nXpos, nYpos, 1, 15, BLUE, B_YELLOW);
    drawMyBox(nXpos - 1, nYpos + 1, 1, 17, BLUE, B_YELLOW);
    drawMyBox(nXpos + 1, nYpos + 1, 1, 13, BLUE, B_CYAN);
    drawMyBox(nXpos - 2, nYpos + 2, 1, 19, BLUE, B_YELLOW);
    drawMyBox(nXpos, nYpos + 2, 1, 15, BLUE, B_CYAN);
    drawMyBox(nXpos - 2, nYpos + 3, 1, 19, BLUE, B_YELLOW);
    drawMyBox(nXpos, nYpos + 3, 1, 15, BLUE, B_CYAN);
    drawMyBox(nXpos - 2, nYpos + 4, 1, 19, BLUE, B_YELLOW);
    drawMyBox(nXpos - 2, nYpos + 5, 1, 19, BLUE, B_YELLOW);
    drawMyBox(nXpos, nYpos + 5, 1, 15, BLUE, B_BLACK);
    drawMyBox(nXpos - 1, nYpos + 6, 1, 17, BLUE, B_YELLOW);
    drawMyBox(nXpos - 1, nYpos + 7, 1, 17, BLUE, B_WHITE);
    drawMyBox(nXpos + 12, nYpos + 8, 1, 3, BLUE, B_BLUE);
    drawMyBox(nXpos, nYpos + 8, 1, 3, BLUE, B_BLUE);
    drawMyBox(nXpos + 12, nYpos + 9, 1, 3, BLUE, B_BLUE);
    drawMyBox(nXpos, nYpos + 9, 1, 3, BLUE, B_BLUE);

    setColor(RESET, RESET);
}

/* 타일맵 */
void map_printCell(int rawData)
{
    switch(rawData)
    {
        case 0:             //길
            setColor(RED, B_GREEN);
            printf(" □ ");
            setColor(RESET, RESET);
            break;
        case 1:             //벽
            setColor(WHITE, B_BLACK);
            printf(" ■ ");
            setColor(RESET, RESET);
            break;
        case 2:             //출구
            setColor(RED, B_BLUE);
            printf(" ★ ");
            setColor(RESET, RESET);
            break;
        case 3:             //플레이어
            setColor(RED, B_BLACK);
            printf(" A ");
            setColor(RESET, RESET);
            break;
        case 4:             //잠긴문
            setColor(RED, B_YELLOW);
            printf(" ☆ ");
            setColor(RESET, RESET);
            break;
        case 5:             //열쇠
            setColor(GREEN, B_MAGENTA);
            printf(" § ");
            setColor(RESET, RESET);
            break;
        default :
            printf("   ");
            setColor(RESET, RESET);
            break;
    }
}

/* 전체맵 */
void map_drawAll(int *pMap)
{
    for(int i = 0 ; i < 8 ; i++)
    {
        for(int j = 0 ; j < 8 ; j++)
        {
            map_printCell(pMap[(i * 8) + j]);
        }
        puts("");
    }
}

/* 맵 불러오기 */
void readMap()
{
    int width,height;
	FILE *pf;
	pf = fopen("./map.txt","r");
	
	char buf[256];

	fgets(buf, 256, pf);

	char *pTemp = strtok(buf, ",");
	width = atoi(pTemp);
	pTemp = strtok(NULL, ",");
	height = atoi(pTemp);

	pMapBuf = (char *)malloc(width * height);
	
	int ix,iy;
	for(iy = 0 ; iy < height ; iy++) {
		fgets(buf, 256, pf);
		pTemp = strtok(buf, ",");
		pMapBuf[iy * width + 0] = atoi(pTemp);
		for(ix=1 ; ix < width ; ix++) {
			pTemp = strtok(NULL,",");
			pMapBuf[iy * width + ix] = atoi(pTemp);
		}
	}

	fclose(pf);
}

/* 키보드 입력 */
int kbhit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	} else {
		return c;
	}
}

struct termios orig_termios;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
	struct termios new_termios;

	/* take two copies - one for now, one for later */
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	/* register cleanup handler, and set the new terminal mode */
	atexit(reset_terminal_mode);
	cfmakeraw(&new_termios);
	tcsetattr(0, TCSANOW, &new_termios);
}

#endif