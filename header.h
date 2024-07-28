#ifndef HEADER_H
#define HEADER_H

#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "parameters.h"

//
// スプライトの種類
//
typedef enum
{
    DEFAULT,
    PLAYER,
    ENEMY1,
    ENEMY2,
    ENEMY3,
    ENEMY4,
    ENEMY5,
    ENEMY6,
    NORMALBULLET,
    FASTBULLET,
    KANTSUBULLET,
} TYPE;

typedef enum
{
    START,
    OVER,
    STAGECLEAR,
    GAMECLEAR,
    TUTORIAL,
    GAME,
    COUNTDOWN,
} SCENE;

//
// 2次元ベクトル
//
typedef struct vctr
{
    double x;
    double y;
} VECTOR;

//
// スプライトの基本データ
//
typedef struct spd
{
    GLuint img[10];
    pngInfo img_info[10];
    TYPE type;
    int img_num;
    int speed;
    int cnt_max;
} SPRITE_DATA;

//
// スプライトのインスタンス
//
typedef struct sp
{
    int is_active;
    int is_exploding;
    SPRITE_DATA data;
    VECTOR pos;
    VECTOR dir;
    VECTOR status;
    int param[6]; // 便利変数
} SPRITE;

void Display(void);
void Reshape(int w, int h);
void Timer(int);
void EnemyTimer(int);
void PutSprite(GLuint num, VECTOR pos, VECTOR dir, pngInfo *);
void Keyboard(unsigned char, int, int);
void Mouse(int b, int s, int x, int y);
char Move(VECTOR *pos, VECTOR dir, int kantsu);
void PassiveMotion(int x, int y);
void Attack(TYPE bullet, VECTOR pos, VECTOR dir, int id);
void InitSprite(SPRITE *sp);
void InitStage();
VECTOR vc(double x, double y);
VECTOR MaltipleVector(VECTOR v, double a);
void CalcBulletMove(SPRITE *sp, int id, int isSimuration);
void CalcCharaMove(SPRITE *sp, int id);
void CalcAimMove();
void CalcEnemy1Move(SPRITE *sp, int id);
void CalcEnemy2Move(SPRITE *sp, int id);
void CalcEnemy3Move(SPRITE *sp, int id);
void CalcEnemy4Move(SPRITE *sp, int id);
void CalcEnemy5Move(SPRITE *sp, int id);
void CalcEnemy6Move(SPRITE *sp, int id);
int TrailBlock(char block, int *x, int *y, int dir_x, int dir_y);
int CourseSimurate(VECTOR pos, VECTOR dir, int bound);
int SearchCourseByDir(SPRITE *sp, int div1, int div2, int a, int b, int bound);
void Explosion(SPRITE *sp);
VECTOR RandomDir();

//  PNG画像を読み込むための変数
extern GLuint back_img;
extern pngInfo back_img_info;
extern GLuint start_img[2];
extern pngInfo start_img_info[2];
extern GLuint tutorial_img[2];
extern pngInfo tutorial_img_info[2];
extern GLuint home_img[2];
extern pngInfo home_img_info[2];
extern GLuint scene_img[5];
extern pngInfo scene_img_info[5];
extern GLuint count_img[3];
extern pngInfo count_img_info[3];
extern GLuint img[MAP_IMG_NUM];
extern pngInfo info[MAP_IMG_NUM];
extern GLuint explosion_img[EXPLOSION_IMG_NUM];
extern pngInfo explosion_img_info[EXPLOSION_IMG_NUM];
extern char map_data[STAGE_NUM][MAP_HEIGHT][MAP_WIDTH];
extern char map[MAP_HEIGHT][MAP_WIDTH];
extern SPRITE_DATA default_data;
extern SPRITE_DATA player;
extern SPRITE_DATA enemy1;
extern SPRITE_DATA enemy2;
extern SPRITE_DATA enemy3;
extern SPRITE_DATA enemy4;
extern SPRITE_DATA enemy5;
extern SPRITE_DATA enemy6;
extern SPRITE_DATA normal_bullet;
extern SPRITE_DATA fast_bullet;
extern SPRITE_DATA kantsu_bullet;
extern SPRITE characters[CHARACTERS_NUM];
extern SPRITE bullets[BULLETS_NUM];
extern VECTOR up;
extern VECTOR down;
extern VECTOR right;
extern VECTOR left;
extern SCENE scene;
extern VECTOR cursor;
extern int stage;
extern int isCusorTiming;
extern int isBulletTiming;
extern int isEnemyTiming;
extern int isMoveTiming;
extern int margin_x;
extern int margin_y;
extern int button1_status;
extern int button2_status;

#endif