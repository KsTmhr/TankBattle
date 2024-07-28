#include "header.h"

// 画像読み込み用変数
GLuint back_img;
pngInfo back_img_info;
GLuint start_img[2];
pngInfo start_img_info[2];
GLuint tutorial_img[2];
pngInfo tutorial_img_info[2];
GLuint home_img[2];
pngInfo home_img_info[2];
GLuint scene_img[5];
pngInfo scene_img_info[5];
GLuint count_img[3];
pngInfo count_img_info[3];
GLuint img[MAP_IMG_NUM];
pngInfo info[MAP_IMG_NUM];
GLuint explosion_img[EXPLOSION_IMG_NUM];
pngInfo explosion_img_info[EXPLOSION_IMG_NUM];

// 全ステージのマップ情報を保持する変数
char map_data[STAGE_NUM][MAP_HEIGHT][MAP_WIDTH];
// 現在のステージのマップ情報を保持する変数
char map[MAP_HEIGHT][MAP_WIDTH];

// 各スプライトの基本データ
SPRITE_DATA default_data = {{}, {}, DEFAULT, 0, 0, 0};
SPRITE_DATA player = {{}, {}, PLAYER, PLAYER_SPEED, 0};
SPRITE_DATA enemy1 = {{}, {}, ENEMY1, ENEMY_SPEED, 0};
SPRITE_DATA enemy2 = {{}, {}, ENEMY2, ENEMY_SPEED, 0};
SPRITE_DATA enemy3 = {{}, {}, ENEMY3, ENEMY_SPEED, 0};
SPRITE_DATA enemy4 = {{}, {}, ENEMY4, ENEMY_SPEED, 0};
SPRITE_DATA enemy5 = {{}, {}, ENEMY5, ENEMY_SPEED, 0};
SPRITE_DATA enemy6 = {{}, {}, ENEMY6, ENEMY_SPEED, 0};
SPRITE_DATA normal_bullet = {{}, {}, NORMALBULLET, NORMALBULLET_SPEED, NORMALBULLET_BOUND};
SPRITE_DATA fast_bullet = {{}, {}, FASTBULLET, FASTBULLET_SPEED, 0};
SPRITE_DATA kantsu_bullet = {{}, {}, KANTSUBULLET, KANTSUBULLET_SPEED, 0};

// スプライトのインスタンスを保持する変数
SPRITE characters[CHARACTERS_NUM];
SPRITE bullets[BULLETS_NUM];

// 便利
VECTOR up = {0, -1};
VECTOR down = {0, 1};
VECTOR right = {1, 0};
VECTOR left = {-1, 0};

// 現在のシーンとステージ
SCENE scene = START;
int stage = 0;

// カーソル位置
VECTOR cursor;
// 敵周期で呼ばれたかどうか
int isEnemyTiming = 0;

// 画面の外側の大きさ
int margin_x = 0;
int margin_y = 0;

// ボタンがホバーされているかどうか
int button1_status = 0;
int button2_status = 0;

//
//  Program start
//
int main(int argc, char **argv)
{
    int i, j;
    char str[128];
    FILE *fp;

    //  一般的な準備
    glutInitWindowSize(MAP_WIDTH * 32, MAP_HEIGHT * 32);
    srandom(time(NULL));
    glutInit(&argc, argv);
    glutCreateWindow("TankBattle");
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    //  テクスチャのアルファチャネルを有効にする設定
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //  コールバック関数の登録
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutTimerFunc(SHUKI, Timer, 0);
    glutTimerFunc(ENEMY_SHUKI, EnemyTimer, 0);
    glutMouseFunc(Mouse);
    glutPassiveMotionFunc(PassiveMotion);

    // 開始時のウィンドウ位置
    glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH) / 2 - (MAP_WIDTH * 16), glutGet(GLUT_SCREEN_HEIGHT) / 2 - (MAP_HEIGHT * 16));

    // マップデータの読み込み
    for (i = 0; i < STAGE_NUM; i++)
    {
        sprintf(str, "./maps/map%d.txt", i);
        if ((fp = fopen(str, "r")) != NULL)
        {
            for (j = 0; j < MAP_HEIGHT; j++)
            {
                fgets(map_data[i][j], 256, fp);
            }
            fclose(fp);
        }
        else
        {
            printf("Couldn't open the file \"%s\"\n", str);
        }
    }

    //  PNG画像の読み込み
    back_img = pngBind("./imgs/scenes/background.png", PNG_NOMIPMAP, PNG_ALPHA, &back_img_info,
                       GL_CLAMP, GL_NEAREST, GL_NEAREST);

    for (i = 0; i < 2; i++)
    {
        sprintf(str, "./imgs/scenes/start%d.png", i);
        start_img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &start_img_info[i],
                               GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < 2; i++)
    {
        sprintf(str, "./imgs/scenes/tutorial%d.png", i);
        tutorial_img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &tutorial_img_info[i],
                                  GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < 2; i++)
    {
        sprintf(str, "./imgs/scenes/home%d.png", i);
        home_img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &home_img_info[i],
                              GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < 5; i++)
    {
        sprintf(str, "./imgs/scenes/scene%d.png", i);
        scene_img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &scene_img_info[i],
                               GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < 3; i++)
    {
        sprintf(str, "./imgs/scenes/count%d.png", i + 1);
        count_img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &count_img_info[i],
                               GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < MAP_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/scenes/%d.png", i + 1);
        img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &info[i],
                         GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < PLAYER_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/sprites/player%d.png", i);
        player.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &player.img_info[i],
                                GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < ENEMY_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/sprites/enemy1_%d.png", i);
        enemy1.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &enemy1.img_info[i],
                                GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < ENEMY_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/sprites/enemy2_%d.png", i);
        enemy2.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &enemy2.img_info[i],
                                GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < ENEMY_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/sprites/enemy3_%d.png", i);
        enemy3.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &enemy3.img_info[i],
                                GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < ENEMY_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/sprites/enemy4_%d.png", i);
        enemy4.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &enemy4.img_info[i],
                                GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < ENEMY_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/sprites/enemy5_%d.png", i);
        enemy5.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &enemy5.img_info[i],
                                GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < ENEMY_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/sprites/enemy6_%d.png", i);
        enemy6.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &enemy6.img_info[i],
                                GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < 2; i++)
    {
        sprintf(str, "./imgs/sprites/bullet1_%d.png", i);
        normal_bullet.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &normal_bullet.img_info[i],
                                       GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < 3; i++)
    {
        sprintf(str, "./imgs/sprites/bullet2_%d.png", i);
        fast_bullet.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &fast_bullet.img_info[i],
                                     GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < 3; i++)
    {
        sprintf(str, "./imgs/sprites/bullet3_%d.png", i);
        kantsu_bullet.img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &kantsu_bullet.img_info[i],
                                       GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    for (i = 0; i < EXPLOSION_IMG_NUM; i++)
    {
        sprintf(str, "./imgs/sprites/explosion%d.png", i);
        explosion_img[i] = pngBind(str, PNG_NOMIPMAP, PNG_ALPHA, &explosion_img_info[i],
                                   GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }

    //  イベントループ突入
    glutMainLoop();

    return (0);
}
