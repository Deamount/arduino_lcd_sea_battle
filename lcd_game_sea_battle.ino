#include <UTFTGLUE.h>              //use GLUE class and constructor
UTFTGLUE myGLCD(0, A2, A1, A3, A4, A0);

#define MAX_SHIP 7
#define MAX_BULLET 20
#define WIDTH 480
#define HIGHT 320
#define R 15
#define BTN 10


struct ship_t {
  float x, y;
  int c;
  float dx;
};


struct bullet_t {
  float x, y;
  int c;
  float dy;
};

ship_t ship[MAX_SHIP];
bullet_t bul[MAX_BULLET];
int bcnt = 0;
float bul_speed = 0.0;
int last_btn = LOW;
int now_btn = LOW;

void initbullet(int index, float sp = 3.0 )
{
  if (sp < 2.0) sp = 2.0; 
  clear_bullet(index);
  bul[index].x = WIDTH / 2 ;
  bul[index].y = HIGHT;
  bul[index].c = 0xFF00FF;
  bul[index].dy = -sp;
}

void move_bullet(int index)
{
  bul[index].y = bul[index].y + bul[index].dy;
  //Serial.println(bul.y);
}

void draw_bullet(int index)
{
  myGLCD.setColor(bul[index].c);
  myGLCD.fillCircle(round(bul[index].x), round(bul[index].y), 2);
}

void clear_bullet(int index)
{
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillCircle(round(bul[index].x), round(bul[index].y), 2);
}

void initship(int index)
{
  //clear_ship(index);
  myGLCD.setColor(0, 0, 0);
  myGLCD.drawLine(round(ship[index].x - R), round(ship[index].y), round(ship[index].x + R), round(ship[index].y));
  myGLCD.drawLine(round(ship[index].x - R), round(ship[index].y - 1), round(ship[index].x + R), round(ship[index].y - 1));
  myGLCD.drawLine(round(ship[index].x - R), round(ship[index].y + 1), round(ship[index].x + R), round(ship[index].y + 1));
  myGLCD.drawLine(round(ship[index].x - R), round(ship[index].y - 2), round(ship[index].x + R), round(ship[index].y - 2));
  if (index==0)
  ship[index].c = 0xFFFFFF; else
  ship[index].c = random(0xFFFFFF) | 0xA0A0A0;
  ship[index].y = 20 + index * R;
  ship[index].x = -random(WIDTH);
  ship[index].dx = (30.0 + random(180)) / 100.0;
}

void move_ship(int index)
{
  ship[index].x = ship[index].x + ship[index].dx;

}

void draw_ship(int index)
{
  myGLCD.setColor(ship[index].c);
  if (index==0) myGLCD.drawCircle(round(ship[index].x), round(ship[index].y), 5);

  //myGLCD.fillCircle(round(ship[index].x), round(ship[index].y), R);
  //myGLCD.drawCircle(round(ship[index].x), round(ship[index].y), R);
  myGLCD.drawPixel(round(ship[index].x + R), round(ship[index].y));
  myGLCD.drawPixel(round(ship[index].x + R - 1), round(ship[index].y) - 1);
  myGLCD.drawPixel(round(ship[index].x + R - 1), round(ship[index].y) + 1);

  myGLCD.drawPixel(round(ship[index].x - 1), round(ship[index].y) - 2);
 

}

void clear_ship(int index)
{
  myGLCD.setColor(0, 0, 0);
  //myGLCD.fillCircle(round(ship[index].x), round(ship[index].y), R);
  //myGLCD.drawCircle(round(ship[index].x), round(ship[index].y), R);
  myGLCD.drawLine(round(ship[index].x - R), round(ship[index].y), round(ship[index].x - R - 3), round(ship[index].y));
  myGLCD.drawLine(round(ship[index].x - R + 1), round(ship[index].y - 1), round(ship[index].x - R - 3 - 1), round(ship[index].y - 1));
  myGLCD.drawLine(round(ship[index].x - R + 1), round(ship[index].y + 1), round(ship[index].x - R - 3 - 1), round(ship[index].y + 1));
  myGLCD.drawLine(round(ship[index].x - 3), round(ship[index].y - 2), round(ship[index].x - 3 - 2), round(ship[index].y - 2));
  if (index==0) myGLCD.drawCircle(round(ship[index].x), round(ship[index].y), 5);

}

void check()
{
  static int score;
  for (int j = 0; j < MAX_BULLET; j++)
  {
    for (int i = 0; i < MAX_SHIP; i++)
    {
      //    if((ship[i].x-R < bul[.x )&&(ship[i].x+R > bul.x )&&(ship[i].y-R < bul.y )&&(ship[i].y+R > bul.y ))
      if ((ship[i].x - R < bul[j].x ) && (ship[i].x + R > bul[j].x ) && (ship[i].y - R / 2 < bul[j].y ) && (ship[i].y + R / 2 > bul[j].y ))
      {
        initship(i);
        score++;
        Serial.println(score);
        myGLCD.setColor(255, 255, 255);
        myGLCD.print("Score: " + String(score), 400, 300);
      }
      
    }
   
  }

    if (bul[0].y < -20)
    { 
      if (bcnt >=2)
      {
        for(int k = 0;k < bcnt-1;k++)
        {
          bul[k].x = bul[k+1].x;
          bul[k].y = bul[k+1].y;
          bul[k].c = bul[k+1].c;
          bul[k].dy = bul[k+1].dy;
          Serial.print('*');
        }   
        Serial.println();
        bcnt--;
        if (bcnt<0) bcnt=0;
        //bul[bcnt].dy = 0;
      }
    }
  
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(BTN, INPUT);
  // Setup the LCD
  myGLCD.InitLCD();

  myGLCD.clrScr();
  myGLCD.setColor(random(255), random(255), random(255));
  //for (int i = 0; i < MAX_BULLET; i++) initbullet(i);
  for (int i = 0; i < MAX_SHIP; i++) initship(i);

  myGLCD.setColor(255, 255, 255);
  myGLCD.print("Score: 0", 400, 300);
  myGLCD.drawLine(235, 320, 235, 280);
  myGLCD.drawLine(245, 320, 245, 280);
}

void loop() {
  int i;
  
  

  //стираем
  for (i = 0; i < MAX_SHIP; i++) clear_ship(i);
  for (i = 0; i < MAX_BULLET; i++) clear_bullet(i);

  //двигаем
  for (i = 0; i < MAX_SHIP; i++)
  {
    move_ship(i);
    if (ship[i].x >= WIDTH * 2)
    {
      initship(i);
    }
  }
  for (i = 0; i < MAX_BULLET; i++) move_bullet(i);
  check();

  //рисуем
  for (i = 0; i < MAX_SHIP; i++)  draw_ship(i);
  for (i = 0; i < MAX_BULLET; i++) draw_bullet(i);

  now_btn = digitalRead(BTN);

  if (((now_btn == LOW) && (last_btn == HIGH)))
  {
    if (bcnt > 0)  
    {
      if (bul[bcnt-1].y < (HIGHT-20))
      { 
        if (bcnt < MAX_BULLET) 
        {
          initbullet(bcnt, bul_speed);
          bcnt++;
        }
      }
    } else 
    {
      initbullet(0, bul_speed);
      bcnt++;
   
    } 
  }
  
  if (now_btn == HIGH)
  {
    if (last_btn == LOW) bul_speed = 0;
    bul_speed = bul_speed + 0.1;
    if (bul_speed > 7.0) bul_speed = 7.0;
  }
  last_btn = now_btn;
  
  delay(10);
}
