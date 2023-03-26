#include "eyebot.h"
#include "math.h"
#include <stdio.h>

#define X_GOAL 500
#define Y_GOAL -500
#define ANGLE_GOAL 0
#define INTERVAL 0.01
#define SPEED 100
#define ANGLESPEED 20

int currentXposition, currentYposition, currentAngle;

//Hermit spline calculation
int P(double u, double pk, double pk1, double dpk, double dpk1){
    double u2, u3, h1, h2, h3, h4;
    
    u2 = u * u;
    u3 = u2 * u;

    h1 = 2*u3-3*u2+1;
    h2 = -2*u3 + 3*u2;
    h3 = u3 - 2*u2 + u;
    h4 = u3 - u2;

    return pk*h1 + pk1*h2 + dpk*h3 + dpk1*h4;

}


// Task 1
void splineDrive(int goalX, int goalY, int alpha){
    
    LCDClear();
    //using variables from lecture notes

    //Get initial position
    int ax, ay, bx, by;
    VWGetPosition(&currentXposition, &currentYposition, &currentAngle);
    //printf("x = %i, y = %i, angle = %i\n", currentXposition, currentYposition, currentAngle);
    
    float Dax, Day, Dbx, Dby, len;

    //Calculate Dbx
    //Calculate DBy
    //Calculate length to position
    int xDist = goalX;
    int yDist = goalY;
    len = sqrt(xDist*xDist+yDist*yDist);
    Dax = len * cos(currentAngle);
    Day = len * sin(currentAngle);
    Dbx = len * cos(alpha);
    Dby = len * sin(alpha);
    ax = currentXposition;
    ay = currentYposition;
    bx = xDist;
    by = yDist;


    double sx, sy;
    int sphi;
    double lastx = 0.0;
    double lasty = 0.0;

    for (float u = 0.0; u <= 1.0; u+=INTERVAL){
        

        sx = P(u, ax, bx, Dax, Dbx);
        sy = P(u, ay, by, Day, Dby);


        //printf("h1 = %f, h2 = %f, h3 = %f, h4 = %f, sx = %f, sy = %f\n", h1, h2, h3, h4, sx, sy);
        
        double yDif = sy-lasty;
        double xDif = sx-lastx;
        sphi = round(atan2(yDif, xDif)*180/M_PI);
        VWCurve(len*INTERVAL, sphi-currentAngle, SPEED);
        VWWait();


        VWGetPosition(&currentXposition, &currentYposition, &currentAngle);
        //printf("x = %i, y = %i, angle = %i\n", currentXposition, currentYposition, currentAngle);

        lastx = sx;
        lasty = sy;
    }
    // Update and log the current position at the end
    
    VWGetPosition(&currentXposition, &currentYposition, &currentAngle);
    printf("x = %i, y = %i, angle = %i\n\n", currentXposition, currentYposition, currentAngle);
    
}


// Task 2
void Task2(){
    
    // Set the initial position to comply with world coordinates position
    VWGetPosition(&currentXposition, &currentYposition, &currentXposition);

    // Read the file using the supplied duction from readfile.c
    FILE* file = fopen("way.txt", "r");
    int points[20][2];
    int i = 0;
    while (fscanf(file, "%d %d", &points[i][0], &points[i][1])==2){

        i++;
    }
    fclose(file);


    //run forever
    while(true) {
        //int nextAngle; int dist;?
        for (int j = 0; j < i; j++){//go through each point
            VWGetPosition(&currentXposition, &currentYposition, &currentAngle);
            
            int x = points[j][0];
            int y = points[j][1];
            printf("GoalX = %i, GoalY = %i\n", x, y);
            //calculate movement required to get to next point
            //int nx = x-currentXposition;
            //int ny = y-currentYposition;
            //nextAngle = atan(ny/nx)-currentAngle;
            splineDrive(x, y, 0);



            //move to the next new point

            //splineDrive(required_x, required_y, required_alpha)


        }
    }
}



int main() {
    //resets robots location in simulator
    SIMSetRobot(0, 1000, 500, 0 , -90);

    //Menu for task 1 and 2
    LCDMenu("Task1", "Task 2", "", "Break");
    while (1){
        int key = KEYRead();
        if (key == KEY1){
            printf("\n\n        Task 1\n\n\n");
            splineDrive(X_GOAL, Y_GOAL, ANGLE_GOAL);
            break;
        } 
        if (key == KEY2) {
            printf("\n\n        Task 2\n\n\n");
            printf("x = %i, y = %i, angle = %i\n\n", currentXposition, currentYposition, currentAngle);
            Task2();
        }
        if (key == KEY4){
            break;
        }

    }
    return EXIT_SUCCESS;
}