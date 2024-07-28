#include "header.h"

//
//  ウィンドウの表示内容を更新する
//
void Display(void)
{
    int w, h;
    int i, j;
    int enemy_num;
    static int cnt = 0;

    // ウィンドウサイズ取得
    w = glutGet(GLUT_WINDOW_WIDTH);
    h = glutGet(GLUT_WINDOW_HEIGHT);

    // 画面の外側のサイズ計算
    margin_x = (w - (MAP_WIDTH * 32)) / 2;
    margin_y = (h - (MAP_HEIGHT * 32)) / 2;

    // 規定より小さかったら戻す
    if (w < MAP_WIDTH * 32)
    {
        w = MAP_WIDTH * 32;
        glutReshapeWindow(w, h);
    }
    if (h < MAP_HEIGHT * 32)
    {
        h = MAP_HEIGHT * 32;
        glutReshapeWindow(w, h);
    }

    // 背景色塗りつぶし（黒）
    glClear(GL_COLOR_BUFFER_BIT);

    // ゲーム中以外は対応する画像表示
    if (scene != GAME && scene != COUNTDOWN)
    {
        PutSprite(back_img, vc(0, 0), right, &back_img_info);
        PutSprite(scene_img[scene], vc(0, 0), right, &scene_img_info[scene]);
    }

    // シーンごと
    switch (scene)
    {
    // カーソル再表示とボタンの配置
    case START:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        PutSprite(start_img[button1_status], vc(700, 400), right, &start_img_info[button1_status]);
        PutSprite(tutorial_img[button2_status], vc(675, 460), right, &tutorial_img_info[button2_status]);
        break;
    case TUTORIAL:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        PutSprite(home_img[button1_status], vc(64, 64), right, &home_img_info[button1_status]);
        break;

    // 表示だけ，キャラは動かさない
    case COUNTDOWN:
        // マップ描画
        for (i = 0; i < MAP_HEIGHT; i++)
        {
            for (j = 0; j < MAP_WIDTH; j++)
            {
                PutSprite(img[map[i][j] - '1'], vc(j * 32, i * 32), right, &info[map[i][j] - '1']);
            }
        }
        // キャラクター描画
        for (i = 0; i < CHARACTERS_NUM; i++)
        {
            if (characters[i].is_active)
            {
                PutSprite(characters[i].data.img[characters[i].param[0] + 1],
                          characters[i].pos, characters[i].status, &characters[i].data.img_info[characters[i].param[0] + 1]);
                PutSprite(characters[i].data.img[0],
                          characters[i].pos, characters[i].dir, &characters[i].data.img_info[0]);
            }
        }

        PutSprite(count_img[2 - (cnt / 10)],
                  vc(380, 220), right, &count_img_info[2 - (cnt / 10)]);

        if (isEnemyTiming)
            cnt++;

        // 3秒たったらゲームスタート
        if (cnt >= 30)
        {
            cnt = 0;
            scene = GAME;
        }

        break;

    case GAME:

        // マップ描画
        for (i = 0; i < MAP_HEIGHT; i++)
        {
            for (j = 0; j < MAP_WIDTH; j++)
            {
                PutSprite(img[map[i][j] - '1'], vc(j * 32, i * 32), right, &info[map[i][j] - '1']);
            }
        }

        // キャラクター描画
        enemy_num = 0; // 生きてる敵の数カウンター
        for (i = 0; i < CHARACTERS_NUM; i++)
        {
            if (characters[i].is_active)
            {
                if (!characters[i].is_exploding)
                {
                    // キャラ移動 & 表示
                    CalcCharaMove(&characters[i], i);

                    PutSprite(characters[i].data.img[characters[i].param[0] + 1],
                              characters[i].pos, characters[i].status, &characters[i].data.img_info[characters[i].param[0] + 1]);
                    PutSprite(characters[i].data.img[0],
                              characters[i].pos, characters[i].dir, &characters[i].data.img_info[0]);
                }
                else
                {
                    // 爆発
                    if (isEnemyTiming)
                    {
                        characters[i].param[0]++;
                    }
                    Explosion(&characters[i]);
                }
                enemy_num++;
            }
        }

        // プレイヤーが死んでいたらゲームオーバー
        if (characters[0].data.type == DEFAULT)
        {
            scene = OVER;
        }
        // 場にキャラが1体だけになったら（=敵がいなくなったら）ステージクリア
        else if (enemy_num == 1)
        {
            scene = STAGECLEAR;
            if (++stage >= STAGE_NUM)
            {
                scene = GAMECLEAR;
            }
            InitStage();
        }

        // 弾描画
        for (i = 0; i < BULLETS_NUM; i++)
        {
            if (bullets[i].is_active)
            {
                if (!bullets[i].is_exploding)
                {
                    // 弾丸移動 & 表示
                    CalcBulletMove(&bullets[i], i, 0);
                    PutSprite(bullets[i].data.img[bullets[i].param[0]],
                              bullets[i].pos, bullets[i].dir, &bullets[i].data.img_info[bullets[i].param[0]]);
                }
                else
                {
                    // 爆発
                    if (isEnemyTiming)
                    {
                        bullets[i].param[0]++;
                    }
                    Explosion(&bullets[i]);
                }
            }
        }

        // カーソル描画
        CalcAimMove();
        break;

    case OVER:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        break;

    case STAGECLEAR:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        break;

    case GAMECLEAR:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

        // ランダムな位置に爆発アニメーション表示
        if (isEnemyTiming)
        {
            InitSprite(&characters[++cnt % CHARACTERS_NUM]);
            characters[cnt % CHARACTERS_NUM].is_active = 1;
            characters[cnt % CHARACTERS_NUM].pos = vc(random() % 960, random() % 640);
            characters[cnt % CHARACTERS_NUM].dir = right;
        }

        for (i = 0; i < CHARACTERS_NUM; i++)
        {
            if (characters[i].is_active)
            {
                if (isEnemyTiming)
                {
                    characters[i].param[0]++;
                }
                Explosion(&characters[i]);
            }
        }
        stage = 0;
        break;

    default:
        break;
    }

    // 周期判定リセット
    if (isEnemyTiming)
    {
        isEnemyTiming = 0;
    }

    glFlush();
}
