#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <stdbool.h>

#define PI 3.14159265358979323846264338327950288419716939937510

typedef struct{
	double x,y,z;//x,y,z position of point 0 on cube
	int color;//0=green 1=red 2=blue 3=yellow
	bool taken;
}cube;

static int mainWin;
static float angle=0.0;
int turn;
cube cubes[3][9];
int layer = 0, num = 0, color=0;
double distance = 5.0;
double eyeX = 4.0, eyeY = 4.0, eyeZ = 1.0;
GLfloat colors[4][4] = {
	{0.0,1.0,0.0,0.5},
	{1.0,0.0,0.0,0.5},
	{0.0,0.0,1.0,0.5},
	{1.0,1.0,0.0,0.5},
};

void initCubes(){
	int i,j;
	for(i=0; i<3; i++){
		for(j=0; j<9; j++){
			cubes[i][j].x = j%3*distance;
			if(j<3)
				cubes[i][j].y = 0;
			else if(j>2 && j<6)
				cubes[i][j].y = distance;
			else
				cubes[i][j].y = distance*2;
			cubes[i][j].z = distance*i;
			cubes[i][j].color = 0;
			cubes[i][j].taken = false;
		}
	}
	cubes[0][0].color=3;
}

void drawCubes(){
	int i,j,color;
	double x,y,z;
	for(i=0; i<3; i++){
		for(j=0; j<9; j++){
			x=cubes[i][j].x;
			y=cubes[i][j].y;
			z=cubes[i][j].z;
			color=cubes[i][j].color;
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glColor4fv(colors[cubes[i][j].color]);
			glBegin(GL_QUADS);
				//LEFT FACE
				glVertex3f(x,y,z);
				glVertex3f(x,y+1.0,z);
				glVertex3f(x,y+1.0,z+1.0);
				glVertex3f(x,y,z+1.0);
				//RIGHT FACE
				glVertex3f(x+1.0,y,z);
				glVertex3f(x+1.0,y+1.0,z);
				glVertex3f(x+1.0,y+1.0,z+1.0);
				glVertex3f(x+1.0,y,z+1.0);
				//BACK FACE
				glVertex3f(x,y,z+1.0);
				glVertex3f(x,y+1.0,z+1.0);
				glVertex3f(x+1.0,y+1.0,z+1.0);
				glVertex3f(x+1.0,y,z+1.0);
				//FRONT FACE
				glVertex3f(x,y,z);
				glVertex3f(x,y+1.0,z);
				glVertex3f(x+1.0,y+1.0,z);
				glVertex3f(x+1.0,y,z);
				//BOTTOM FACE
				glVertex3f(x,y,z);
				glVertex3f(x,y,z+1.0);
				glVertex3f(x+1.0,y,z+1.0);
				glVertex3f(x+1.0,y,z);
				//TOP FACE
				glVertex3f(x,y+1.0,z);
				glVertex3f(x,y+1.0,z+1.0);
				glVertex3f(x+1.0,y+1.0,z+1.0);
				glVertex3f(x+1.0,y+1.0,z);
			glEnd();
		}
	}
}

void newGame(){
	turn=0;
	initCubes();
	int layer = 0, num = 0, color = 0;
	double eyeX = 4.0, eyeY = 4.0, eyeZ = 1.0;
}

void menu(int value){
	if(value==0){
		glutDestroyWindow(mainWin);
		exit(0);
	}else if(value==1){
		newGame();
		initCubes();
	}
	glutPostRedisplay();
}

void createMenu(){
	glutCreateMenu(menu);
	glutAddMenuEntry("New Game", 1);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(){
	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0,20.0,-20.0,20.0,-50.0,50.0);
	glViewport(0,0,800,600);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_LIGHTING);
	// glEnable(GL_LIGHT0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glLightfv(GL_LIGHT2, GL_POSITION, (GLfloat[]){1.0,2.0,5.0,1.0});
}

void display(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	gluLookAt(eyeX,eyeY,eyeZ,5.0,5.0,5.0,0.0,0.1,0.0);

	glPushMatrix();
		drawCubes();
	glPopMatrix();

	glutSwapBuffers();
}

void idle(){
	glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y){
	int newLayer,newNum;
	if(key == 'Q' || key == 'q'){
		exit(0);
	}else if(key == 'J' || key == 'j'){
		angle += (2.0*PI/30);
		eyeX=4.0+0.5*cos(angle);
		eyeY=4.0+0.5*sin(angle);
	}else if(key == 'L' || key == 'l'){
		angle -= (2.0*PI/30);
		eyeX=4.0+0.5*cos(angle);
		eyeY=4.0+0.5*sin(angle);
	}else if(key == 'S' || key == 's'){
		if(num<3)
			newNum=num+6;
		else
			newNum=num-3;
		if(cubes[layer][num].taken)//check if current cube is taken
			cubes[layer][num].color=color; //restore color if taken
		else
			cubes[layer][num].color=0;//else color is green (not taken)
		if(cubes[layer][newNum].taken)//if next cube is taken
			color=cubes[layer][newNum].color; //store the color
		cubes[layer][newNum].color=3;//highlight new cube
		num=newNum;
	}else if(key == 'W' || key == 'w'){
		if(num>5)
			newNum=num-6;
		else
			newNum=num+3;
		if(cubes[layer][num].taken)//check if current cube is taken
			cubes[layer][num].color=color; //restore color if taken
		else
			cubes[layer][num].color=0;//else color is green (not taken)
		if(cubes[layer][newNum].taken)
			color=cubes[layer][newNum].color;
		cubes[layer][newNum].color=3;
		num=newNum;
	}else if(key == 'A' || key == 'a'){
		if(num==2 || num==5 || num==8)
			newNum=num-2;
		else
			newNum=num+1;
		if(cubes[layer][num].taken)//check if current cube is taken
			cubes[layer][num].color=color; //restore color if taken
		else
			cubes[layer][num].color=0;//else color is green (not taken)
		if(cubes[layer][newNum].taken)
			color=cubes[layer][newNum].color;
		cubes[layer][newNum].color=3;
		num=newNum;
	}else if(key == 'D' || key == 'd'){
		if(num%3==0)
			newNum=num+2;
		else
			newNum=num-1;
		if(cubes[layer][num].taken)//check if current cube is taken
			cubes[layer][num].color=color; //restore color if taken
		else
			cubes[layer][num].color=0;//else color is green (not taken)
		if(cubes[layer][newNum].taken)
			color=cubes[layer][newNum].color;
		cubes[layer][newNum].color=3;
		num=newNum;
	}else if(key == '3'){
		if(layer!=2)
			newLayer=2;
		if(cubes[layer][num].taken)//check if current cube is taken
			cubes[layer][num].color=color; //restore color if taken
		else
			cubes[layer][num].color=0;//else color is green (not taken)
		if(cubes[newLayer][num].taken)
			color=cubes[newLayer][num].color;
		cubes[newLayer][num].color=3;
		layer=newLayer;
	}else if(key == '2'){
		if(layer!=1)
			newLayer=1;
		if(cubes[layer][num].taken)//check if current cube is taken
			cubes[layer][num].color=color; //restore color if taken
		else
			cubes[layer][num].color=0;//else color is green (not taken)
		if(cubes[newLayer][num].taken)
			color=cubes[newLayer][num].color;
		cubes[newLayer][num].color=3;
		layer=newLayer;
	}else if(key == '1'){
		if(layer!=0)
			newLayer=0;
		if(cubes[layer][num].taken)//check if current cube is taken
			cubes[layer][num].color=color; //restore color if taken
		else
			cubes[layer][num].color=0;//else color is green (not taken)
		if(cubes[newLayer][num].taken)
			color=cubes[newLayer][num].color;
		cubes[newLayer][num].color=3;
		layer=newLayer;
	}else if(key == ' '){
		bool taken = cubes[layer][num].taken;
		if(!taken){ //if not taken mark it
			if(turn%2==0){
				cubes[layer][num].color=1;
				cubes[layer][num].taken=true;
				turn++;
				color=1;
			}else{
				cubes[layer][num].color=2;
				cubes[layer][num].taken=true;
				turn++;
				color=2;
			}
			layer=0, num=0, color=cubes[0][0].color;
			cubes[0][0].color=3;
		}
	}
}

void mouseInput(int button, int state, int x, int y){
	//rotate face in the direction of the mouse movement
}

int main(int argc, char **argv){
	glutInit(&argc, argv); //initialize GLUT Library
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(800,600); //set Window Size
	glutInitWindowPosition(0,0); //set Window Position
	mainWin = glutCreateWindow("3D TicTacCube"); //Create window named "3D TicTacCube"

	newGame();
	createMenu();
	
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseInput);
	glutPassiveMotionFunc(passiveMotion);

	init();

	glutMainLoop(); //enter GLUT Main Loop
	return 0;
}