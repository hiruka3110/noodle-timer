#include <MsTimer2.h>            // タイマー割り込みを利用する為に必要なヘッダファイル
#include "stdef.h"

UH time = 180;  // 3分=180秒
UB status = eSTOP;  // 状態(ストップ/カウント)
UB mode = e3min;
UB buzzer = OFF;

UB cnt_ok = NG;  // カウント許可
UB key_nml = FALSE; // スイッチ押下
UB key_lng = FALSE; // スイッチ長押し


/* -------------------------
 * 割り込み関数
 * 10ms毎にこの関数に入る
   -------------------------*/
void flash() {
  
  static UB tim1s = 0;    // カウントダウン１秒タイマー
  static UB key = 0;
  
  static UB key_cnt = 0;  // スイッチ押下カウント
  static UB before = LOW; // スイッチ前状態
  static UB flg_edge = FALSE; // 立上り検出フラグ

  static UH buz = 0;  // ブザーカウント

  /* --------------------
      カウントダウン処理   */
  tim1s++;

  if(tim1s >= 100){  // 10ms * 100 = 1sec経過
    if(time > 0 && cnt_ok == OK){
        time--;   // カウントダウン
    }  
    tim1s = 0;
  }

  /* -----------------
      スイッチ処理    */

  if(digitalRead(13) == HIGH){
    if(key_cnt < 0xff){
      key_cnt++;
    }
  }

  if(before == LOW && digitalRead(13) == HIGH){ // 立上り検出
    flg_edge = TRUE;
  }

  if(flg_edge == TRUE && digitalRead(13) == LOW){ // スイッチ押下後、スイッチを離した
    if(key_cnt > 5){
      key_nml = TRUE; // スイッチ押下フラグ
    }
    flg_edge = FALSE;
  }

  if(flg_edge == TRUE && key_cnt > 100){    // スイッチ押下後、1秒間スイッチを押し続けた
    key_lng = TRUE;   // スイッチ長押しフラグ
    flg_edge = FALSE;
  }

  if(digitalRead(13) == LOW){
    key_cnt = 0;
  }

  before = digitalRead(13);   // スイッチ状態を保存

  /*-------------------
   * ブザー処理
   */  
  if(buzzer == ON){
    digitalWrite(0, HIGH);  // ブザーを鳴らす
    buz++;
  }

  if(buz > 100){  // １秒経過
    buzzer = OFF;
    buz = 0;
    digitalWrite(0, LOW);  // ブザーOFF
  }


}

/*-------------------------
 * セットアップ関数
 * 初期処理を行う
 -------------------------*/
	void setup()
	{
    MsTimer2::set(10, flash);     // 10ms毎にflash( )割込み関数を呼び出す様に設定
    MsTimer2::start();    // タイマー割り込み開始
  
    pinMode(sw, INPUT);   // スイッチ
    pinMode(bz, OUTPUT);  // ブザー
    
	  pinMode(d1, OUTPUT);
	  pinMode(d2, OUTPUT);
	  pinMode(d3, OUTPUT);
	  pinMode(d4, OUTPUT);
	  pinMode(a, OUTPUT);
	  pinMode(b, OUTPUT);
	  pinMode(c, OUTPUT);
	  pinMode(d, OUTPUT);
	  pinMode(e, OUTPUT);
	  pinMode(f, OUTPUT);
	  pinMode(g, OUTPUT);
	  pinMode(dp, OUTPUT);
	}
/////////////////////////////////////////////////////////////
/*-------------------------
 * メイン関数
 -------------------------*/
void loop()
{
  UB min = 0;
  UB sec = 0;
  UB sec1 = 0;
  UB sec2 = 0;

  // カウントダウン時間の計算
  min = time / 60;        // 分の計算
  sec = time - min * 60;  // 秒の計算
  sec2 = sec / 10;        // 秒 十の位
  sec1 = sec % 10;        // 秒 一の位

  // セグメントに表示処理
  Display(2, min);
  Display(3, sec2);
  Display(4, sec1);
  Display(2, 10); 

// ストップ中
  if(status == eSTOP){     
    cnt_ok = NG;  // カウントストップ
    
    if(mode == e3min){
      time = 180; // タイマーリセット 3分
    }else{
      time = 300; // 5分
    }
  
    if(key_nml == TRUE){  // スイッチ押下
      key_nml = FALSE;
      cnt_ok = OK;        // カウントダウンスタート
      status = eCOUNT;
      Click();  // クリック音
    }

    if(key_lng == TRUE){  // スイッチ長押し
      key_lng = FALSE;
      if(mode == e3min){  // タイマー3分/5分切り替え
        mode = e5min;
      }else{
        mode = e3min;
      }
      Click();  // クリック音
    }

 // カウントダウン中
  }else{                    
    if(key_lng == TRUE){  // 長押しでカウントダウンタイマーをリセット
      key_lng = FALSE;
      status = eSTOP;
      Click();  // クリック音
    }
    
    if(time == 0){    // タイムアップ
      buzzer = ON;    // ブザーを１秒間鳴らす
      status = eSTOP;
    }
    
  }



}
///////////////////////////////////////////////////////////////
/*
 * クリック音再生
 * スイッチ押下後、ブザーを一瞬鳴らす
 */
void Click(void){
  UH i;
  for(i=0;i<2000;i++){
    digitalWrite(0, HIGH);  // ブザーを鳴らす
  }
  digitalWrite(0, LOW);  // ブザーOFF
}

 
void WeiXuan(unsigned char n)// 表示桁 1が一番左
{
    switch(n)
     {
	case 1: 
	  digitalWrite(d1,HIGH);
 	  digitalWrite(d2, LOW);
	  digitalWrite(d3, LOW);
	  digitalWrite(d4, LOW);   
	 break;
	 case 2: 
	  digitalWrite(d1, LOW);
 	  digitalWrite(d2, HIGH);
	  digitalWrite(d3, LOW);
	  digitalWrite(d4, LOW); 
	    break;
	  case 3: 
	    digitalWrite(d1,LOW);
 	   digitalWrite(d2, LOW);
	   digitalWrite(d3, HIGH);
	   digitalWrite(d4, LOW); 
	    break;
	  case 4: 
	   digitalWrite(d1, LOW);
 	   digitalWrite(d2, LOW);
	   digitalWrite(d3, LOW);
	   digitalWrite(d4, HIGH); 
	    break;
        default :
     digitalWrite(d1, LOW);
	   digitalWrite(d2, LOW);
	   digitalWrite(d3, LOW);
	   digitalWrite(d4, LOW);
        break;
	  }
}
void Num_0()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
  digitalWrite(dp,HIGH);
}
void Num_1()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp,HIGH);
}
void Num_2()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  digitalWrite(dp,HIGH);
}
void Num_3()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
  digitalWrite(dp,HIGH);
}
void Num_4()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(dp,HIGH);
}
void Num_5()
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(dp,HIGH);
}
void Num_6()
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(dp,HIGH);
}
void Num_7()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp,HIGH);
}
void Num_8()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(dp,HIGH);
}
void Num_9()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(dp,HIGH);
}
void Dp()
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp,LOW);
}
void Clear()  // Clear the screen
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
  digitalWrite(dp,HIGH);
}
void pickNumber(unsigned char n)//Choose the number of
{
  switch(n)
  {
   case 0:Num_0();
   break;
   case 1:Num_1();
   break;
   case 2:Num_2();
   break;
   case 3:Num_3();
   break;
   case 4:Num_4();
   break;
   case 5:Num_5();
   break;
   case 6:Num_6();
   break;
   case 7:Num_7();
   break;
   case 8:Num_8();
   break;
   case 9:Num_9();
   break;
   case 10:Dp();
   break;
   default:Clear();
   break; 
  }
}
void Display(unsigned char x, unsigned char Number)//Show that x is the coordinate, Number is the number
{
  WeiXuan(x);
  pickNumber(Number);
 delay(1);
 Clear() ; //Vanishing
}
