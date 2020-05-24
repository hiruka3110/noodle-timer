#ifndef _STDEF_H_
#define _STDEF_H_

typedef char B;
typedef unsigned char UB;
typedef short H;
typedef unsigned short UH;
typedef long W;
typedef unsigned long UW;

#define TRUE 1
#define FALSE 0
#define OK 1
#define NG 0
#define ON 1
#define OFF 0

//Set cathode interface
#define a 1
#define b 2
#define c 3
#define d 4
#define e 5
#define f 6
#define g 7
#define dp 8

//Set anode interface
#define d4 9
#define d3 10
#define d2 11
#define d1 12

#define sw 13   // スイッチ
#define bz  0   // ブザー

enum{
  eSTOP = 0,  // ストップ
  eCOUNT     // タイマーカウント中
};

enum{
  e3min = 0,
  e5min
};

#endif
