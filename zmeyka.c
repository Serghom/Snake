//gcc -o zmeyka.exe -Wall zmeyka.c -lfreeglut -lglu32 -lglaux -lopengl32 -IGLU32 -IGLU
//gcc -o zmeyka.exe -Wall zmeyka.c -lglut32 -lglu32 -lglaux -lopengl32 -IGLU32 -IGLU

#include <windows.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 45

#define TRUE 1
#define FALSE 0

#define MAXTAB 1000




struct settings {
	int delay;
	float scale;
	int WinScale;
	int WinPosX;
	int WinPosY;
	int StartPosX;
	int StartPosY;
	int StartSize;
	int difficult;
	int death;
}settings;

struct food {
	int x;
	int y;
}food;

struct Sfood {
	int x;
	int y;
}Sfood;



struct snake {
	int direction;
	int size;
	int turn;
	int x;
	int y;
}snake;

struct tails{
	int x;
	int y;
}tails[WIDTH*HEIGHT];

struct Player{
	char Name[100];
	int Score;
	
}Players[MAXTAB];



void init (int StartPosX, int StartPosY){
	tails[0].x = StartPosX;
	tails[0].y = StartPosY;
}


void swap(struct Player *Pl, int j);
void color_tail(int i);

int NoZap = 0, Nomber;

void sort (struct Player *Pla){
	int i, j ,buf;

	for(i = 0 ; i < Nomber - 1; i++) { 
		for(j = 0 ; j < Nomber - i - 1; j++) {  
			if(Pla[j].Score < Pla[j + 1].Score) {           
				buf = Pla[j + 1].Score;
				Pla[j + 1].Score = Pla[j].Score ;
				Pla[j].Score = buf;

				swap(Players, j);
			
			}
		}
	}
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// void COLOR (){

// 	SetConsoleTextAttribute(hConsole,(WORD) ((0 << 4) | 7));

//}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void swap(struct Player *Pl, int j){
	char buf1[100];

	strcpy(buf1, Pl[j + 1].Name);
	strcpy(Pl[j + 1].Name, Pl[j].Name);
	strcpy(Pl[j].Name, buf1);

}

void dead (){

	settings.death = FALSE;
	int i;
	for (i = snake.size; i >= 1 ; --i){
		if (i % 5 == 0) color_tail(i);
	}

	system("cls");
	Players[Nomber].Score = snake.size-settings.StartSize;
	printf("Your Score: %d\n\n", Players[Nomber].Score);


	FILE *PScore;
	PScore = fopen ("PlayerScore.ps", "a");


	NoZap++;
	if (NoZap <= 1){
	FILE *PNomber;
	PNomber = fopen ("PlayerNomber.ps", "w");
	fprintf(PNomber,"%d", Nomber);
	fclose(PNomber);
		fprintf(PScore, "%s %d\n",Players[Nomber].Name, Players[Nomber].Score);
		fclose(PScore);
		//rewind(PScore);

		PScore = fopen ("PlayerScore.ps", "r");
		for(i = 0; i < Nomber; i++){
			fscanf(PScore, "%s", Players[i].Name);
			fscanf(PScore, "%d", &Players[i].Score);
		}
		sort(Players);

	}
	
	for(i = 0; i < Nomber; i++){
		printf("=================================================\
			  \n|| %3d ||%29s || %4d ||\
			  \n", i+1, Players[i].Name, Players[i].Score);
	}
	printf("=================================================\n");
}


void draw_Sfood(){
	//int i;
	
	//for (i = 0; i < WIDTH*HEIGHT; i++){
		glColor3f(1, 1, 1);
		glVertex2f(Sfood.x*settings.scale,Sfood.y*settings.scale);
		glColor3f(1, 0, 0);
		glVertex2f(Sfood.x*settings.scale+settings.scale,Sfood.y*settings.scale);
		glColor3f(1, 1, 1);
		glVertex2f(Sfood.x*settings.scale+settings.scale,Sfood.y*settings.scale+settings.scale);
		glColor3f(1.0, 0, 0.0);
		glVertex2f(Sfood.x*settings.scale,Sfood.y*settings.scale+settings.scale);
	//}
}





void spawnSfood(){

	Sfood.x = (rand()%WIDTH);
	Sfood.y = (rand()%HEIGHT);
	

}

int sf = 0;
void spawnfood (){
	int i;
	sf++;
	food.x = (rand()%WIDTH);
	food.y = (rand()%HEIGHT);
	for(i = 0; i <snake.size; i++){
		if((food.x == tails[i].x)&&(food.y == tails[i].y)){
			spawnfood();
		}
		if ((sf % 5) == 0){
			spawnSfood();
		}

	}
}


void color_tail(int i){
	
	glColor3d(128,128,0);
	glVertex2f(tails[i].x*settings.scale,tails[i].y*settings.scale);
	glColor3f(0,0.5,0);
	glVertex2f(tails[i].x*settings.scale+settings.scale,tails[i].y*settings.scale);
	glColor3d(128,128,0);
	glVertex2f(tails[i].x*settings.scale+settings.scale,tails[i].y*settings.scale+settings.scale);
	glColor3f(0,0.5,0);
	glVertex2f(tails[i].x*settings.scale,tails[i].y*settings.scale+settings.scale);
	
}


void draw_food(){
	glColor3f(0,1,1);
	glVertex2f(food.x*settings.scale,food.y*settings.scale);
	glVertex2f(food.x*settings.scale + settings.scale,food.y*settings.scale);
	glVertex2f(food.x*settings.scale + settings.scale,food.y*settings.scale + settings.scale);
	glVertex2f(food.x*settings.scale,food.y*settings.scale + settings.scale);
 
}

void draw_Snake(){
	
	glColor3f(1,1,1);
	glVertex2f(snake.x*settings.scale,snake.y*settings.scale);
	glVertex2f(snake.x*settings.scale+settings.scale,snake.y*settings.scale);
	glVertex2f(snake.x*settings.scale+settings.scale,snake.y*settings.scale+settings.scale);
	glVertex2f(snake.x*settings.scale,snake.y*settings.scale+settings.scale);
	
}

void draw_tail(){
	int i;
	for(i = 0; i < snake.size; i++){
		
		glColor3f(0,0.5,0);
		glVertex2f(tails[i].x*settings.scale,tails[i].y*settings.scale);
		glColor3d(255,255,0);
		glVertex2f(tails[i].x*settings.scale+settings.scale,tails[i].y*settings.scale);
		glColor3f(0,0.5,0);
		glVertex2f(tails[i].x*settings.scale+settings.scale,tails[i].y*settings.scale+settings.scale);
		glColor3d(255,255,0);
		glVertex2f(tails[i].x*settings.scale,tails[i].y*settings.scale+settings.scale);

	}
}

int i = 1;
void move(){
	/*
	printf("______________________\
		  \n|     Settings       |\
		  \n======================\
		  \n|StartSize = %4d    |\
		  \n|StartPosX = %4d    |\
		  \n|StartPosY = %4d    |\
		  \n|delay     = %4d    |\
		  \n|scale     = %2.2f    |\
		  \n|WinScale  = %4d    |\
		  \n|WinPosX   = %4d    |\
		  \n|WinPosY   = %4d    |\
		  \n======================",\
		 settings.StartSize, settings.StartPosX, settings.StartPosY, settings.delay,\
		 settings.scale, settings.WinScale, settings.WinPosX, settings.WinPosY);
	*/
	

	if(settings.death){

		system("cls");
		printf("======================\
		      \n|    Score: %4d     |\
		  	  \n======================\
		  	  \n|    SFood:  %3d     |\
		  	  \n======================\
		  	  \n|    GSpeed: %5.1f   |\
		  	  \n======================\
		  	  \n| Difficult: %s        \
		  	  \n======================\n\n", snake.size-settings.StartSize, 5 - (sf % 5), (10 - (float) settings.delay / 10), (settings.difficult == 1 ? "Easy    |" : "Normal  |"));

		for (i = snake.size; i >= 1 ; --i){
			tails[i].x = tails[i - 1].x; 
			tails[i].y = tails[i - 1].y;
			if (i % 5 == 0) color_tail(i);
		}
		switch (snake.direction){
		case GLUT_KEY_UP :
			tails[0].y++;
		
			break;
		case GLUT_KEY_DOWN :
			tails[0].y--;
			
			break;
		case GLUT_KEY_LEFT :
			tails[0].x--;
			
			break;
		case GLUT_KEY_RIGHT :
			tails[0].x++;

			break;
		}
		if(tails[0].x < 0)tails[0].x += WIDTH;
		if(tails[0].x >= WIDTH)tails[0].x -= WIDTH;
		if(tails[0].y < 0)tails[0].y += HEIGHT;
		if(tails[0].y >= HEIGHT)tails[0].y -= HEIGHT;

		snake.x = tails[0].x;
		snake.y = tails[0].y;
	}	
}	


void eat(){
	int i;
	if((tails[0].x == food.x)&&(tails[0].y == food.y)){
		snake.size++;
		spawnfood();
	}
	if((tails[0].x == Sfood.x)&&(tails[0].y == Sfood.y)){
		snake.size += 5;
		Sfood.x = -1;
		Sfood.y = -1;
		for (i = 0; i < 5; i++){
			tails[snake.size -i -1].x = -1;
			tails[snake.size -i -1].y = -1;
		}
		if (settings.delay > 0) settings.delay += -5;
		
	}
}


void cut (){
	for(i = 3; i < snake.size; i++){
		if((tails[0].x == tails[i].x)&&(tails[0].y == tails[i].y)){
			if (settings.difficult == 1){
				snake.size = i;	
			}  
			else {
				dead();
			}
		}
	}
}


void keyboard(int key, int x,int y){
	switch(key){
	case GLUT_KEY_UP :
		if((snake.direction != GLUT_KEY_DOWN)&&(snake.turn))snake.direction = GLUT_KEY_UP;
		snake.turn = FALSE;
		printf("UP\n");
		//snake.y++;
		break;
	case GLUT_KEY_DOWN :
		if((snake.direction != GLUT_KEY_UP)&&(snake.turn))snake.direction = GLUT_KEY_DOWN;
		snake.turn = FALSE;
		//snake.y--;
		printf("DOWN\n");
		break;
	case GLUT_KEY_LEFT :
		if((snake.direction != GLUT_KEY_RIGHT)&&(snake.turn))snake.direction = GLUT_KEY_LEFT;
		snake.turn = FALSE;
		//snake.x--;
		printf("LEFT\n");
		break;
	case GLUT_KEY_RIGHT :
		if((snake.direction != GLUT_KEY_LEFT)&&(snake.turn))snake.direction = GLUT_KEY_RIGHT;
		snake.turn = FALSE;
		//snake.x++;
		printf("RIGHT\n");
		break;

	}
}

void imag (){

}



void Timer (int value){
	snake.turn = TRUE;
	glutPostRedisplay();
	glutTimerFunc(settings.delay,Timer,0);
	//draw_tail();
}

//int zas = 0;
void draw (){

	
//	zas++;
	imag();
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
		eat();
		draw_Sfood();
		draw_food();
		draw_tail();
		draw_Snake();
		move();
		cut();
	glEnd();
	glutSwapBuffers();
}

int main(int argc, char **argv) {

//=-=-=-=-=-=-=-Файл=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-Настройки-=-=-=-=-=-=-=-=-=-=-=-=-=

	settings.StartSize = 3;
	settings.StartPosX = (int)(WIDTH/2);
	settings.StartPosY = (int)(HEIGHT/2);
	settings.delay = 100;
	settings.scale = 1;
	settings.WinScale = 10;
	settings.WinPosX = 200;
	settings.WinPosY = 0;
	settings.death = TRUE;


	FILE *PNomber;
	PNomber = fopen ("PlayerNomber.ps", "r");
	fscanf(PNomber, "%d", &Nomber);
	Nomber++;
	fclose(PNomber);


	// HANDLE hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
	// SetConsoleTextAttribute(hConsole,(WORD) ((1) | 10));


	printf("Build 2.1.7\n\nYour Name: ");
	scanf("%s", Players[Nomber].Name);
	//SetConsoleTextAttribute(hConsole,(WORD) ((0 << 4) | 15));
	printf("Hi %s\n", Players[Nomber].Name);
	Sleep(1000);
	printf("_________________\
		  \n|  Difficult    |\
		  \n=================\
	 	  \n| 1 - Easy      |\
	 	  \n| 2 - Normal    |\
	 	  \n=================\
	 	  \n| Your shoice:  |\b\b");
	scanf("%d", &settings.difficult);
	
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=Змейка=-=-=-=-=-=-=-=-=-=-=-=-=-

	snake.turn = FALSE;
	snake.size = settings.StartSize;
	snake.direction = GLUT_KEY_UP;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=Супер Еда=-=-=-=-=-=-=-=-=-=-=-=

	Sfood.x = -1;
	Sfood.y = -1;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=Спавн Змеии=-=-=-=-=-=-=-=-=-=-=

	init(settings.StartPosX, settings.StartPosY);

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


//NoZap++;
	// if (NoZap <= 1){
	// 	
	// 	fprintf(PScore, "%s %d\n",Player[n].Name, Player[n].Score);
	// 	rewind(PScore);
	// 	fprintf(PScore,"%d", n);
	// }
	




	srand(time(NULL));
	spawnfood();
	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(WIDTH*settings.WinScale,HEIGHT*settings.WinScale);
	glutInitWindowPosition(settings.WinPosX, settings.WinPosY);
	glutCreateWindow("***IT's a SNAKE***");
	glutDisplayFunc(draw);
//	glutTimerFunc(2000,Timer1,0);
	glutTimerFunc(settings.delay,Timer,0);
	glutSpecialFunc(keyboard);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,WIDTH,0,HEIGHT,-1,1);
 
	// Основной цикл GLUT
	glutMainLoop();
 
	return 0;
}