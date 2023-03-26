#include "eyebot.h"
#include "math.h"
#include <stdio.h>

#define X_GOAL 0
#define Y_GOAL 1000
#define ANGLE_GOAL 90
#define INTERVAL 0.01
#define SPEED 100
#define ANGLESPEED 20

int currentXposition, currentYposition, currentAngle;

//Hermit spline calculation
int P(double u, double pk, double pk1, double dpk, double dpk1){
    //TODO
    //Code fuction p to return a value
    int x = 0;
    return x;
}


// Task 1
void splineDrive(int goalX, int goalY, int alpha){
    
    LCDClear();
    //using variables from lecture notes

    //Get initial position
    int bx, by;
    VWGetPosition(&currentXposition, &currentYposition, &currentAngle);
    printf("x = %i, y = %i, angle = %i\n", currentXposition, currentYposition, currentAngle);
    
    float Dax, Day, Dbx, Dby, len;

    //Calculate Dbx
    //Calculate DBy
    //Calculate length to position
    int xDist = goalX-currentXposition;
    int yDist = goalY-currentYposition;
    len = sqrt(xDist*xDist+yDist*yDist);
    Dax = len;
    Day = 0;
    Dbx = len * cos(alpha);
    Dby = len * sin(alpha);
    bx = xDist;
    by = yDist;


    float u2, u3, h1, h2, h3, h4, sx, sy;
    int sphi;
    float lastx = 0.0;
    float lasty = 0.0;

    for (float u = 0.0; u <= 1.0; u+=INTERVAL){
        u2 = u * u;
        u3 = u2 * u;

        h1 = 2*u3-3*u2+1;
        h2 = -2*u3 + 3*u2;
        h3 = u3 - 2*u2 + u;
        h4 = u3 - u2;

        sx = currentXposition*h1 + bx*h2 + Dax*h3 + Dbx*h4;
        sy = currentYposition*h1 + by*h2 + Day*h3 + Dby*h4;


        //printf("h1 = %f, h2 = %f, h3 = %f, h4 = %f, sx = %f, sy = %f\n", h1, h2, h3, h4, sx, sy);
        
        
        sphi = round(atan2(sy-lasty, sx-lastx)*180/M_PI);
        VWCurve(len*INTERVAL, sphi-currentAngle, SPEED);
        VWWait();


        VWGetPosition(&currentXposition, &currentYposition, &currentAngle);
        printf("x = %i, y = %i, angle = %i\n", currentXposition, currentYposition, currentAngle);

        lastx = sx;
        lasty = sy;
    }
    // Update and log the current position at the end
    
    VWGetPosition(&currentXposition, &currentYposition, &currentAngle);
    printf("x = %i, y = %i, angle = %i\n", currentXposition, currentYposition, currentAngle);
    
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
        int nextAngle;// int dist;?
        for (int j = 0; j < i; j++){//go through each point
            VWGetPosition(&currentXposition, &currentYposition, &currentAngle);
            int x = points[j][0];
            int y = points[j][1];
            //calculate movement required to get to next point
            int nx = x-currentXposition;
            int ny = y-currentYposition;
            nextAngle = atan(ny/nx)-currentAngle;
            printf("next x = %i, next y = %i, next angle = %i\n", x, y, nextAngle);
            splineDrive(nx, ny, nextAngle);



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
            Task2();
        }
        if (key == KEY4){
            break;
        }

    }
    return EXIT_SUCCESS;
}