#include "header.h"

//
// キー入力取得
//
void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    // 移動キー
    case 'd':
        if (scene == GAME)
        {
            Move(&characters[0].pos, MaltipleVector(right, characters[0].data.speed), 0);
            characters[0].status = right;
        }
        break;

    case 'a':
        if (scene == GAME)
        {
            Move(&characters[0].pos, MaltipleVector(left, characters[0].data.speed), 0);
            characters[0].status = left;
        }
        break;

    case 'w':
        if (scene == GAME)
        {
            Move(&characters[0].pos, MaltipleVector(up, characters[0].data.speed), 0);
            characters[0].status = up;
        }
        break;

    case 's':
        if (scene == GAME)
        {
            Move(&characters[0].pos, MaltipleVector(down, characters[0].data.speed), 0);
            characters[0].status = down;
        }
        break;

    // スペースキー
    case 32:
        if (scene == OVER || scene == STAGECLEAR)
        {
            InitStage();
            glutSetCursor(GLUT_CURSOR_NONE);
            scene = COUNTDOWN;
        }
        if (scene == GAMECLEAR)
        {
            scene = START;
        }
        break;

    // 終了
    case 'q':
        printf("Terminated\n");
        exit(0);
        break;

    default:
        break;
    }

    // 画像切り替え
    if (scene == GAME && (key == 'w' || key == 'a' || key == 's' || key == 'd') && !characters[0].is_exploding)
        if (++characters[0].param[0] >= PLAYER_IMG_NUM - 1)
            characters[0].param[0] = 0;
}

//
// クリック位置取得
//
void Mouse(int b, int s, int x, int y)
{
    double theta;
    if (b == GLUT_LEFT_BUTTON)
    {
        if (s == GLUT_UP)
        {
            // ゲーム中なら射撃
            if (scene == GAME)
            {
                theta = atan2(y - characters[0].pos.y - margin_y, x - characters[0].pos.x - margin_x);
                characters[0].dir = vc(cos(theta), sin(theta));
                Attack(NORMALBULLET, characters[0].pos, characters[0].dir, 0);
            }

            // タイトル画面なら対応するボタン
            if (scene == START)
            {
                if (700 + margin_x < x && x < 907 + margin_x && 400 < y + margin_y && y < 454 + margin_y)
                {
                    InitStage();
                    glutSetCursor(GLUT_CURSOR_NONE);
                    scene = COUNTDOWN;
                }
                if (675 + margin_x < x && x < 959 + margin_x && 460 + margin_y < y && y < 514 + margin_y)
                {
                    InitStage();
                    glutSetCursor(GLUT_CURSOR_NONE);
                    scene = TUTORIAL;
                }
            }

            // チュートリアル画面の戻るボタン
            if (scene == TUTORIAL)
            {
                if (64 + margin_x < x && x < 287 + margin_x && 64 + margin_y < y && y < 118 + margin_y)
                {
                    scene = START;
                }
            }
        }
    }
}

//
// カーソル位置取得
//
void PassiveMotion(int x, int y)
{
    // ゲーム中
    if (scene == GAME || scene == COUNTDOWN)
    {
        double theta = atan2(y - characters[0].pos.y - margin_y, x - characters[0].pos.x - margin_x);
        characters[0].dir = vc(cos(theta), sin(theta));
        cursor = vc(x, y);
    }

    // マウスホバー判定
    if (scene == START)
    {
        if (700 + margin_x < x && x < 907 + margin_x && 400 + margin_y < y && y < 454 + margin_y)
        {
            button1_status = 1;
            button2_status = 0;
        }
        else if (675 + margin_x < x && x < 959 + margin_x && 460 + margin_y < y && y < 514 + margin_y)
        {
            button1_status = 0;
            button2_status = 1;
        }
        else
        {
            button1_status = 0;
            button2_status = 0;
        }
    }
    if (scene == TUTORIAL)
    {
        if (64 + margin_x < x && x < 287 + margin_x && 64 + margin_y < y && y < 118 + margin_y)
        {
            button1_status = 1;
        }
        else
        {
            button1_status = 0;
        }
    }
}
