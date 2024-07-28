#include "header.h"

//
// 弾丸の動き計算
//
void CalcBulletMove(SPRITE *sp, int id, int isSimuration)
{
    int j;
    char ret;
    int kantsu = 0;

    if (sp->data.type == KANTSUBULLET)
        kantsu = 1;

    // 移動
    ret = Move(&sp->pos, MaltipleVector(sp->dir, sp->data.speed), kantsu);

    // コスチューム切り替え
    if (sp->data.type == NORMALBULLET)
    {
        sp->param[0] ^= 1;
    }
    else if (sp->data.type == FASTBULLET || sp->data.type == KANTSUBULLET)
    {
        sp->param[0]++;
        if (sp->param[0] >= 3)
            sp->param[0] = 0;
    }

    // 跳ね返り
    if (ret & (RIGHTBLOCK | LEFTBLOCK))
    {
        sp->dir.x *= -1;
    }
    if (ret & (UPBLOCK | DOWNBLOCK))
    {
        sp->dir.y *= -1;
    }

    // 跳ね返り数インクリメント
    if (ret & 0x0f && sp->param[2] > 1)
    {
        sp->param[1]++;
        sp->param[2] = 0;
    }
    sp->param[2]++;

    // シュミレーション時は当たり判定等無効
    if (!isSimuration)
    {
        // 上限まで跳ねたら爆発
        if (sp->param[1] > sp->data.cnt_max)
        {
            sp->is_exploding = 1;
        }

        // 当たり判定
        for (j = id + 1; j < BULLETS_NUM; j++)
        {
            if (bullets[j].is_active)
            {
                if (abs(bullets[j].pos.x - sp->pos.x) < 32 && abs(bullets[j].pos.y - sp->pos.y) < 32)
                {
                    sp->is_exploding = 1;
                    bullets[j].is_exploding = 1;
                    break;
                }
            }
        }

        // マップの外に出たら初期化
        if (sp->pos.x < 0 || 960 < sp->pos.x || sp->pos.y < 0 || 640 < sp->pos.y)
            InitSprite(sp);
    }

    // 攻撃者自身とある程度離れたら，攻撃者自身との当たり判定有効
    if (!sp->param[4])
    {
        if (fabs(characters[sp->param[3]].pos.x - sp->pos.x) >= 40 ||
            fabs(characters[sp->param[3]].pos.y - sp->pos.y) >= 40)
        {
            sp->param[4] = 1;
        }
    }
}

//
// キャラクターの動き
//
void CalcCharaMove(SPRITE *sp, int id)
{
    int j;
    if (isEnemyTiming)
    {
        switch (sp->data.type)
        {
        case ENEMY1:
            CalcEnemy1Move(sp, id);
            break;

        case ENEMY2:
            CalcEnemy2Move(sp, id);
            break;

        case ENEMY3:
            CalcEnemy3Move(sp, id);
            break;

        case ENEMY4:
            CalcEnemy4Move(sp, id);
            break;

        case ENEMY5:
            CalcEnemy5Move(sp, id);
            break;

        case ENEMY6:
            CalcEnemy6Move(sp, id);
            break;

        default:
            break;
        }
    }

    // 当たり判定
    for (j = 0; j < BULLETS_NUM; j++)
    {
        if (bullets[j].is_active)
        {
            // 攻撃者本人への当たり判定が有効になっているか，自分が撃った以外の弾だったら
            if ((bullets[j].param[4] || bullets[j].param[3] != id) && (bullets[j].data.type != KANTSUBULLET || id == 0))
            {
                if (fabs(bullets[j].pos.x - sp->pos.x) < 32 && fabs(bullets[j].pos.y - sp->pos.y) < 32)
                {
                    bullets[j].is_exploding = 1;
                    sp->is_exploding = 1;
                    break;
                }
            }
        }
    }
}

//
// 照準の動き
//
void CalcAimMove()
{
    glLineWidth(5.0);
    // 十字を描く
    glBegin(GL_LINES);
    glVertex2i(cursor.x, cursor.y + 16);
    glVertex2i(cursor.x, cursor.y + 5);
    glVertex2i(cursor.x, cursor.y - 16);
    glVertex2i(cursor.x, cursor.y - 5);
    glVertex2i(cursor.x + 16, cursor.y);
    glVertex2i(cursor.x + 5, cursor.y);
    glVertex2i(cursor.x - 16, cursor.y);
    glVertex2i(cursor.x - 5, cursor.y);
    glEnd();

    // 点線を描く
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5, 0xf0f0);
    glBegin(GL_LINES);
    glVertex2i(characters[0].pos.x + 16 + margin_x, characters[0].pos.y + 16 + margin_y);
    glVertex2i(cursor.x, cursor.y);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

//
//  num番のPNG画像を座標(x,y)に表示する
//
void PutSprite(GLuint num, VECTOR pos, VECTOR dir, pngInfo *info)
{
    int w, h; //  テクスチャの幅と高さ
    double x2, y2;

    w = info->Width / 2; //  テクスチャの幅と高さを取得する
    h = info->Height / 2;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, num);
    glColor4ub(255, 255, 255, 255);

    glBegin(GL_QUADS); //  幅w, 高さhの四角形

    glTexCoord2i(0, 0);
    x2 = (-w * dir.x) + (h * dir.y) + w + pos.x;
    y2 = -1 * ((h * dir.x) - (-w * dir.y) - h) + pos.y;
    glVertex2i(x2 + margin_x, y2 + margin_y);

    glTexCoord2i(0, 1);
    x2 = (-w * dir.x) + (-h * dir.y) + w + pos.x;
    y2 = -1 * ((-h * dir.x) - (-w * dir.y) - h) + pos.y;
    glVertex2i(x2 + margin_x, y2 + margin_y);

    glTexCoord2i(1, 1);
    x2 = (w * dir.x) + (-h * dir.y) + w + pos.x;
    y2 = -1 * ((-h * dir.x) - (w * dir.y) - h) + pos.y;
    glVertex2i(x2 + margin_x, y2 + margin_y);

    glTexCoord2i(1, 0);
    x2 = (w * dir.x) + (h * dir.y) + w + pos.x;
    y2 = -1 * ((h * dir.x) - (w * dir.y) - h) + pos.y;
    glVertex2i(x2 + margin_x, y2 + margin_y);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//
// キャラクター移動
//
char Move(VECTOR *pos, VECTOR dir, int kantsu)
{
    char r = 0x00;
    // ブロックを無視するならそのまま移動
    if (kantsu)
    {
        pos->x += dir.x;
        pos->y += dir.y;
    }
    else
    {
        // 右移動
        if (dir.x >= 0)
        {
            if (map[(int)pos->y / 32][(int)pos->x / 32 + 1] != BLOCK &&
                ((int)pos->y % 32 == 0 || ((int)pos->y % 32 != 0 && map[(int)pos->y / 32 + 1][(int)pos->x / 32 + 1] != BLOCK)))
            {
                pos->x += dir.x;
            }
            else
            {
                r |= RIGHTBLOCK;
            }
        }
        // 左移動
        else if (dir.x < 0)
        {
            if (map[(int)pos->y / 32][((int)pos->x - 1) / 32] != BLOCK &&
                ((int)pos->y % 32 == 0 || ((int)pos->y % 32 != 0 && map[(int)pos->y / 32 + 1][((int)pos->x - 1) / 32] != BLOCK)))
            {
                pos->x += dir.x;
            }
            else
            {
                r |= LEFTBLOCK;
            }
        }
        // 下移動
        if (dir.y < 0)
        {
            if (map[((int)pos->y - 1) / 32][(int)pos->x / 32] != BLOCK &&
                ((int)pos->x % 32 == 0 || ((int)pos->x % 32 != 0 && map[((int)pos->y - 1) / 32][(int)pos->x / 32 + 1] != BLOCK)))
            {
                pos->y += dir.y;
            }
            else
            {
                r |= UPBLOCK;
            }
        }
        // 上移動
        else if (dir.y >= 0)
        {
            if (map[(int)pos->y / 32 + 1][(int)pos->x / 32] != BLOCK &&
                ((int)pos->x % 32 == 0 || ((int)pos->x % 32 != 0 && map[(int)pos->y / 32 + 1][(int)pos->x / 32 + 1] != BLOCK)))
            {
                pos->y += dir.y;
            }
            else
            {
                r |= DOWNBLOCK;
            }
        }
    }
    return r;
}

//
// 攻撃
//
void Attack(TYPE tp, VECTOR pos, VECTOR dir, int id)
{
    int i;
    for (i = 0; i < BULLETS_NUM; i++)
    {
        if (bullets[i].data.type == DEFAULT)
        {
            bullets[i].is_active = 1;
            bullets[i].pos.x = pos.x;
            bullets[i].pos.y = pos.y;
            bullets[i].param[0] = 0;  // コスチューム切り替え
            bullets[i].param[1] = 0;  // 跳ね返り回数
            bullets[i].param[2] = 0;  // 跳ね返り間隔
            bullets[i].param[3] = id; // 攻撃した人
            bullets[i].param[4] = 0;  // 攻撃した本人との当たり判定を行うかどうか
            bullets[i].status = right;
            bullets[i].dir = dir;
            switch (tp)
            {
            case NORMALBULLET:
                bullets[i].data = normal_bullet;
                break;

            case FASTBULLET:
                bullets[i].data = fast_bullet;
                break;

            case KANTSUBULLET:
                bullets[i].data = kantsu_bullet;
                break;
            }
            break;
        }
    }
}

//
// スプライト初期化
//
void InitSprite(SPRITE *sp)
{
    sp->is_active = 0;
    sp->is_exploding = 0;
    sp->data = default_data;
    sp->pos.x = 0;
    sp->pos.y = 0;
    sp->param[0] = 0;
    sp->param[1] = 0;
    sp->param[2] = 0;
    sp->param[3] = 0;
    sp->param[4] = 0;
    sp->param[5] = 0;
    sp->status = right;
    sp->dir.x = 0;
    sp->dir.y = 0;
}
