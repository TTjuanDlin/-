#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <windows.h>

#define COL_NUM 5
#define ROW_NUM 4
#define BLOCK_W 100
#define BLOCK_H 120

int black_block_col[ROW_NUM] = { -1 };
int gread = 0;

int rand_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

void init_random_blocks() {
    for (int j = 0; j < ROW_NUM; j++) {
        black_block_col[j] = rand_range(0, 4);
    }
}

void draw_grid() {
    BeginBatchDraw();
    cleardevice();
    setlinecolor(BLACK);

    for (int i = 0; i < ROW_NUM; i++) {
        for (int j = 0; j < COL_NUM; j++) {
            setfillcolor(black_block_col[i] == j ? BLACK : WHITE);
            fillrectangle(j * BLOCK_W, i * BLOCK_H, (j + 1) * BLOCK_W, (i + 1) * BLOCK_H);
        }
    }
    EndBatchDraw();
}

void move_blocks_down() {
    for (int j = ROW_NUM - 1; j > 0; j--) {
        black_block_col[j] = black_block_col[j - 1];
    }
    black_block_col[0] = rand_range(0, 4);
}

void exitgame(int*gread) {
    char tip[128];
    sprintf_s(tip, "游戏结束你的得分是 % d\n是否重新开始", *gread);
    int res = MessageBoxA(GetHWnd(), tip, "钢琴块", MB_YESNO | MB_ICONERROR);
    if (res == IDYES) {
        *gread = 0;
        init_random_blocks();
    }
    else {
        exit(0);
    }
}

int main()
{
    initgraph(COL_NUM * BLOCK_W, ROW_NUM * BLOCK_H);
    //initgraph(COL_NUM * BLOCK_W, ROW_NUM * BLOCK_H, EX_SHOWCONSOLE);
    srand((unsigned int)time(NULL));
    init_random_blocks();
    draw_grid();
    int speed = 900;
    ExMessage m;
    int start = GetTickCount64();
    while (1)
    {


        if (peekmessage(&m, EX_MOUSE | EX_KEY))
        {
            if (m.message == WM_LBUTTONDOWN)
            {
                int col = m.x / BLOCK_W;
                if (col >= 0 && col < COL_NUM)
                {
                    if (black_block_col[3] == col)
                    {
                        printf("得分\n");
                        gread++;
                        black_block_col[3] = -1;
                    }
                    else
                    {
                        exitgame(&gread);
                    }
                }
                move_blocks_down();
                draw_grid();
                start = GetTickCount64();
            }
        }

        if (m.message == WM_KEYDOWN && m.vkcode == VK_ESCAPE) {
            closegraph();
            return 0;
        }

        if (GetTickCount64() - start > speed)
        {
            exitgame(&gread);
            draw_grid();
            start = GetTickCount64();
        }
       
    }

    closegraph();
    return 0;
}