#include "header.h"

void CalcEnemy1Move(SPRITE *sp, int id)
{
    double dist, theta;
    switch (sp->param[1])
    {
    case 0:
        theta = atan2(characters[0].pos.y - sp->pos.y, characters[0].pos.x - sp->pos.x);
        sp->dir = vc(cos(theta), sin(theta));
        dist = CourseSimurate(sp->pos, sp->dir, 0);
        if (dist < 1024)
        {
            sp->param[1] = 4;
        }
        else
        {
            sp->param[1] = 1;
        }
        break;

    case 1:
        dist = SearchCourseByDir(sp, 1, 16, 0, sp->param[2], normal_bullet.cnt_max);
        if (sp->param[3] > dist)
        {
            sp->param[3] = dist;
            sp->param[4] = sp->param[2];
        }
        if (++sp->param[2] >= 16 || dist < 1024)
        {
            sp->param[1] = 2;
            sp->param[2] = -8;
        }
        break;

    case 2:
        dist = SearchCourseByDir(sp, 16, 16, sp->param[4], sp->param[2], normal_bullet.cnt_max);
        if (sp->param[3] > dist)
        {
            sp->param[3] = dist;
            sp->param[5] = sp->param[2];
        }
        if (++sp->param[2] >= 8 || dist < 1024)
        {
            if (sp->param[3] > 900000)
            {
                sp->param[1] = 0;
                sp->param[2] = 0;
            }
            else
            {
                sp->param[1] = 3;
                sp->param[2] = 0;
            }
        }
        break;
    case 3:
        sp->dir.x = cos((sp->param[4] * 16 + sp->param[5]) * M_PI / 128);
        sp->dir.y = sin((sp->param[4] * 16 + sp->param[5]) * M_PI / 128);
        sp->param[1] = 4;
        break;

    case 4:
        Attack(NORMALBULLET, sp->pos, sp->dir, id);
        sp->param[1] = 0;
        sp->param[2] = 0;
        sp->param[3] = 999999;
        sp->param[4] = 0;
        sp->param[5] = 0;
        break;

    default:
        break;
    }
}

void CalcEnemy2Move(SPRITE *sp, int id)
{
    char ret;
    double dist, theta;
    switch (sp->param[1])
    {
    case -1:
        ret = Move(&sp->pos, MaltipleVector(sp->status, 16), 0);
        if (ret & RIGHTBLOCK)
            sp->status = up;
        else if (ret & LEFTBLOCK)
            sp->status = down;
        else if (ret & UPBLOCK)
            sp->status = left;
        else if (ret & DOWNBLOCK)
            sp->status = right;

        sp->dir = sp->status;

        if (++sp->param[2] >= 10)
        {
            sp->param[1] = 0;
            sp->param[2] = 0;
        }
        break;
    case 0:
        theta = atan2(characters[0].pos.y - sp->pos.y, characters[0].pos.x - sp->pos.x);
        sp->dir = vc(cos(theta), sin(theta));
        dist = CourseSimurate(sp->pos, sp->dir, 0);
        if (dist < 1024)
        {
            sp->param[1] = 4;
        }
        else
        {
            sp->param[1] = 1;
        }
        break;
    case 1:
        dist = SearchCourseByDir(sp, 1, 8, 0, sp->param[2], normal_bullet.cnt_max);
        if (sp->param[3] > dist)
        {
            sp->param[3] = dist;
            sp->param[4] = sp->param[2];
        }
        if (++sp->param[2] >= 8 || dist < 1024)
        {
            sp->param[1] = 2;
            sp->param[2] = -4;
        }
        break;

    case 2:
        dist = SearchCourseByDir(sp, 8, 8, sp->param[4], sp->param[2], normal_bullet.cnt_max);
        if (sp->param[3] > dist)
        {
            sp->param[3] = dist;
            sp->param[5] = sp->param[2];
        }
        if (++sp->param[2] >= 4 || dist < 1024)
        {
            if (sp->param[3] > 10000)
            {
                sp->status = RandomDir();
                sp->param[1] = -1;
                sp->param[2] = 0;
            }
            else
            {
                sp->param[1] = 3;
                sp->param[2] = 0;
            }
        }
        break;
    case 3:
        sp->dir.x = cos((sp->param[4] * 8 + sp->param[5]) * M_PI / 32);
        sp->dir.y = sin((sp->param[4] * 8 + sp->param[5]) * M_PI / 32);
        sp->param[1] = 4;
        break;

    case 4:
        Attack(NORMALBULLET, sp->pos, sp->dir, id);
        sp->param[1] = 0;
        sp->param[2] = 0;
        sp->param[3] = 999999;
        sp->param[4] = 0;
        sp->param[5] = 0;
        break;

    default:
        break;
    }
}

void CalcEnemy3Move(SPRITE *sp, int id)
{
    double theta;
    switch (sp->param[1])
    {
    case 0:
        theta = atan2(characters[0].pos.y - sp->pos.y, characters[0].pos.x - sp->pos.x);
        sp->dir = vc(cos(theta), sin(theta));
        if (++sp->param[2] >= 5)
        {
            sp->param[1] = 1;
            sp->param[2] = 0;
        }
        break;

    case 1:
        if (sp->param[2] % 2 == 0)
        {
            Attack(NORMALBULLET, sp->pos, sp->dir, id);
        }
        if (++sp->param[2] >= 8)
        {
            sp->param[1] = 0;
            sp->param[2] = 0;
        }
        break;

    default:
        break;
    }
}

void CalcEnemy4Move(SPRITE *sp, int id)
{
    double theta;
    switch (sp->param[1])
    {
    case 0:
        theta = atan2(characters[0].pos.y - sp->pos.y, characters[0].pos.x - sp->pos.x);
        sp->dir = vc(cos(theta), sin(theta));

        if (sp->param[3] > 2)
        {
            sp->status = RandomDir();
            sp->param[3] = 0;
        }

        if (++sp->param[2] >= 5)
        {
            sp->param[1] = 1;
            sp->param[2] = 0;
            sp->param[3]++;
        }
        Move(&sp->pos, MaltipleVector(sp->status, 16), 0);
        break;

    case 1:
        if (sp->param[2] % 2 == 0)
        {
            Attack(NORMALBULLET, sp->pos, sp->dir, id);
        }
        if (++sp->param[2] >= 8)
        {
            sp->param[1] = 0;
            sp->param[2] = 0;
        }
        break;

    default:
        break;
    }
}

void CalcEnemy5Move(SPRITE *sp, int id)
{
    double theta;
    switch (sp->param[1])
    {
    case 0:
        theta = atan2(characters[0].pos.y - sp->pos.y, characters[0].pos.x - sp->pos.x);
        sp->dir = vc(cos(theta), sin(theta));

        if (sp->param[3] > 2)
        {
            sp->status = RandomDir();
            sp->param[3] = 0;
        }

        if (++sp->param[2] >= 15)
        {
            sp->param[1] = 1;
            sp->param[2] = 0;
            sp->param[3]++;
        }
        Move(&sp->pos, MaltipleVector(sp->status, 16), 0);
        break;

    case 1:
        Attack(FASTBULLET, sp->pos, sp->dir, id);
        sp->param[1] = 0;
        sp->param[2] = 0;
        break;

    default:
        break;
    }
}

void CalcEnemy6Move(SPRITE *sp, int id)
{
    sp->param[1]++;
    sp->dir = vc(cos(sp->param[1] * M_PI / 8), sin(sp->param[1] * M_PI / 8));
    // if (sp->param[1] % 3 == 0)
    Attack(KANTSUBULLET, sp->pos, sp->dir, id);
}

int TrailBlock(char block, int *x, int *y, int dir_x, int dir_y)
{
    int tmpx, tmpy;
    tmpx = *x;
    tmpy = *y;
    while (map[*y][*x] == block)
    {
        *x += dir_x;
        *y += dir_y;
        if (*y < 0 || MAP_HEIGHT <= *y || *x < 0 || MAP_WIDTH <= *x)
        {
            *x = tmpx;
            *y = tmpy;
            return 0;
        }
    }
    return 1;
}

int CourseSimurate(VECTOR pos, VECTOR dir, int bound)
{
    int i, dist;
    SPRITE_DATA sim;
    sim = normal_bullet;
    sim.speed = 32;

    // 暇そうな弾を探す
    for (i = 0; i < BULLETS_NUM; i++)
    {
        if (bullets[i].data.type == DEFAULT)
        {
            // 仮想弾作成
            bullets[i].is_active = 0;
            bullets[i].data = sim;
            bullets[i].pos.x = pos.x;
            bullets[i].pos.y = pos.y;
            bullets[i].param[1] = 0;
            bullets[i].param[4] = 0;
            bullets[i].status = right;
            bullets[i].dir = dir;
            while (bullets[i].param[1] <= bound)
            {
                CalcBulletMove(&bullets[i], -1, 1);

                // 自分に当たるようだったらそこはなし
                dist = ((pos.x - bullets[i].pos.x) * (pos.x - bullets[i].pos.x)) + ((pos.y - bullets[i].pos.y) * (pos.y - bullets[i].pos.y));
                if (bullets[i].param[4] && dist < 1024)
                {
                    dist = 999999;
                    break;
                }

                // 相手に当たったらそこ決定で
                dist = ((characters[0].pos.x - bullets[i].pos.x) * (characters[0].pos.x - bullets[i].pos.x)) + ((characters[0].pos.y - bullets[i].pos.y) * (characters[0].pos.y - bullets[i].pos.y));
                if (dist < 1024)
                {
                    break;
                }
            }
            InitSprite(&bullets[i]);
            break;
        }
    }
    return dist;
}

//
// 現在地，第一分割数，第二分割数，第一分割数での位置，第二分割数での位置，バウンド数
//
int SearchCourseByDir(SPRITE *sp, int div1, int div2, int a, int b, int bound)
{
    sp->dir.x = cos((a * div1 + b) * M_PI * 2 / (div1 * div2));
    sp->dir.y = sin((a * div1 + b) * M_PI * 2 / (div1 * div2));
    return CourseSimurate(sp->pos, sp->dir, bound);
}
