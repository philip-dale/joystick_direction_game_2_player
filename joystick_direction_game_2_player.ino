#include <LiquidCrystal.h>

// Setup LCD
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Setup Joystick
const int xIn_1 = 0;
const int yIn_1 = 1;
const int pushBtn_1 = 2;

const int xIn_2 = 2;
const int yIn_2 = 3;
const int pushBtn_2 = 3;

const int lowLimit = 100;
const int highLimit = 924;

const int up = 0;
const int down = 1;
const int right = 2;
const int left = 3;
const int pushed = 4;
const int centre = 5;

const String directions[] = {"       Up       ", 
                             "      Down      ",
                             "     Right      ", 
                             "      Left      ",
                             "     Pushed     ",
                             "     Centre     "};

int noOfRounds = 5;
//**********************************************************
void setup() {
  pinMode(pushBtn_1, INPUT);
  pinMode(pushBtn_2, INPUT);
  digitalWrite(pushBtn_1, HIGH);
  digitalWrite(pushBtn_2, HIGH);
  lcd.begin(16, 2);
  lcd.print("    Welcome     ");
  delay(1000);
  randomSeed(analogRead(0));
}

//**********************************************************
int getPos(int xIn, int yIn, int pushBtn)
{
  const int xVal = analogRead(xIn);
  const int yVal = analogRead(yIn);
  const int pushVal = digitalRead(pushBtn);

  if(xVal > highLimit) return up;
  else if(xVal < lowLimit) return down;
  else if(yVal > highLimit) return right;
  else if(yVal < lowLimit) return left;
  else if(pushVal == 0) return pushed; 
  return centre;
}

//**********************************************************
void selectRounds(int &rounds)
{
  while(1)
  {
    lcd.clear();
    lcd.print("  Select Rounds ");
    lcd.setCursor(8, 1);
    lcd.print(rounds);
    int pos = getPos(xIn_1, yIn_1, pushBtn_1);
    if (pos == centre || pos == up || pos == down)
    {
      pos = getPos(xIn_2, yIn_2, pushBtn_2);
    }
    if(pos == pushed) break;
    else if(pos == right) rounds += 2;
    else if(pos == left)
    {
      if (rounds > 1) rounds -= 2;
    }
    delay(100);
  }
}

//**********************************************************
void roundStart(int currentRound)
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("    Round ");
  lcd.print(currentRound);
  lcd.setCursor(0, 0);
  lcd.print("   Start In ");
  for (int i=3; i>0; --i)
  {
    lcd.setCursor(12, 0);
    lcd.print(i);
    delay(1000);
  }
}

//**********************************************************
void endGame(const int p1Wins, const int p2Wins)
{
  lcd.clear();
  if (p1Wins > p2Wins)
  {
    lcd.print("     P1 Win     ");
  }
  else
  {
    lcd.print("     P2 Win     ");
  }
  lcd.setCursor(2, 1);
  lcd.print(p1Wins);
  lcd.setCursor(13, 1);
  lcd.print(p2Wins);

  while(digitalRead(pushBtn_1) == 1 && digitalRead(pushBtn_2) == 1)
  {
    delay(100);
  }
  delay(500);
}

//**********************************************************
void loop() {
  int currentRound = 1;
  int p1Wins = 0;
  int p2Wins = 0;

  selectRounds(noOfRounds);
  
  while(currentRound <= noOfRounds)
  {
    roundStart(currentRound);
    
    const long randNumber = random(0,centre); // So Centre is not an option
    int p1Move = centre;
    int p2Move = centre;
    unsigned long p1Time = 0;
    unsigned long p2Time = 0;
    const unsigned long startTime = millis();
    
    lcd.setCursor(0, 0);
    lcd.print(directions[randNumber]);

    // Wait for moves
    while(p1Move == centre || p2Move == centre)
    {
      if(p1Move != randNumber)
      {
        int currentPos = getPos(xIn_1, yIn_1, pushBtn_1);
        if(currentPos == randNumber)
        {
          p1Move = currentPos;
          p1Time = millis();
          lcd.setCursor(0, 1);
          lcd.print("OK");
        }
      }

      if(p2Move != randNumber)
      {
        int currentPos = getPos(xIn_2, yIn_2, pushBtn_2);
        if(currentPos == randNumber)
        {
          p2Move = currentPos;
          p2Time = millis();
          lcd.setCursor(14, 1);
          lcd.print("OK");
        }
      }
    }

    // Display Round Times
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(p1Time-startTime);
    lcd.setCursor(12, 1);
    lcd.print(p2Time-startTime);
    delay(1000);
    
    // Display Round Winner
    if(p1Time < p2Time)
    {
      p1Wins++;
      lcd.setCursor(0, 1);
      lcd.print("WIN ");
      lcd.setCursor(12, 1);
      lcd.print("lOSE");
    }
    else
    {
      p2Wins++;
      lcd.setCursor(0, 1);
      lcd.print("LOSE");
      lcd.setCursor(12, 1);
      lcd.print("WIN ");
    }
    currentRound++;
    delay(1000);
  }
  
  endGame(p1Wins, p2Wins);
}
