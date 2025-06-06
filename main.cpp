#include "main.h"
#include<stdlib.h>
using namespace std;
using namespace cv;
typedef unsigned char u8;
#define LCD_H  24
#define LCD_W  24
#define BIGGER 30
Mat src(LCD_H, LCD_W, CV_8UC3, Scalar(0));//单通道
Mat src_show(LCD_H * BIGGER, LCD_W * BIGGER, CV_8UC3, Scalar(0));//单通道
#define DELAY 400
#define R 2
#define G 1
#define B 0
#define  KEY_UP 119
#define KEY_DOWN 115
#define KEY_LEFT 97
#define KEY_RIGHT 100
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
u8 WHITE[3] = {255, 255, 255};
u8 BLACK[3] = {0, 0, 0};
u8 RED[3] = {0, 0, 255};
u8 GREEN[3] = {0, 255, 0};

#define BACK_COLOR WHITE
#define HEAD_COLOR RED
#define BODY_COLOR BLACK
#define FOOD_COLOR GREEN
int score = 0;
int body[LCD_H * LCD_W][2] = {LCD_H/2, LCD_W-2, LCD_H/2-1, LCD_W-2, LCD_H/2-1, LCD_W-1};
int body_cnt = 3;
int way = LEFT;
int aim[2];
int END_FLAG = 0;
int food[2];
void GET_KEY() {

    char k = waitKey(DELAY);
    if ((k == KEY_UP||k==KEY_UP-32)&&way!=DOWN)way = UP;
    else if ((k == KEY_DOWN||k==KEY_DOWN-32)&&way!=UP)way = DOWN;
    else if ((k == KEY_LEFT||k==KEY_LEFT-32)&&way!=RIGHT)way = LEFT;
    else if ((k == KEY_RIGHT||k==KEY_RIGHT-32)&&way!=LEFT)way = RIGHT;
}

void BIG_SHOW() {
    for (int i = 0; i < LCD_W * BIGGER; i++) {
        for (int j = 0; j < LCD_H * BIGGER; j++) {
            src_show.at<Vec3b>(i, j)[R] = src.at<Vec3b>(i / BIGGER, j / BIGGER)[R];
            src_show.at<Vec3b>(i, j)[G] = src.at<Vec3b>(i / BIGGER, j / BIGGER)[G];
            src_show.at<Vec3b>(i, j)[B] = src.at<Vec3b>(i / BIGGER, j / BIGGER)[B];

        }
    }
    imshow("SNAKE", src_show);

}

void DEL_TAIL() {
    src.at<cv::Vec3b>(body[body_cnt - 1][0], body[body_cnt - 1][1])[R] = BACK_COLOR[R];
    src.at<cv::Vec3b>(body[body_cnt - 1][0], body[body_cnt - 1][1])[G] = BACK_COLOR[G];
    src.at<cv::Vec3b>(body[body_cnt - 1][0], body[body_cnt - 1][1])[B] = BACK_COLOR[B];

}

void HEAD_FLASH() {
    src.at<cv::Vec3b>(aim[0], aim[1])[R] = HEAD_COLOR[R];
    src.at<cv::Vec3b>(aim[0], aim[1])[G] = HEAD_COLOR[G];
    src.at<cv::Vec3b>(aim[0], aim[1])[B] = HEAD_COLOR[B];

    src.at<cv::Vec3b>(body[0][0], body[0][1])[R] = BODY_COLOR[R];
    src.at<cv::Vec3b>(body[0][0], body[0][1])[G] = BODY_COLOR[G];
    src.at<cv::Vec3b>(body[0][0], body[0][1])[B] = BODY_COLOR[B];
}

void DIE_CHECK() {
    for (int i = 0; i < body_cnt; i++)
        if (body[i][0] == aim[0] && body[i][1] == aim[1]) END_FLAG = 1;
    if (aim[0] >= LCD_H) END_FLAG = 1;
    if (aim[0] < 0) END_FLAG = 1;
    if (aim[1] < 0) END_FLAG = 1;
    if (aim[1] >= LCD_W)END_FLAG = 1;

}
void INIT_BACK()
{
    for (int i = 0; i < LCD_H; i++) {
        for (int j = 0; j < LCD_W; j++) {
            src.at<cv::Vec3b>(i, j)[R] = BACK_COLOR[R];
            src.at<cv::Vec3b>(i, j)[G] = BACK_COLOR[G];;
            src.at<cv::Vec3b>(i, j)[B] = BACK_COLOR[B];;
        }
    }

}
void INIT_BODY()
{
    for (int i = 0; i < 3; i++) {
        if (i == 0) {

            src.at<cv::Vec3b>(body[i][0], body[i][1])[R] = HEAD_COLOR[R];
            src.at<cv::Vec3b>(body[i][0], body[i][1])[G] = HEAD_COLOR[G];
            src.at<cv::Vec3b>(body[i][0], body[i][1])[B] = HEAD_COLOR[B];
            continue;
        }

        src.at<cv::Vec3b>(body[i][0], body[i][1])[R] = BODY_COLOR[R];
        src.at<cv::Vec3b>(body[i][0], body[i][1])[G] = BODY_COLOR[G];
        src.at<cv::Vec3b>(body[i][0], body[i][1])[B] = BODY_COLOR[B];
    }
}
void AIM_SET()
{
    if (way == UP) {
        aim[0] = body[0][0] - 1;
        aim[1] = body[0][1];
    } else if (way == DOWN) {
        aim[0] = body[0][0] + 1;
        aim[1] = body[0][1];
    } else if (way == LEFT) {
        aim[0] = body[0][0];
        aim[1] = body[0][1] - 1;
    } else if (way == RIGHT) {
        aim[0] = body[0][0];
        aim[1] = body[0][1] + 1;
    }
}
void INIT_FOOD(int c)
{
    int FOOD_CHECK=1;

    while(FOOD_CHECK) {
        FOOD_CHECK=0;
        food[0]= rand() % LCD_H;
        food[1]= rand() % LCD_W;
        if(c==1&&aim[0]==food[0]&&aim[1]==food[1])
            {
                FOOD_CHECK=1;
                continue;

            }
        for (int i = 0; i < body_cnt; i++)
        {
          //  if (body[i][0] == food[0] && body[i][1] ==food[1]) FOOD_CHECK = 1;

            if (body[i][0] == food[0] && body[i][1] ==food[1]) {
                FOOD_CHECK = 1;
            break;
            }

        }
    }
    src.at<cv::Vec3b>(food[0], food[1])[R] = FOOD_COLOR[R];
    src.at<cv::Vec3b>(food[0], food[1])[G] = FOOD_COLOR[G];
    src.at<cv::Vec3b>(food[0], food[1])[B] = FOOD_COLOR[B];

}
void FLASH_BODY()
{
    for (int i = body_cnt - 1; i > 0; i--)body[i][0] = body[i - 1][0], body[i][1] = body[i - 1][1];
    body[0][0] = aim[0], body[0][1] = aim[1];
}
void FOOD_FLASH()
{
    if(!(food[0]==aim[0]&&food[1]==aim[1]))   DEL_TAIL();
    else {
        body_cnt++;
        score++;
        if(score==LCD_H*LCD_W-3)
        {
            END_FLAG=1;
            cout<<"YOU WIN!"<<endl;
            return ;
        }
        INIT_FOOD(1);
    }
}
int main() {

//    u8 seed,temp1,temp2,next_seed;
//    seed = 1;
//bool num[20]={0};
//    while(1) {
//        temp1 = seed ^ (seed >> 1);
//        temp2 = temp1 ^ (temp1 << 1);
//        next_seed = temp2 ^ (temp2 >> 2);
//        cout << next_seed % 24 <<"  ";
//        seed = next_seed;
//
//
//    }

    srand( (unsigned)time( NULL ) );
    INIT_BACK();
    INIT_BODY();
    INIT_FOOD(0);
    while (!END_FLAG) {
        GET_KEY();
        AIM_SET();
        DIE_CHECK();
        HEAD_FLASH();
        FOOD_FLASH();
        FLASH_BODY();
        BIG_SHOW();
    }
 cout<<"YOUR SCORE:"<<score<<endl;
    return 0;
}

