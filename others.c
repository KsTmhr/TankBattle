#include "header.h"

//
//  タイマー割り込みで画面を描画し直す
//
void Timer(int t)
{
    glutPostRedisplay();
    glutTimerFunc(SHUKI, Timer, 0);
}

//
//  敵の動きを計算するフラグを立てる
//
void EnemyTimer(int t)
{
    isEnemyTiming = 1;
    glutTimerFunc(ENEMY_SHUKI, EnemyTimer, 0);
}

//
//  ウィンドウのサイズ変更が発生したときに座標系を再設定する関数
//
void Reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScaled(1, -1, 1);
    glTranslated(0, -h, 0);
}

//
//  vをa倍にして返す
//
VECTOR MaltipleVector(VECTOR v, double a)
{
    VECTOR r;
    r.x = v.x * a;
    r.y = v.y * a;
    return r;
}

//
// VECTORを作る
//
VECTOR vc(double x, double y)
{
    VECTOR r;
    r.x = x;
    r.y = y;
    return r;
}

//
// ステージ初期化
//
void InitStage()
{
    int i, j, k;

    // スプライトの初期化
    for (i = 0; i < CHARACTERS_NUM; i++)
    {
        InitSprite(&characters[i]);
    }
    for (i = 0; i < BULLETS_NUM; i++)
    {
        InitSprite(&bullets[i]);
    }

    // マップロード
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            switch (map_data[stage][i][j])
            {
            // プレイヤー設定
            case 'P':
                characters[0].is_active = 1;
                characters[0].data = player;
                characters[0].pos = vc(j * 32, i * 32);
                characters[0].dir = right;
                map[i][j] = GROUND;
                break;

            // 敵設定
            case 'A':
                for (k = 1; k < CHARACTERS_NUM; k++)
                {
                    if (characters[k].data.type == DEFAULT)
                    {
                        characters[k].is_active = 1;
                        characters[k].data = enemy1;
                        characters[k].pos = vc(j * 32, i * 32);
                        characters[k].dir = right;
                        characters[k].param[1] = 0;
                        characters[k].param[3] = 999999;
                        map[i][j] = GROUND;
                        break;
                    }
                }
                break;
            case 'B':
                for (k = 1; k < CHARACTERS_NUM; k++)
                {
                    if (characters[k].data.type == DEFAULT)
                    {
                        characters[k].is_active = 1;
                        characters[k].data = enemy2;
                        characters[k].pos = vc(j * 32, i * 32);
                        characters[k].dir = right;
                        characters[k].param[1] = 0;
                        characters[k].param[3] = 999999;
                        map[i][j] = GROUND;
                        break;
                    }
                }
                break;
            case 'C':
                for (k = 1; k < CHARACTERS_NUM; k++)
                {
                    if (characters[k].data.type == DEFAULT)
                    {
                        characters[k].is_active = 1;
                        characters[k].data = enemy3;
                        characters[k].pos = vc(j * 32, i * 32);
                        characters[k].dir = right;
                        characters[k].param[1] = 0;
                        characters[k].param[2] = 0;
                        map[i][j] = GROUND;
                        break;
                    }
                }
                break;
            case 'D':
                for (k = 1; k < CHARACTERS_NUM; k++)
                {
                    if (characters[k].data.type == DEFAULT)
                    {
                        characters[k].is_active = 1;
                        characters[k].data = enemy4;
                        characters[k].pos = vc(j * 32, i * 32);
                        characters[k].dir = right;
                        characters[k].param[1] = 0;
                        characters[k].param[2] = 0;
                        characters[k].param[3] = 0;
                        characters[k].status = RandomDir();
                        map[i][j] = GROUND;
                        break;
                    }
                }
                break;
            case 'E':
                for (k = 1; k < CHARACTERS_NUM; k++)
                {
                    if (characters[k].data.type == DEFAULT)
                    {
                        characters[k].is_active = 1;
                        characters[k].data = enemy5;
                        characters[k].pos = vc(j * 32, i * 32);
                        characters[k].dir = right;
                        characters[k].param[1] = 0;
                        characters[k].param[2] = 0;
                        characters[k].param[3] = 0;
                        characters[k].status = RandomDir();
                        map[i][j] = GROUND;
                        break;
                    }
                }
                break;
            case 'F':
                for (k = 1; k < CHARACTERS_NUM; k++)
                {
                    if (characters[k].data.type == DEFAULT)
                    {
                        characters[k].is_active = 1;
                        characters[k].data = enemy6;
                        characters[k].pos = vc(j * 32, i * 32);
                        characters[k].dir = right;
                        characters[k].param[1] = 0;
                        characters[k].param[2] = 0;
                        characters[k].param[3] = 0;
                        characters[k].status = RandomDir();
                        map[i][j] = GROUND;
                        break;
                    }
                }
                break;

            default:
                map[i][j] = map_data[stage][i][j];
                break;
            }
        }
    }

    // プレイヤー位置が指定されていなかったらランダムに配置
    if (characters[0].data.type == DEFAULT)
    {
        while (map[i = random() % MAP_HEIGHT][j = random() % MAP_WIDTH] == BLOCK)
            ;
        characters[0].is_active = 1;
        characters[0].data = player;
        characters[0].pos = vc(j * 32, i * 32);
        characters[0].dir = right;
    }

    isEnemyTiming = 0;
}

//
// 爆発のアニメーション
//
void Explosion(SPRITE *sp)
{
    if (sp->param[0] >= EXPLOSION_IMG_NUM)
    {
        InitSprite(sp);
        return;
    }
    PutSprite(explosion_img[sp->param[0]],
              sp->pos, sp->dir, &explosion_img_info[sp->param[0]]);
}

//
// 上下左右のうちどれかをランダムに返す
//
VECTOR RandomDir()
{
    VECTOR dir;
    switch (random() % 4)
    {
    case 0:
        dir = right;
        break;
    case 1:
        dir = left;
        break;
    case 2:
        dir = up;
        break;
    case 3:
        dir = down;
        break;
    }
    return dir;
}
