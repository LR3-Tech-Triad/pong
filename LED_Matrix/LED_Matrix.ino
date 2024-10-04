int IIC_SCL[2] = {A2, A4};
int IIC_SDA[2] = {A1, A3};

int up[2] = {2,4};
int down[2] = {3,5};

const int PADDLE_WIDTH = 3;
int playerY[2] = { 3, 3 };
int score[2] = { 0, 0 };

struct ball_struct {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
};
struct ball_struct ball = { 8, 8, 0.25, 0.5 };

void IIC_start(int screenNumber);
void IIC_send(unsigned char send_data, int screenNumber);
void IIC_end(int screenNumber);

int world[16][16] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

void columnToInt(int column[16], unsigned int values[2]) {

  for (int exp = 0; exp < 8; exp++) {

    unsigned int value = column[7 - exp];
    unsigned int value2 = column[15 - exp];
    values[0] += value * ceil(pow(2, exp));
    values[1] += value2 * ceil(pow(2, exp));
  }

}

void drawPaddles() {

  for (int player = 0; player < 2; player++) {
    for (int i = playerY[player]; i < playerY[player] + PADDLE_WIDTH; i++) {
      world[15 * player][i] = 1;
    }
  }
}

void drawBall(){
  ball.x += ball.xSpeed;
  if (ball.x > 15){
    ball.x=8;
    ball.y=8;
    score[0]++;
    Serial.print("Player 1:");
    Serial.println(score[0]);
    return;
  }
  else if(ball.x<0){
    ball.x=8;
    ball.y=8;
    score[1]++;
    Serial.print("Player 2:");
    Serial.println(score[1]);
    return;
  }
  ball.y += ball.ySpeed;
  if (ball.y > 15){
    ball.ySpeed*=-1;
    ball.y-=2;
  }
  else if (ball.y < 0){
    ball.ySpeed*=-1;
    ball.y += 2;
  }
  int x = floor(ball.x);
  int y = floor(ball.y);
  if (!world[x][y] == 1){
    world[x][y] = 1;
  }
  else {
    ball.xSpeed*=-1;
    ball.ySpeed*=-1;
    delay(250);
  }
  
}

void cleanMap() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      world[i][j] = 0;
    }
  }
}

void handleInput(int player) {

  int movement = 0;
  if (digitalRead(up[player])){
    movement++;
  }
  if (digitalRead(down[player])){
    movement--;
  }
  
  bool canMove = playerY[player] + movement >= 0 && playerY[player] + movement <= 16-PADDLE_WIDTH;
  playerY[player] += canMove ? movement : 0;

  #ifdef DEBUG
  if (movement != 0){
    Serial.print("Player");
    Serial.print(player);
    Serial.print(" movement: ");
    Serial.println(movement);
    Serial.print("Player pos: ");
    Serial.println(playerY[player]);
    Serial.println("---");
  }
  #endif

}

void setup() {
  Serial.begin(9600);
  pinMode(IIC_SCL[0], OUTPUT);
  pinMode(IIC_SCL[1], OUTPUT);
  pinMode(IIC_SDA[0], OUTPUT);
  pinMode(IIC_SDA[1], OUTPUT);
  pinMode(up[0], INPUT);
  pinMode(up[1], INPUT);
  pinMode(down[0], INPUT);
  pinMode(down[1], INPUT);
  digitalWrite(IIC_SCL[0], LOW);
  digitalWrite(IIC_SCL[1], LOW);
  digitalWrite(IIC_SDA[0], LOW);
  digitalWrite(IIC_SDA[1], LOW);
}

/*----------------------------------------------------------------*/
void loop() {
  /**************set the address plus 1***************/
  IIC_start(0);
  IIC_start(1);
  IIC_send(0x40, 0);
  IIC_send(0x40, 1);  // set the address plus 1 automatically
  IIC_end(0);
  IIC_end(1);
  /************end the process of address plus 1 *****************/
  /************set the data display*****************/
  IIC_start(0);
  IIC_start(1);
  IIC_send(0xc0, 0);  // set the initial address as 0 c0
  IIC_send(0xc0, 1);

  cleanMap();
  handleInput(0);
  handleInput(1);
  drawPaddles();
  drawBall();


  for (int i = 0; i < 16; i++) {

    unsigned int values[2] = {0,0};

    columnToInt(world[i], values);

    IIC_send(values[0], 0);  // send the display data
    IIC_send(values[1], 1);
  }

  IIC_end(0);
  IIC_end(1);
  /************end the data display*****************/
  /*************set the brightness display***************/
  IIC_start(0);
  IIC_start(1);
  IIC_send(0x8A, 0);  // set the brightness display
  IIC_send(0x8A, 1);
  IIC_end(0);
  IIC_end(1);

  /*************end the brightness display***************/
}
/*----------------------------------------------------------------*/
void IIC_start(int screenNumber) {
  digitalWrite(IIC_SCL[screenNumber], LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA[screenNumber], HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL[screenNumber], HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA[screenNumber], LOW);
  delayMicroseconds(3);
}
void IIC_send(unsigned char send_data, int screenNumber) {
  for (char i = 0; i < 8; i++) {
    digitalWrite(IIC_SCL[screenNumber], LOW);
    delayMicroseconds(3);
    if (send_data & 0x01) {
      digitalWrite(IIC_SDA[screenNumber], HIGH);
    } else {
      digitalWrite(IIC_SDA[screenNumber], LOW);
    }
    delayMicroseconds(3);
    digitalWrite(IIC_SCL[screenNumber], HIGH);
    delayMicroseconds(3);
    send_data = send_data >> 1;
  }
}
void IIC_end(int screenNumber) {
  digitalWrite(IIC_SCL[screenNumber], LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA[screenNumber], LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL[screenNumber], HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA[screenNumber], HIGH);
  delayMicroseconds(3);
}
