#include <Gamebuino.h>
#include <SPI.h>
#include <LinkedList.h>

enum direction {
  UP, DOWN, LEFT, RIGHT
};

class Coordinate {

  public:
    int x, y;
    void setX(int a) {
      x = a;
    };
    void setY(int b) {
      y = b;
    };
    Coordinate() {
      x = 0;
      y = 0;
    };
    Coordinate(int a, int b) {
      x = a;
      y = b;

    }
    bool equalsOther(Coordinate c) {
      return c.x == x && c.y == y;

    }
    void changeX(int a) {
      x = a;
    }
    void changeY(int b) {
      y = b;

    }

};
LinkedList<Coordinate> projectiles = LinkedList<Coordinate>();
LinkedList<direction> projectilesDir = LinkedList<direction>();

LinkedList<Coordinate> Enemies = LinkedList<Coordinate>();

bool won = false;
bool gameEnded = false;

LinkedList<Coordinate> enemyProjectiles = LinkedList<Coordinate>();

int waitShoot = 0;
int waitMove = 0;
int waitMoveCentral = 0;

direction youDir = UP;
direction enemyDir = RIGHT;

Gamebuino gb;
Coordinate currYou =  Coordinate(LCDWIDTH / 2, LCDHEIGHT * 9 / 10);
Coordinate currTarget =  Coordinate(LCDWIDTH / 2, LCDHEIGHT * 1 / 10);


void setup() {
  // put your setup code here, to run once:
  gb.begin();
  gb.titleScreen(F("Shoot"));
  gb.pickRandomSeed();
  initEnemies();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gb.update()) {
    if (!gameEnded) {
      moveEnemyProjectiles();
      moveProjectiles();
      moveGuy();
      moveEnemies();
      //shootEnemy();
      shootEnemies();
      shoot();
      checkKills();
      checkGameOver();
      drawPixels();
    }
    else if (won) {
      gb.popup(F("YOU WON"), 20);

    }
    else {
      gb.popup(F("YOU LOST"), 20);

    }
  }
}


void initEnemies() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 5; j++) {
      Enemies.add(Coordinate(LCDWIDTH * (3 + j  )  / 10, LCDHEIGHT * (i + 1) / 10));


    }
  }
}
void shootEnemy() {
  if (waitShoot > 20) {
    enemyProjectiles.add(Coordinate(currTarget.x, currTarget.y + 1));
    waitShoot = 0;
  }
  else {
    waitShoot++;
  }

}

void shootEnemy(Coordinate co ) {
  if (random(0, 100) > 50) {
    enemyProjectiles.add(Coordinate(co.x, co.y + 1));
  }

}

void shootEnemies() {
  if (waitShoot > 10) {
    for (int i = 0; i < Enemies.size(); i++) {
      shootEnemy(Enemies.get(i));



    }
    waitShoot = 0;
  }

  else {
    waitShoot++;
  }


}








void moveEnemy() {
  // if (waitMoveCentral > 2) {
  if (enemyDir == RIGHT) {
    if (currTarget.x >= LCDWIDTH * 7 / 10) {
      enemyDir = LEFT;
      for (int i = 0; i < Enemies.size(); i++) {
        Enemies.set(i, Coordinate(Enemies.get(i).x, Enemies.get(i).y + 1));


      }
      currTarget.changeY(currTarget.y + 1);


    }
    else {

      currTarget.changeX(currTarget.x + 1);
    }
  }

  else {
    if (currTarget.x <= LCDWIDTH * 3 / 10) {
      enemyDir = RIGHT;

      for (int i = 0; i < Enemies.size(); i++) {
        Enemies.set(i, Coordinate(Enemies.get(i).x, Enemies.get(i).y + 1));


      }
      currTarget.changeY(currTarget.y + 1);
    }

    else {

      currTarget.changeX(currTarget.x - 1);
    }

  }
  waitMoveCentral = 0;
  /*}
    else {
    waitMoveCentral++;
    }
  */
}

void moveEnemy(Coordinate co, int index) {

  if (enemyDir == RIGHT) {
    // if (currTarget.x >= LCDWIDTH * 7 / 10) {
    //enemyDir = LEFT;
    co.changeX(co.x + 1);
    Enemies.set(index, co);

    //}
    /* else {
       co.changeX(co.x + 1);
       Enemies.set(index, co);
      }*/
  }

  else {
    //if (currTarget.x <= LCDWIDTH * 3 / 10) {
    //enemyDir = RIGHT;
    co.changeX(co.x - 1);
    Enemies.set(index, co);
    // }
    /* else {
       co.changeX(co.x - 1);
       Enemies.set(index, co);
      }*/
  }

}





void moveEnemies() {
  if (waitMove > 2) {
    moveEnemy();
    for (int i = 0; i < Enemies.size(); i++) {

      moveEnemy(Enemies.get(i), i);


    }
    waitMove = 0;

  }
  waitMove++;

}








void moveEnemyProjectiles() {
  int i = 0;
  int deletednb = 0;
  while (i < enemyProjectiles.size() - deletednb) {

    direction myDir = projectilesDir.get(i);

    enemyProjectiles.set(i,   Coordinate( enemyProjectiles.get(i).x, enemyProjectiles.get(i).y + 1));

    if (enemyProjectiles.get(i).y < 0 || enemyProjectiles.get(i).y > LCDHEIGHT) {
      deletednb++;
      enemyProjectiles.remove(i);
      i--;
    }
    i++;
  }



}

void moveProjectiles() {
  int i = 0;
  int deletednb = 0;
  while (i < projectiles.size() - deletednb) {

    direction myDir = projectilesDir.get(i);
    switch (myDir) {
      case UP:
        projectiles.set(i, Coordinate( projectiles.get(i).x, projectiles.get(i).y - 1));

        break;

      case DOWN:
        projectiles.set(i,   Coordinate( projectiles.get(i).x, projectiles.get(i).y + 1));
        break;

      case LEFT:
        projectiles.set(i,   Coordinate( projectiles.get(i).x - 1, projectiles.get(i).y ));
        break;

      case RIGHT:
        projectiles.set(i,   Coordinate( projectiles.get(i).x + 1, projectiles.get(i).y ));
        break;





    }
    if (projectiles.get(i).y < 0 || projectiles.get(i).y > LCDHEIGHT) {
      deletednb++;
      projectiles.remove(i);
      i--;
    }
    i++;
  }

}

void gameOver() {



}

void shoot() {
  if (gb.buttons.pressed(BTN_A)) {
    projectiles.add(Coordinate(currYou.x, currYou.y - 1));

    /*
      switch (youDir) {

      case UP:
        projectiles.add(Coordinate(currYou.x, currYou.y - 1));
        projectilesDir.add(UP);
        break;

      case DOWN:
        projectiles.add(Coordinate(currYou.x, currYou.y + 1));
        projectilesDir.add(DOWN);
        break;

      case LEFT:
        projectiles.add(Coordinate(currYou.x - 1, currYou.y));
        projectilesDir.add(LEFT);
        break;

      case RIGHT:
        projectiles.add(Coordinate(currYou.x + 1, currYou.y));
        projectilesDir.add(RIGHT);
        break;



      }*/
  }
}


void checkKills() {
  int nbDeleted = 0;
  for (int i = 0; i < projectiles.size(); i++) {
    int j = 0;
    while ( j < Enemies.size() - nbDeleted) {
      if (projectiles.get(i).x ==  Enemies.get(j).x && projectiles.get(i).y == Enemies.get(j).y) {

        nbDeleted++;
        Enemies.remove(j);
        j--;
      }
      j++;
    }
  }

}


void moveGuy() {

  /*
    if (gb.buttons.pressed(BTN_UP) ) {
    currYou.changeY(currYou.y - 1);
    youDir = UP;


    }
    if (gb.buttons.pressed(BTN_DOWN) ) {
    currYou.changeY(currYou.y + 1);
    youDir = DOWN;
    }*/
  if (gb.buttons.pressed(BTN_LEFT) ) {
    youDir = LEFT;
    currYou.changeX(currYou.x - 1);
  }
  if (gb.buttons.pressed(BTN_RIGHT) ) {
    currYou.changeX(currYou.x + 1);
    youDir = RIGHT;
  }
}

void drawPixels() {
  gb.display.drawPixel(currYou.x, currYou.y);
  //gb.display.drawPixel(currTarget.x, currTarget.y);
  for (int i = 0; i < projectiles.size(); i++) {
    gb.display.drawPixel(projectiles.get(i).x, projectiles.get(i).y);


  }
  for (int i = 0; i < enemyProjectiles.size(); i++) {
    gb.display.drawPixel(enemyProjectiles.get(i).x, enemyProjectiles.get(i).y);


  }

  for (int i = 0; i < Enemies.size(); i++) {
    gb.display.drawPixel(Enemies.get(i).x, Enemies.get(i).y);


  }
}

void checkGameOver() {
  if (Enemies.size() == 0) {
    won = true;
    gameEnded = true;
  }
  else {
    for (int i = 0; i < enemyProjectiles.size(); i++) {

      if (enemyProjectiles.get(i).x ==  currYou.x && enemyProjectiles.get(i).y == currYou.y) {
        gameEnded = true;
      }

    }
    for (int i = 0; i < Enemies.size(); i++) {
      if(Enemies.get(i).y==currYou.y){
        gameEnded=true;
        
        }


    }

  }

}



