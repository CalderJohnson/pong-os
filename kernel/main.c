/* entry point for the game */

#include "../drivers/vga_driver.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"
#include "../x86/isr.h"
#include "util.h"
#include "main.h"

//declarations of globals
volatile enum BallDirection ballDirection;
volatile uint16_t playerPaddleX;
volatile uint16_t playerPaddleY;
volatile uint16_t enemyPaddleX;
volatile uint16_t enemyPaddleY;
volatile uint16_t ballX;
volatile uint16_t ballY;
uint32_t playerScore;
uint32_t enemyScore;

void keyboard_input(char key) { //player control
    if (key == 'w') {
        playerPaddleY -= 3;
    }
    else if (key == 's') {
        playerPaddleY += 3;
    }
}

//pong time baby!
void kernel_main() {
    playerPaddleX = 10;
    playerPaddleY = 90;
    enemyPaddleX = 310;
    enemyPaddleY = 90;
    ballX = 160;
    ballY = 100;
    ballDirection = upRight;
    playerScore = 0;
    enemyScore = 0;

    isr_install();
    irq_install();
    clock_t gameClock;
    init_clock(&gameClock);

    loop:
        if (get_elapsed_time_ms(&gameClock) > 10) {
            if (ballY <= 0 || ballY >= 195) { //bounce ball (walls)
                switch (ballDirection) { 
                    case upLeft:
                        ballDirection = downLeft;
                        break;
                    case upRight:
                        ballDirection = downRight;
                        break;
                    case downLeft:
                        ballDirection = upLeft;
                        break;
                    case downRight:
                        ballDirection = upRight;
                        break;
                }
            }
            if (ballX <= 0 || ballX >= 315) { //reset ball position
                ballX = 160;
                ballY = 100;
                if (ballX <= 0) enemyScore++;
                else playerScore++;
            }
            else if (ballX <= 15) {//left paddle collision detection
                if (ballY + 5 > playerPaddleY && ballY < playerPaddleY + 20) {
                    if (ballDirection == downLeft) {
                        ballDirection = downRight;
                    }
                    else {
                        ballDirection = upRight;
                    }
                }
            }
            else if (ballX >= 305) { //right paddle collision detection 
                if (ballY + 5 > enemyPaddleY && ballY < enemyPaddleY + 20) {
                    if (ballDirection == downRight) {
                        ballDirection = downLeft;
                    }
                    else {
                        ballDirection = upLeft;
                    }
                }
            }
            else if (ballX >= 250) { //enemy paddle "ai"
                if (enemyPaddleY <= 0) { //doesn't go off edge
                    enemyPaddleY++;
                }
                else if (enemyPaddleY >= 200) {
                    enemyPaddleY--;
                }
                else if (ballY + 2 > enemyPaddleY + 10) { //tries to keep centered on ball
                    enemyPaddleY++;
                }
                else {
                    enemyPaddleY--;
                }
            }
            switch (ballDirection) {
                case upLeft:
                    ballX -= 1;
                    ballY -= 1;
                    break;
                case upRight:
                    ballX += 1;
                    ballY -= 1;
                    break;
                case downLeft:
                    ballX -= 1;
                    ballY += 1;
                    break;
                case downRight:
                    ballX += 1;
                    ballY += 1;
                    break;
            }
            restart_clock(&gameClock);
        }
        clear();
        draw_box(ballX, ballY, ballX + 5, ballY + 5, WHITE);
        draw_box(playerPaddleX, playerPaddleY, playerPaddleX + 5, playerPaddleY + 20, WHITE);
        draw_box(enemyPaddleX, enemyPaddleY, enemyPaddleX + 5, enemyPaddleY + 20, WHITE);
        write();
    goto loop;
}