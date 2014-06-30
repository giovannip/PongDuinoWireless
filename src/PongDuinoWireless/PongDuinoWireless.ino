#include <TVout.h>
#include <fontALL.h>

/* --------------------------------------------------------------------------- */

// Consts
const byte ArrayPosX = 0;
const byte ArrayPosY = 1;
const byte Tdelay = 30;

const byte MovimentFactor[] = { 2,   1 };
const byte ScreenSize[]     = { 120, 90 };
const byte TableSize[]      = { 120, 81 };
const byte RacketSize[]     = { 2,   25 };
const byte BallSize = 4;

//const int PINSound = 2;
//const int PINLEDPause = 13;
//const int PINLEDInGame = 12;
//const int PINLEDPower = 10;

/* --------------------------------------------------------------------------- */

// Facilitator Components
typedef struct
{
    byte X, Y;
    byte SizeOfX, SizeOfY;
} XY;
enum EBallMovimentType{ UP_RIGHT, DOWN_RIGHT, DOWN_LEFT, UP_LEFT };

/* --------------------------------------------------------------------------- */

// Variables
volatile bool Pause = 0;
byte BallMovimentType;
byte RoundWinner;
XY Ball;

// ignore 0
byte PlayerLifes[3]; 
XY CurrentRackets[3];
XY ReadRackets[3];


TVout TV;

/* --------------------------------------------------------------------------- */

void setup()
{
    TV.begin(NTSC, ScreenSize[ArrayPosX], ScreenSize[ArrayPosY]);
    //pinMode(SoundPIN, OUTPUT);	
    //pinMode(PauseLEDPIN, OUTPUT);
    //pinMode(InGameLEDPIN, OUTPUT);
    //pinMode(PowerLEDPIN, OUTPUT);

    //digitalWrite(PowerLEDPIN, HIGH);

    Ball.SizeOfX     = Ball.SizeOfY     = BallSize;
    P1Racket.SizeOfX = P2Racket.SizeOfX = RacketSize[ArrayPosX];
    P1Racket.SizeOfY = P2Racket.SizeOfY = RacketSize[ArrayPosY];

    TV.select_font(font8x8);

    TV.clear_screen();
    TV.println((TableSize[ArrayPosX] / 2) - 36 ,(TableSize[ArrayPosY] / 2) - 5,"PongDuino");
    TV.delay(2500);

    TV.println((TableSize[ArrayPosX] / 2) - 36 ,(TableSize[ArrayPosY] / 2) + 5,"V 3.0.2.4");
    TV.delay(2500);

    BeginGame();
}

void loop()
{
    ReadRacketsValues();
    MoveRackets();
    SetCurrentRacketValue();

    MoveBall();

    CheckWinner();
    CheckPause();
}

/* --------------------------------------------------------------------------- */

void MakeObj(XY Obj , int Erase = 0)
{
    for(int i = Obj.X ; i < (Obj.X + Obj.SizeOfX); i++)
    {
        for(int j = Obj.Y ; j < (Obj.Y + Obj.SizeOfY); j++)
        {
            char color =(Erase)? BLACK : WHITE;
            TV.set_pixel(i,j, color);
        }
    }
}

void SetPause()
{
    Pause = !Pause;
}

void CheckPause()
{
    bool InternalPause = Pause;
    if(InternalPause)
    {		
        //digitalWrite(PauseLEDPIN , HIGH);
        //digitalWrite(InGameLEDPIN, LOW);	
        TV.select_font(font8x8);
        while(InternalPause)
        {
            TV.clear_screen();
            TV.delay(500);
            TV.print((TableSize[ArrayPosX] / 2) - 25 ,(TableSize[ArrayPosY] / 2) - 5,"PAUSE");
            TV.delay(500);
            InternalPause = Pause;
        }
        TV.clear_screen();	
        DrawHeader();
        //digitalWrite(PauseLEDPIN , LOW);
        //digitalWrite(InGameLEDPIN, HIGH);
    }
}

void Beep(int pSoundType = 0)
{
    /*digitalWrite(SoundPIN, HIGH);*/
    TV.delay((pSoundType%2));
    /*digitalWrite(SoundPIN, LOW);*/
}

void SetBallFirstPosition()
{
    Ball.X =((TableSize[ArrayPosX] / 2) -(BallSize / 2));
    Ball.Y =((TableSize[ArrayPosY] / 2) -(BallSize / 2));

    MakeObj(Ball);
}

void MoveBall()
{
    int i ,j;
    XY xBall;

    RoundWinner = 0;
    xBall.X       = Ball.X;
    xBall.Y       = Ball.Y;
    xBall.SizeOfX = Ball.SizeOfX;
    xBall.SizeOfY = Ball.SizeOfY;	

    switch(BallMovimentType)
    {
    case UP_RIGHT:
        xBall.Y -= MovimentFactor[ArrayPosY]; // Up
        xBall.X += MovimentFactor[ArrayPosX]; // Right

        if((xBall.Y) <= 0) 
        {
            BallMovimentType = DOWN_RIGHT; 
            Beep(1);
        }
        else if((xBall.X + xBall.SizeOfX) >= CurrentRackets[2].X)
        {
            if((((xBall.Y + xBall.SizeOfY) >= CurrentRackets[2].Y)) &&((xBall.Y) <=(CurrentRackets[2].Y + CurrentRackets[2].SizeOfY)))
            {
                BallMovimentType = UP_LEFT; 
                Beep(); 
            }
            else
            {
                RoundWinner = 1;
                exit;
            }
        }
        break;

    case DOWN_RIGHT:
        xBall.Y += MovimentFactor[ArrayPosY]; // Down
        xBall.X += MovimentFactor[ArrayPosX]; // Right

        if((xBall.Y + xBall.SizeOfY) >= TableSize[ArrayPosY])
        {
            BallMovimentType = UP_RIGHT;	
            Beep(1);
        }
        else if((xBall.X + xBall.SizeOfX) >= CurrentRackets[2].X)
        {
            if((((xBall.Y + xBall.SizeOfY) >= CurrentRackets[2].Y)) &&((xBall.Y) <=(CurrentRackets[2].Y + CurrentRackets[2].SizeOfY)))
            {
                BallMovimentType = DOWN_LEFT; 
                Beep(); 
            }
            else
            {
                RoundWinner = 1;
                exit;
            }
        }	
        break;

    case DOWN_LEFT:
        xBall.Y += MovimentFactor[ArrayPosY]; // Down
        xBall.X -= MovimentFactor[ArrayPosX]; // Left


        if((xBall.Y + xBall.SizeOfY) >= TableSize[ArrayPosY])
        {
            BallMovimentType = UP_LEFT; 
            Beep(1);
        }
        else if(xBall.X <=(CurrentRackets[1].X + CurrentRackets[1].SizeOfX))
        {
            if((((xBall.Y + xBall.SizeOfY) >= CurrentRackets[1].Y)) &&((xBall.Y) <=(CurrentRackets[1].Y + CurrentRackets[1].SizeOfY)))
            {
                BallMovimentType = DOWN_RIGHT; 
                Beep();
            }
            else
            {
                RoundWinner = 2;
                exit;
            }				
        }			
        break;

    case UP_LEFT:
        xBall.Y -= MovimentFactor[ArrayPosY]; // Up
        xBall.X -= MovimentFactor[ArrayPosX]; // Left


        if((xBall.Y) <= 0) 
        {
            BallMovimentType = DOWN_LEFT;  
            Beep(1);
        }
        else if(xBall.X <=(CurrentRackets[1].X + CurrentRackets[1].SizeOfX))
        {
            if((((xBall.Y + xBall.SizeOfY) >= CurrentRackets[1].Y)) &&((xBall.Y) <=(CurrentRackets[1].Y + CurrentRackets[1].SizeOfY)))
            {
                BallMovimentType = UP_RIGHT; 
                Beep();
            }
            else
            {
                RoundWinner = 2;
                exit;
            }	
        }	
        break;
    }

    MakeObj(Ball , 1);

    Ball.X 	     = xBall.X;
    Ball.Y 	     = xBall.Y;
    Ball.SizeOfX = xBall.SizeOfX;
    Ball.SizeOfY = xBall.SizeOfY;

    MakeObj(Ball);

    TV.delay(Tdelay);

}

void SetRacketsFirstPosition()
{
    P1Racket.X = 0;
    P1Racket.Y =((TableSize[ArrayPosY] / 2) -(RacketSize[ArrayPosY] / 2));
    MakeObj(P1Racket);

    P2Racket.X = TableSize[ArrayPosX] - RacketSize[ArrayPosX];
    P2Racket.Y =((TableSize[ArrayPosY] / 2) -(RacketSize[ArrayPosY] / 2));
    MakeObj(P2Racket);
}

byte AdjustRacketValue(int ReadValue)
{          
    return map(ReadValue, 0, 1023, 0,(TableSize[ArrayPosY] - RacketSize[ArrayPosY]));  
}

void ReadRacketsValues()
{
    ReadRackets[1].Y = AdjustRacketValue(123);//??
    ReadRackets[2].Y = AdjustRacketValue(123);//??
}

void SetCurrentRacketValue()
{
    CurrentRackets[1].Y = ReadRackets[1].Y;
    CurrentRackets[2].Y = ReadRackets[2].Y;
}

void MoveRackets()
{
    MakeObj(CurrentRackets[1], 1);
    MakeObj(ReadRackets[1]);

    MakeObj(CurrentRackets[2], 1);
    MakeObj(ReadRackets[2]);
}

void DrawWinner(int pWinner)
{
    /*digitalWrite(InGameLEDPIN, LOW);*/
    TV.select_font(font8x8);
    for(int i = 0 ; i < 5 ; i++)
    {
        TV.clear_screen();
        TV.delay(500);
        TV.println((TableSize[ArrayPosX] / 2) - 30 ,(TableSize[ArrayPosY] / 2) - 15,"Winner");
        TV.print((TableSize[ArrayPosX] / 2) - 35 ,(TableSize[ArrayPosY] / 2) - 5,"Player ");
        TV.print(pWinner);
        TV.delay(500);
    }	
    BeginGame();
}

void CheckWinner()
{
    if(RoundWinner)
    {	
        MakeObj(Ball , 1);
        switch(RoundWinner)
        {
        case 1:
            PlayerLifes[2]--;
            break;
        case 2:
            PlayerLifes[1]--;
            break;
        }
        TV.delay(500);

        if(PlayerLifes[1] == 0)
            DrawWinner(2);
        else if(PlayerLifes[2] == 0)
            DrawWinner(1);
        else		
            Start();
    }	
}

void Start()
{
    TV.clear_screen();

    DrawHeader();

    SetRacketsFirstPosition();
    SetBallFirstPosition();

    TV.delay(1000);

    BallMovimentType =(BallMovimentType + 1)%4;
}

void BeginGame()
{
    PlayerLifes[1] = PlayerLifes[2] = 5;

    BallMovimentType = -1;

    DrawWelcome();	
    Start();
    /*digitalWrite(InGameLEDPIN, HIGH);*/
}

void DrawHeader()
{
    for(int i = 0; i <= ScreenSize[ArrayPosX]; i++)
        TV.set_pixel(i,(TableSize[ArrayPosY] + 1), WHITE);

    TV.select_font(font4x6);

    TV.print(2 ,(TableSize[ArrayPosY] + ((ScreenSize[ArrayPosY] -(TableSize[ArrayPosY])) / 2)) ,"P1 = ");
    for(int i = 1; i <= PlayerLifes[1]; i++)
        TV.print("x");

    TV.print(((ScreenSize[ArrayPosX] / 2) + 2) ,(TableSize[ArrayPosY] + ((ScreenSize[ArrayPosY] -(TableSize[ArrayPosY])) / 2)) ,"P2 = ");
    for(int i = 1; i <= PlayerLifes[2]; i++)
        TV.print("x");
}

void DrawWelcome()
{
    TV.select_font(font8x8);

    TV.clear_screen();
    TV.println((TableSize[ArrayPosX] / 2) - 12 ,(TableSize[ArrayPosY] / 2) - 5,"Are");
    TV.delay(750);

    TV.clear_screen();
    TV.println((TableSize[ArrayPosX] / 2) - 12 ,(TableSize[ArrayPosY] / 2) - 5,"You");
    TV.delay(750);

    TV.clear_screen();
    TV.println((TableSize[ArrayPosX] / 2) - 32 ,(TableSize[ArrayPosY] / 2) - 5,"Ready???");
    TV.delay(2000);

    TV.clear_screen();
    TV.println((TableSize[ArrayPosX] / 2) - 8 ,(TableSize[ArrayPosY] / 2) - 5,"3");
    TV.delay(1000);

    TV.clear_screen();
    TV.println((TableSize[ArrayPosX] / 2) - 8 ,(TableSize[ArrayPosY] / 2) - 5,"2");
    TV.delay(1000);

    TV.clear_screen();
    TV.println((TableSize[ArrayPosX] / 2) - 8 ,(TableSize[ArrayPosY] / 2) - 5,"1");
    TV.delay(1000);

    for(int i = 0; i<5; i++)
    {
        TV.clear_screen();
        TV.delay(250);
        TV.println((TableSize[ArrayPosX] / 2) - 28 ,(TableSize[ArrayPosY] / 2) - 5,"Pong!!!");
        TV.delay(250);
    }
}
