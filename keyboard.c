#include "header.h"

void Keyboard(unsigned char key, int x, int y)
{
    // int a = glutGetModifiers();

    switch (key)
    {
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

    case 'q':
        printf("Terminated\n");
        exit(0);
        break;

    default:
        break;
    }
    if (scene == GAME && (key == 'w' || key == 'a' || key == 's' || key == 'd') && !characters[0].is_exploding)
        if (++characters[0].param[0] >= PLAYER_IMG_NUM - 1)
            characters[0].param[0] = 0;
}

void Mouse(int b, int s, int x, int y)
{
    double theta;
    if (b == GLUT_LEFT_BUTTON)
    {
        if (s == GLUT_UP)
        {
            if (scene == GAME)
            {
                theta = atan2(y - characters[0].pos.y - margin_y, x - characters[0].pos.x - margin_x);
                characters[0].dir = vc(cos(theta), sin(theta));
                Attack(NORMALBULLET, characters[0].pos, characters[0].dir, 0);
            }

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

void PassiveMotion(int x, int y)
{
    if (scene == GAME || scene == COUNTDOWN)
    {
        double theta = atan2(y - characters[0].pos.y - margin_y, x - characters[0].pos.x - margin_x);
        characters[0].dir = vc(cos(theta), sin(theta));
        cursor = vc(x, y);
    }
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
