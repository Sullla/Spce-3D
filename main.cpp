//Windows includes - DO NOT EDIT AS LOTS OF INTERDEPENDENCE



#include <string.h>

#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <GL\GL.h>
#include <iostream>
#include <math.h>
#include <string.h>
#ifdef WIN32
#include "gltools.h"
#include <windows.h>		
#include <gl\gl.h>			
#include <gl\glu.h>			
#include "glm.h"

#endif



#pragma region variables



float xpos, ypos, zpos;
GLfloat xRotated, yRotated, zRotated;
GLfloat planetX, planetY, planetZ;
GLdouble innerRaidus = 0.3;
GLdouble outterRaidus = 1;
GLint sides = 50;
GLint rings = 50;
int appear = 1;
bool tf = true;

int appear2 = 1;
bool tf2 = true;

int appear3 = 1;
bool tf3 = true;

int appear4 = 1;
bool tf4 = true;


#define TEXTURE_COUNT 3
GLuint  textures[TEXTURE_COUNT];

float xball = 0.0;
float yball = 0.0;
float zball = -4;
//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char *textureFiles[TEXTURE_COUNT] = {"jupiter.tga", "stars.tga","hud.tga"};
float angle = 0.0;

//  camera's direction
float lookx = 0.0f, lookz = -1.0f, looky = 0.0f;
float upx = 0.0, upy = 1.0, upz = 0.0f;
// camera position
float x = 0.0f, z = -15.0f, y = 0.0f;

GLfloat mKa[4] = {0.11f,0.06f,0.11f,1.0f}; //ambient
GLfloat mKd[4] = {0.43f,0.47f,0.54f,1.0f}; //diffuse
GLfloat mKs[4] = {1.0f,1.0f,1.0f,1.0f}; //specular
GLfloat mKe[4] = {0.5f,0.5f,0.0f,1.0f}; //emission

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_colormap[] = { 16.0, 47.0, 79.0 };

float xsphere = 0.0f;
float ysphere = 0.75f;
float zsphere = 0.0f;
float xsphere1 = 0.0f;
float ysphere1 = 0.0f;
float zsphere1 = 0.0f;
double radiushud = 5.0;

double xcentre = 0.0;
double ycentre = 0.0;
double zcentre = 0.0;

GLfloat xscale1 = 0.5;
GLfloat yscale1 = 0.5;
GLfloat zscale1 = 0.5;
//cross product
float viewX = lookx - x;
float viewY = looky - y;
float viewZ = lookz - z;
float x2 = ((viewY * upz) - (viewZ * upy));
float y2 = ((viewZ * upx) - (viewX * upz));
float z2 = ((viewX * upy) - (viewY * upx));

bool onOff = true;
const double radius2 = 2.0;
float crystalx;
float crystaly;
float crystalz;
//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte *pBytes0;
#pragma endregion


void displayText(float x, float y,float z, int r, int g, int b, const char *string) // Displays text
{
	int j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos3f(x, y,z);
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

bool collision2(double x1, double y1, double z1,double r1, double  x2, double y2,double z2, double r2) // Collision for asteroids by finding a distance between bulley and asteroids
{
	//just to not forget here is the Pythagoras formula for distance
	// Square root(squared(x2-x1) + squared(y2-y1));
	int sqrX = x2 - x1;
	int sqrY = y2 - y1;
	int sqrZ = z2 - z1;
	int radSum = r1 + r2;

	int collide = (sqrX * sqrX) + (sqrY * sqrY) + (sqrZ * sqrZ) <= radSum * radSum;

	return collide;

}

void drawTexturedQuad(int image)
{
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
   // glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable( GL_TEXTURE_2D );
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0,0.0);
    glVertex3f(-50.0, 0.0,100.0);
    glTexCoord3f(1.0,0.0,0.0);
    glVertex3f(50.0, 0.0,100.0);
    glTexCoord2f(1.0,1.0);
    glVertex3f(50.0, 100.0,100.0);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-50.0, 100.0,100.0);
    glEnd();
    glDisable( GL_TEXTURE_2D );
    
}
void drawTorus()
{
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTranslatef(0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(yRotated , 1.0, 0.0, 0.0);
	glRotatef(xRotated, 0.0, 1.0, 0.0);
	glRotatef(zRotated +180, 0.0, 0.0, 1.0);
	glScalef(2.5, 2.5, 2.5);
	glutSolidTorus(innerRaidus, outterRaidus, sides, rings);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}




void drawHUD(int image)
{
	
	GLUquadricObj *hud;
	
	glColor4f(1.0, 1.0, 1.0, 0.10);
	hud = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	gluQuadricDrawStyle(hud, GLU_FILL);
	gluQuadricNormals(hud, GLU_SMOOTH);
	gluQuadricOrientation(hud, GLU_INSIDE);
	gluQuadricTexture(hud, GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
    glTranslatef(xball, yball, zball);
	glRotatef(xsphere1+90, 1.0, 0.0, 0.0);
	glRotatef(ysphere1, 0.0, 1.0, 0.0);
	glRotatef(zsphere1, 0.0, 0.0, 1.0);
	glScalef(xscale1, yscale1,zscale1);
	
	gluSphere(hud, radiushud, 50, 20);
	

	glDisable(GL_TEXTURE_2D);


}

#pragma region crystal positions



int i = 0;
int j = -30;
int k = 0;

int crys2x = 6;
int crys2y = 4;
int crys2z = -34;

int crys3x = 5;
int crys3y = -6;
int crys3z = -25;

int crys4x = -4;
int crys4y = 3;
int crys4z = -30;
#pragma endregion
double radius = 5.0f;
GLfloat xscale = 0.5;
GLfloat yscale = 0.5;
GLfloat zscale = 0.5;

void drawSphere() {

	glTranslatef(xsphere, xsphere, xsphere);
	glColor3f(0.0, 1.0, 1.0);
	glRotatef(crystalz, 1.0, 0.0, 0.0);
	glRotatef(crystaly, 0.0, 1.0, 0.0);
	glRotatef(crystalx, 0.0, 0.0, 1.0);
	glScalef(0.5, 0.5, 0.5);
	glutSolidDodecahedron();
	glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_RESCALE_NORMAL);
	glBlendFunc(GL_ONE, GL_SRC_COLOR);


}


void drawTorusWithCrystal()
{
	glPushMatrix();
	glTranslatef(i, k, j);
	drawSphere();
	drawTorus();
	yRotated += 0.1;
	glPopMatrix();



}
void drawTorusWithCrystal2()
{
	glPushMatrix();
	glTranslatef(crys2x, crys2y, crys2z);
	drawSphere();
	drawTorus();
	yRotated += 0.1;
	glPopMatrix();



}
void drawTorusWithCrystal3()
{
	glPushMatrix();
	glTranslatef(crys3x, crys3y, crys3z);
	drawSphere();
	drawTorus();
	yRotated += 0.1;
	glPopMatrix();



}

void drawTorusWithCrystal4()
{
	glPushMatrix();
	glTranslatef(crys4x, crys4y, crys4z);
	drawSphere();
	drawTorus();
	yRotated += 0.1;
	glPopMatrix();

}
void collisionHandler()
{//1
	if (tf = true && appear == 1) {


		drawTorusWithCrystal();
	
	}
	else
	{
		appear = 0;
	}
	if (collision2(i, k, j, 2, x, y, z, 0)) {
		tf = false;
		appear = 0;

	}
//2

	if (tf2 = true && appear2 == 1) {


		drawTorusWithCrystal2();
		
	}
	else
	{
		appear2 = 0;
	}
	if (collision2(crys2x, crys2y, crys2z, 2, x, y, z, 0)) {
		tf2 = false;
		appear2 = 0;

	}

//3
	if (tf3 = true && appear3 == 1) {


		drawTorusWithCrystal3();
		
	}
	else
	{
		appear3 = 0;
	}
	if (collision2(crys3x, crys3y, crys3z, 2, x, y, z, 0)) {
		tf3 = false;
		appear3 = 0;

	}
	//4

	if (tf4 = true && appear4 == 1) {


		drawTorusWithCrystal4();

	}
	else
	{
		appear4 = 0;
	}
	if (collision2(crys4x, crys4y, crys4z, 2, x, y, z, 0)) {
		tf4 = false;
		appear4 = 0;

	}
	crystalz++;
}

GLUquadricObj *quadricFootball;

void Jupiter() 
{
	
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glColor3f(0.4, 0.4, 0.4);
	quadricFootball = gluNewQuadric();
	gluQuadricDrawStyle(quadricFootball, GLU_FILL);
	gluQuadricNormals(quadricFootball, GLU_SMOOTH);
	gluQuadricOrientation(quadricFootball, GLU_OUTSIDE);
	gluQuadricTexture(quadricFootball, GL_TRUE);

	glTranslatef(-5.0, 10, -50);
	glRotatef(planetX, 1.0, 0.0, 0.0);
	glRotatef(planetY, 0.0, 1.0, 0.0);
	glRotatef(planetZ + 90, 0.0, 0.0, 1.0);
	glTranslatef(0.0, 0.0, 0.0);
	gluSphere(quadricFootball, 5.0f, 50, 35);
	glDisable(GL_TEXTURE_2D);
	
}


void sphericalbackground()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glColor3f(0.4, 0.4, 0.4);
	quadricFootball = gluNewQuadric();
	gluQuadricDrawStyle(quadricFootball, GLU_FILL);
	gluQuadricNormals(quadricFootball, GLU_SMOOTH);
	gluQuadricOrientation(quadricFootball, GLU_INSIDE);
	gluQuadricTexture(quadricFootball, GL_TRUE);
	gluSphere(quadricFootball, 80, 50, 35);

	glDisable(GL_TEXTURE_2D);
}

void displayText(float x, float y, int r, int g, int b, const char *string) // Displays text
{
	int j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}



void display(void)
{
	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	
	gluLookAt(x, y, z,
		x +lookx, y + looky, z + lookz,
		upx, upy, upz);
	displayText(x, y, 1.0, 0.0, 0.0, "string");
	sphericalbackground();
	collisionHandler();
		glPushMatrix();
	    Jupiter();	
		planetZ+=0.05;
	glPopMatrix();
	glLoadIdentity();


	
	if (onOff == true)
	{
		
		displayText(xball, yball, zball, 1.0, 0.0, 1.0, "Coordinates");
		char buf[100] = { 0 };
		

		glColor3f(1.0, 0.0, 1.0);
		snprintf(buf, 50, " %.1f", xball);
		drawHUD(2);
		
	
	}
	else
	{
	

	}
	
	glutSwapBuffers();
	glFlush();
	

}
void processNormalKeys(unsigned char key, int x, int y)
{

	if (key == 27)
		exit(0);
}
void strafeControl(unsigned char key, int xx, int yy) {

	float fraction = 0.5f;

	float magnitude = sqrt((x2 * x2) + (y2* y2) + (z2 * z2));
	x2 /= magnitude;
	y2 /= magnitude;
	z2 /= magnitude;

	switch (key) {
	case 'a':
		x += x2;
		y += y2;
		z += z2;
		
		break;
	case 'd':
		x -= x2;
		y -= y2;
		z -= z2;
		
		
		break;
	case 'w':
		x += lookx * fraction;
		z += lookz * fraction;
		xball += sin((3.14 / 180)*xsphere1) * 0.001;
		zball += -cos((3.14 / 180)*zsphere1) * 0.001;
		
		break;
	case 's':
		x -= lookx * fraction;
		z -= lookz * fraction;
		y -= looky * fraction;
		xball -= sin((3.14 / 180)*xsphere1) * 0.001;
		zball -= -cos((3.14 / 180)*zsphere1) * 0.001;
		break;
	case 'e':
	
			onOff = !onOff;
	}
	glutPostRedisplay();
}
void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.5f;


	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lookx = sin(angle);
		lookz = -cos(angle);
		xsphere1 =angle;
		zsphere1 = angle;
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lookx = sin(angle);
		lookz = -cos(angle);
		xsphere1 = angle;
		zsphere1 = angle;
		break;
	case GLUT_KEY_UP:
		y +=  fraction;
		yball += ysphere1 *fraction;
		break;
	case GLUT_KEY_DOWN:
		y -= fraction;
		yball -= ysphere1 * fraction;
		break;
	
	}
	glutPostRedisplay();
}

// This function does any needed initialization on the rendering
void init(void)
{

	
	GLuint texture;

	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	glGenTextures(TEXTURE_COUNT, textures);
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		free(pBytes0);
	}
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
										  // Enable depth testing for z-culling



		
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,mKa);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_COLOR_INDEXES, mat_colormap);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, mKa);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, mKd);
	glLightfv(GL_LIGHT2, GL_SPECULAR, mKs);
//glMaterialf(GL_FRONT, GL_DIFFUSE, 20);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_RESCALE_NORMAL);
	//glBlendFunc(GL_ONE, GL_SRC_COLOR);
	

}


void TimerFunc(int value)
{
	glutSwapBuffers();
	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);
}

void reshape(int width, int height)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (height == 0)
		height = 1;
	float ratio = width * 1.0 / height;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}




int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("space");
	xRotated = yRotated = zRotated = 30.0;
	xRotated = 33;
	yRotated = 40;
	planetX = planetY = planetZ = 90.0;
	planetY = 30;
	xsphere1 = ysphere1 = zsphere1 = 0;
	
	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardFunc(strafeControl);
	glutSpecialFunc(processSpecialKeys);
	glutTimerFunc(25, TimerFunc, 1);
	// OpenGL init
	init();
	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}






