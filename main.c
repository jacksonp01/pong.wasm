#define OLIVEC_IMPLEMENTATION
#include "./libraries/olive.c"
#define WIDTH 800
#define HEIGHT 600
#define BLACK 0xFF000000
#define WHITE 0xFFFFFFFF
#define PADEL_SPEED 10
#define PADEL_HEIGHT 100
#define PADEL_WIDTH 25
#define BALL_SIZE 25
#define PADEL_COLOR WHITE
#define BALL_COLOR WHITE
#define BALL_STARTING_XSPEED 10
#define BALL_STARTING_YSPEED 1

typedef struct {
  int x, y;
} Padel;
typedef struct {
  int x, y, speedx, speedy;
} Ball;

uint32_t size = WIDTH * HEIGHT;
uint32_t pixels[WIDTH * HEIGHT];
Olivec_Canvas canvas = {pixels, WIDTH, HEIGHT, WIDTH};

int previousx = BALL_STARTING_XSPEED;
int previousy = BALL_STARTING_YSPEED;

Padel padel1 = {50, HEIGHT / 2 - 100 / 2};
Padel padel2 = {WIDTH - 50, HEIGHT / 2 - 100 / 2};
Ball ball = {WIDTH / 2 - 25 / 2, HEIGHT / 2 - 25 / 2, BALL_STARTING_XSPEED,
             BALL_STARTING_YSPEED};

int score1 = 0;
int score2 = 0;

int getScore1() { return score1; }
int getScore2() { return score2; }

void moveRightUp() {
  padel2.y -= PADEL_SPEED;
  if (padel2.y < 0) {
    padel2.y = 0;
  }
}
void moveRightDown() {
  padel2.y += PADEL_SPEED;
  if (padel2.y + PADEL_HEIGHT > canvas.height) {
    padel2.y = canvas.height - PADEL_HEIGHT;
  }
}
void moveLeftUp() {
  padel1.y -= PADEL_SPEED;
  if (padel1.y < 0) {
    padel1.y = 0;
  }
}
void moveLeftDown() {
  padel1.y += PADEL_SPEED;
  if (padel1.y + PADEL_HEIGHT > canvas.height) {
    padel1.y = canvas.height - PADEL_HEIGHT;
  }
}
void reset() {
  ball.x = canvas.width / 2 - BALL_SIZE / 2;
  ball.y = canvas.height / 2 - BALL_SIZE / 2;
  padel1.y = canvas.height / 2 - PADEL_HEIGHT / 2;
  padel2.y = canvas.height / 2 - PADEL_HEIGHT / 2;
  ball.speedx = previousx;
  ball.speedy = previousy;
  previousx *= -1;
  previousy *= -1;
}
void resetScore() {
  score1 = 0;
  score2 = 0;
  reset();
}

int getWidth() { return WIDTH; }
int getHeight() { return HEIGHT; }

uint32_t *getPixels() { return pixels; }

void rectangle(uint32_t color, size_t x1, size_t y1, size_t x2, size_t y2) {
  olivec_rect(canvas, x1, y1, x2, y2, color);
}

void fill(uint32_t color) { olivec_fill(canvas, color); }

int abs(int number) { return number > 0 ? number : -number; }

void calculateBall() {
  ball.x += ball.speedx;
  ball.y += ball.speedy;

  // Check if ball bounces off the ceiling or floor
  if (ball.y < 0) {
    ball.y = ball.y;
    ball.speedy *= -1;
  } else if (ball.y + BALL_SIZE > canvas.height) {
    ball.y = canvas.height - BALL_SIZE;
    ball.speedy *= -1;
  }

  // Check if ball scores on either side or hits a padel
  if (ball.x < 0) {
    score2 += 1;
    reset();
  } else if (ball.x + BALL_SIZE > canvas.width) {
    score1 += 1;
    reset();
  } else if (ball.y + BALL_SIZE > padel1.y &&
             ball.y < padel1.y + PADEL_HEIGHT && ball.x > padel1.x &&
             ball.x < padel1.x + PADEL_WIDTH) {
    ball.x = padel1.x + PADEL_WIDTH;
    ball.speedx *= -1;
    ball.speedy =
        (ball.speedy > 0 ? 1 : -1) *
        abs(canvas.height / 2 - PADEL_HEIGHT / 2 - BALL_SIZE / 2 + ball.y) /
        PADEL_HEIGHT;
  } else if (ball.y + BALL_SIZE > padel2.y &&
             ball.y < padel2.y + PADEL_HEIGHT &&
             ball.x + BALL_SIZE < padel2.x + PADEL_WIDTH &&
             ball.x + BALL_SIZE > padel2.x) {
    ball.x = padel2.x - BALL_SIZE;
    ball.speedx *= -1;
    ball.speedy =
        (ball.speedy > 0 ? 1 : -1) *
        abs(canvas.height / 2 - PADEL_HEIGHT / 2 - BALL_SIZE / 2 + ball.y) /
        PADEL_HEIGHT / 2;
  }
}

void render() {
  olivec_fill(canvas, BLACK);
  olivec_rect(canvas, padel1.x, padel1.y, PADEL_WIDTH, PADEL_HEIGHT,
              PADEL_COLOR);
  olivec_rect(canvas, padel2.x, padel2.y, PADEL_WIDTH, PADEL_HEIGHT,
              PADEL_COLOR);
  olivec_rect(canvas, ball.x, ball.y, BALL_SIZE, BALL_SIZE, BALL_COLOR);
}

void step() {
  calculateBall();
  render();
}
