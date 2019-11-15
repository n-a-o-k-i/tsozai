#include <stdio.h>
#include <stdbool.h>

//item
typedef struct
{
  char exp[30];
  int change;
  int kind;
}Item;

//player
typedef struct
{
  int location[2];
  Item have_items[5];
  int mental_point;
  int player_hp;
  int attack_magnification;
  int through[5][5];
}Player;


//迷路の1マス
enum RoomKind {FURNITURE, EVENT, START, GOAL};    //マスの種類(道、壁、スタート、ゴール)
enum RoomFlag {FALSE, TRUE};                //通過したことがあるか

typedef struct
{
  enum RoomKind kind;            //種類(家具,イベント,スタート,ゴール)
  enum RoomFlag flag;            //通過したことがあるか
}Area;


//プレイヤー初期化
int PlayerInit(int location[2], Area room[5][5])
{
  int i, j;

  for(i = 0; i < sizeof(*room) ; i++) //行
  {
    for(j = 0;  j < sizeof(**room); j++) //列
    {
      if(room[i][j].kind == START) //スタート地点ならばプレイヤーの位置に設定する
      {
        location[0] = i;
        location[1] = j;
        return 0; //関数終了(プレイヤーを設定できれば残りの処理は必要ない)
      }
    }
  }

  //スタート地点がなければ、プレイヤーを設定できずここまでくる
  printf("スタートがありません\n");
  return -1;
}


//迷路表示
void RoomDraw(int location[2], Area room[5][5])
{
  int i, j;

  for(i = 0; i < 5; i++) //行
  {
    for(j = 0; j < 5; j++ ) //列
    {
      if(i == location[0] && j == location[1]) //プレイヤー位置
      {
        printf("P");
      }
      else if(room[i][j].flag == FALSE) //マスが判明していない場合
      {
        printf("?");
      }
      else
      {
        switch(room[i][j].kind)
        {
        case FURNITURE:
          printf("F"); break; //家具
        case EVENT:
          printf("E"); break; //イベント
        case GOAL:
          printf("G"); break; //ゴール
        default:
          printf("S"); break; //その他(道、スタート)
        }
      }
    }
    printf("\n");
  }
}


//方向
enum RoomDirection{UP, DOWN, LEFT, RIGHT, Invalid}; 

//プレイヤー移動
void PlayerMove(int location[2], Area room[5][5])
{
  char buf[100];
  int  direction = -1;

  printf("%d:上\n", UP);
  printf("%d:下\n", DOWN);
  printf("%d:左\n", LEFT);
  printf("%d:右\n", RIGHT);
  printf("数字を入力してください。：");

  fgets(buf, sizeof(buf), stdin);      //文字列で入力を読み込む
  sscanf(buf, "%d", &direction);        //数字にできる場合は変換(できなければ何もしない)

  while(direction < 0 || direction > (Invalid - 1)) //入力が正しい場合まで繰り返す
  {
    printf("入力が不正です。再入力してください:");
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d", &direction);
  }


  switch(direction){
    //上移動
  case UP:
  {
    if(location[0] - 1 >= 0) //迷路の範囲外でないことを確認
    {
      room[location[0] - 1][location[1]].flag = TRUE; //マスの種類が判明

      if(room[location[0] - 1][location[1]].kind != FURNITURE) //壁かどうか確認
      {
        location[0] -= 1; //移動
        printf("\n上に移動しました。\n");
      }
      else
      {
        printf("\n壁です。\n");
      }
    }
    else
    {
      printf("\n範囲外です\n");

    }
  }
  break;

  //下移動
  case DOWN:
  {
    if(location[0] + 1 < sizeof(*room))
    {
      room[location[0] + 1][location[1]].flag = TRUE;

      if(room[location[0] + 1][location[1]].kind != FURNITURE)
      {
        location[0] += 1;
        printf("\n下に移動しました。\n");
      }
      else
      {
        printf("\n壁です。\n");
      }
    }
    else
    {
      printf("\n範囲外です\n");

    }
  }
  break;

  //左移動
  case LEFT:
  {
    if(location[1] - 1 >= 0)
    {
      room[location[0]][location[1] - 1].flag = TRUE;

      if(room[location[0]][location[1] - 1].kind != FURNITURE)
      {
        location[1] -= 1;
        printf("\n左に移動しました。\n");
      }
      else
      {
        printf("\n壁です。\n");
      }
    }
    else
    {
      printf("\n範囲外です\n");

    }
  }
  break;

  //右移動
  case RIGHT:
  {
    if(location[1] + 1 < sizeof(**room))
    {
      room[location[0]][location[1] + 1].flag = TRUE;

      if(room[location[0]][location[1] + 1].kind != FURNITURE)
      {
        location[1] += 1;
        printf("\n右に移動しました。\n");
      }
      else
      {
        printf("\n壁です。\n");
      }
    }
    else
    {
      printf("\n範囲外です\n");

    }
  }
  break;
  }
}

//ゴール判定
int RoomGoalCheck(int location[2], Area room[5][5])
{
  if(room[location[0]][location[1]].kind == GOAL) //プレイヤー位置がゴール地点に等しい
  {
    printf("ゴール!\n");
    return 1;
  }
    return 0;
}

int main(void)
{
  //プレイヤー
  Player player;

  //迷路
  Area room[5][5] = {
      { {START, TRUE } , {FURNITURE , FALSE}, {FURNITURE , FALSE}, {FURNITURE , FALSE}, {FURNITURE , FALSE} },
      { {EVENT , FALSE} , {EVENT , FALSE}, {FURNITURE , FALSE}, {EVENT , FALSE}, {EVENT , FALSE} },
      { {EVENT , FALSE} , {FURNITURE , FALSE}, {FURNITURE , FALSE}, {FURNITURE , FALSE}, {FURNITURE , FALSE} },
      { {FURNITURE , FALSE} , {FURNITURE , FALSE}, {EVENT , FALSE}, {EVENT , FALSE}, {EVENT , FALSE} },
      { {EVENT , FALSE} , {FURNITURE , FALSE}, {FURNITURE , FALSE}, {FURNITURE , FALSE}, {GOAL , TRUE } },
    };


  //プレイヤー初期化
  if(PlayerInit(player.location, room) == -1)
  {
    //関数PlayerInitが-1を返すとき初期化に失敗している
    //よって、この時点でプログラムを終了し、迷路の表示は行わない
    return 0;
  }

  while(RoomGoalCheck(player.location, room) != 1) //ゴールするまで移動を繰り返す
  {
    //迷路表示
    RoomDraw(player.location, room);
    //プレイヤー移動
    PlayerMove(player.location, room);
  }

  //迷路最終結果表示
  RoomDraw(player.location, room);
  return 0;
}