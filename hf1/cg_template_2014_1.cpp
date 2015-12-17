//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2014-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    :
// Neptun :
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

struct Vector {
   float x, y, z;

   Vector( ) { 
	x = y = z = 0;
   }
   Vector(float x0, float y0, float z0 = 0) { 
	x = x0; y = y0; z = z0;
   }
   Vector operator/(float a) {
	   return Vector(x / a, y / a, z / a);
   }
   Vector operator*(float a) { 
	return Vector(x * a, y * a, z * a); 
   }
   Vector operator+(const Vector& v) {
 	return Vector(x + v.x, y + v.y, z + v.z); 
   }
   Vector operator-(const Vector& v) {
 	return Vector(x - v.x, y - v.y, z - v.z); 
   }
   float operator*(const Vector& v) {
	return (x * v.x + y * v.y + z * v.z); 
   }
   Vector operator%(const Vector& v) {
	return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
   float Length() { return sqrt(x * x + y * y + z * z); }
   Vector normalize() {
	   return *this / (*this).Length();
   }
};
 
struct Color {
   float r, g, b;

   Color( ) { 
	r = g = b = 0;
   }
   Color(float r0, float g0, float b0) { 
	r = r0; g = g0; b = b0;
   }
   Color operator*(float a) { 
	return Color(r * a, g * a, b * a); 
   }
   Color operator*(const Color& c) { 
	return Color(r * c.r, g * c.g, b * c.b); 
   }
   Color operator+(const Color& c) {
 	return Color(r + c.r, g + c.g, b + c.b); 
   }
};

struct PointCenter {
	float x, y;
	long time;

	PointCenter * next;

	PointCenter() {
		x = 0;
		y = 0;
		time = 0;
	}

	PointCenter(float xCoord, float yCoord, long t) {
		x = xCoord;
		y = yCoord;
		time = t;
	}

	~PointCenter() {

	}

};

class Points {
private:
	PointCenter * first;
	PointCenter * addLast;
	PointCenter * last;
	int elements;

public:
	Points() {
		elements = 0;
	}

	void add(float x, float y, long t) {
		PointCenter * p = new PointCenter(x, y, t);
		if (first == NULL) {
			first = p;
			addLast = p;
			PointCenter * lst = new PointCenter(x, y, t + 700);
			lst->next = NULL;
			last = lst;
			++elements;
		}
		else {
			addLast->next = p;
			this->addLast = p;
			this->last->time = t + 700;
		}
		p->next = last;
		++elements;
	}

	PointCenter * getFirst() {
		return first;
	}

	PointCenter * getLast() {
		return last;
	}

	int getSize() {
		return elements;
	}

	PointCenter * getNth(int n) {
		if (0 <= n && n < (elements)) {
			PointCenter * pc = first;
			for (int i = 0; i < n; ++i) {
				pc = pc->next;
			}
			return pc;
		}
		return NULL;
	}

	~Points() {
		PointCenter * pc = first;
		while (pc != NULL) {
			PointCenter * del = pc;
			pc = pc->next;
			delete del;
		}
	}

};

const int screenWidth = 600;
const int screenHeight = 600;

int fieldWidth = 1000;
int fieldHeight = 1000;

Vector intersectCP;
Vector parabolaVect;
Vector catmullVect;

bool animate = false;
Vector viewPos;
Vector viewSpeed(2, 3, 0);
long lastFrame = 0;

Color image[screenWidth*screenHeight];

Vector vectors[850];
int parabolicVecNum = 0;
bool calculated = false;

float vectorAngle;

Points points;

float floatToMeter(float f) {
	return f * (float)fieldWidth / 2;
}

float meterToFloat(float meter) {
	return meter / ((float)fieldWidth / 2);
}

float xMeterToFloat(float meter) {
	return (meter - viewPos.x) / ((float)fieldWidth / 2);
}

float yMeterToFloat(float meter) {
	return (meter - viewPos.y) / ((float)fieldHeight / 2);
}

float xPixelToMeter(int x) {
	return (((float)x) - ((float)screenHeight / 2)) / ((float)screenWidth / 2) * ((float)fieldWidth / 2);
}

float yPixelToMeter(int y) {
	return (((float)screenHeight / 2) - ((float)y)) / ((float)screenHeight / 2) * ((float)fieldWidth / 2);
}

Vector rotate(Vector v, double angle) {
	Vector rotated;
	rotated.x = v.x*cos(angle) - v.y*sin(angle);
	rotated.y = v.x*sin(angle) + v.y*cos(angle);
	return rotated;
}

Vector speed(int pointNum) {
	Vector v;
	if (points.getSize() <= 3) {
		return v;
	}
	PointCenter p1 = *points.getNth(pointNum);
	PointCenter p0;
	long p0time;
	if (pointNum == 0) {
		p0 = *points.getNth(points.getSize() - 2);
		p0time = p1.time - 700;
	}
	else {
		p0 = *points.getNth(pointNum - 1);
		p0time = p0.time;
	}

	PointCenter p2;
	long p2time;
	if (pointNum == points.getSize() - 1) {
		p2 = *points.getNth(1);
		p2time = p1.time + 700;
	}
	else {
		p2 = *points.getNth(pointNum + 1);
		p2time = p2.time;
	}
	long fulltime = points.getLast()->time;
	long t21 = ((p2.time - p1.time) + fulltime) % fulltime;
	long t10 = ((p1.time - p0.time) + fulltime) % fulltime;
	v.x = (((xMeterToFloat(p2.x) - xMeterToFloat(p1.x)) / (p2time - p1.time)) + ((xMeterToFloat(p1.x) - xMeterToFloat(p0.x)) / (p1.time - p0time))) / 2;
	v.y = (((yMeterToFloat(p2.y) - yMeterToFloat(p1.y)) / (p2time - p1.time)) + ((yMeterToFloat(p1.y) - yMeterToFloat(p0.y)) / (p1.time - p0time))) / 2;
	return v;
}

void drawCircle(float x, float y) {

	float x2, y2;
	float angle;
	float radius = meterToFloat(5);

	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);

	for (angle = 1.0f; angle<361.0f; angle += 0.2)
	{
		x2 = x + sin(angle)*radius;
		y2 = y + cos(angle)*radius;
		glVertex2f(x2, y2);
	}

	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 360; ++i) {
		float radian = i * M_PI / 180;
		glVertex2f(x + cosf(radian) * radius, y + sinf(radian) * radius);
	}

	glEnd();

}

void calcDrawLines(float x, float y, long time) {
	if (points.getSize() <= 3) {
		return;
	}
	glColor3f(0.0f, 0.702f, 0.047f);

	Vector e1(xMeterToFloat(points.getNth(0)->x), yMeterToFloat(points.getNth(0)->y), 0);
	Vector e2(xMeterToFloat(points.getNth(1)->x), yMeterToFloat(points.getNth(1)->y), 0);
	Vector parameterLine = (e1 - e2).normalize();
	Vector tp(x, y, 0);
	Vector f(xMeterToFloat(points.getNth(2)->x), yMeterToFloat(points.getNth(2)->y), 0);

	Vector t = e1 + parameterLine*((tp - e1)*parameterLine);

	Vector n = tp - (f + t)/2;

	intersectCP.x = floatToMeter(tp.x);
	intersectCP.y = floatToMeter(tp.y);

	parabolaVect.x = floatToMeter(n.x);
	parabolaVect.y = floatToMeter(n.y);

	glBegin(GL_LINES);

	glVertex2f(x - n.x * 1000, y - n.y * 1000);
	glVertex2f(x + n.x * 1000, y + n.y * 1000);

	glEnd();

	Vector s;
	
	for (int i = 0; i < points.getSize() - 1; ++i) {
		if (points.getNth(i)->time <= time &&  time <= points.getNth(i + 1)->time) {
			PointCenter p1 = *points.getNth(i);
			PointCenter p2 = *p1.next;
			long t_i = p1.time;
			long t_ii = p2.time;
			Vector v1 = speed(i);
			Vector v2 = speed(i + 1);
			float a3_x = (2 * (xMeterToFloat(p1.x) - xMeterToFloat(p2.x)) / (pow(t_ii - t_i, 3))) + ((v2.x + v1.x) / (pow(t_ii - t_i, 2)));
			float a3_y = (2 * (yMeterToFloat(p1.y) - yMeterToFloat(p2.y)) / (pow(t_ii - t_i, 3))) + ((v2.y + v1.y) / (pow(t_ii - t_i, 2)));
			float a2_x = (3 * (xMeterToFloat(p2.x) - xMeterToFloat(p1.x)) / (pow(t_ii - t_i, 2)) - ((v2.x + 2 * v1.x) / (t_ii - t_i)));
			float a2_y = (3 * (yMeterToFloat(p2.y) - yMeterToFloat(p1.y)) / (pow(t_ii - t_i, 2)) - ((v2.y + 2 * v1.y) / (t_ii - t_i)));
			float a1_x = v1.x;
			float a1_y = v1.y;
			s.x = 3 * a3_x*(pow(time - t_i, 2)) + 2 * a2_x*(time - t_i) + a1_x;
			s.y = 3 * a3_y*(pow(time - t_i, 2)) + 2 * a2_y*(time - t_i) + a1_y;
		}
	}

	catmullVect.x = floatToMeter(s.x);
	catmullVect.y = floatToMeter(s.y);

	glBegin(GL_LINES);

	glVertex2f(x - s.x * 10000000, y - s.y * 10000000);
	glVertex2f(x + s.x * 10000000, y + s.y * 10000000);

	glEnd();
}

void drawLines() {
	glColor3f(0.0f, 0.702f, 0.047f);

	glBegin(GL_LINES);

	glVertex2f(xMeterToFloat(intersectCP.x - parabolaVect.x * 1000), yMeterToFloat(intersectCP.y - parabolaVect.y * 1000));
	glVertex2f(xMeterToFloat(intersectCP.x + parabolaVect.x * 1000), yMeterToFloat(intersectCP.y + parabolaVect.y * 1000));

	glEnd();

	glBegin(GL_LINES);

	glVertex2f(xMeterToFloat(intersectCP.x - catmullVect.x * 10000000), yMeterToFloat(intersectCP.y - catmullVect.y * 10000000));
	glVertex2f(xMeterToFloat(intersectCP.x + catmullVect.x * 10000000), yMeterToFloat(intersectCP.y + catmullVect.y * 10000000));

	glEnd();
}

Vector hermite(PointCenter p1, PointCenter p2, Vector v1, Vector v2, float t) {
	long t_i = p1.time;
	long t_ii = p2.time;

	Vector interPoint;
	
	float a3_x = (2 * (xMeterToFloat(p1.x) - xMeterToFloat(p2.x)) / (pow(t_ii - t_i, 3))) + ((v2.x + v1.x) / (pow(t_ii - t_i, 2)));
	float a3_y = (2 * (yMeterToFloat(p1.y) - yMeterToFloat(p2.y)) / (pow(t_ii - t_i, 3))) + ((v2.y + v1.y) / (pow(t_ii - t_i, 2)));
	float a2_x = (3 * (xMeterToFloat(p2.x) - xMeterToFloat(p1.x)) / (pow(t_ii - t_i, 2)) - ((v2.x + 2 * v1.x) / (t_ii - t_i)));
	float a2_y = (3 * (yMeterToFloat(p2.y) - yMeterToFloat(p1.y)) / (pow(t_ii - t_i, 2)) - ((v2.y + 2 * v1.y) / (t_ii - t_i)));
	float a1_x = v1.x;
	float a1_y = v1.y;
	float a0_x = xMeterToFloat(p1.x);
	float a0_y = yMeterToFloat(p1.y);

	interPoint.x = a3_x*(pow(t - t_i, 3)) + a2_x*(pow(t - t_i, 2)) + a1_x*(t - t_i) + a0_x;
	interPoint.y = a3_y*(pow(t - t_i, 3)) + a2_y*(pow(t - t_i, 2)) + a1_y*(t - t_i) + a0_y;

	return interPoint;
}

Vector catmullRom(float t) {
	Vector iPoint;
	for (int i = 0; i < points.getSize() - 1; ++i) {
		if (points.getNth(i)->time <= t &&  t <= points.getNth(i+1)->time) {
			PointCenter p1 = *points.getNth(i);
			PointCenter p2 = *p1.next;
			iPoint = hermite(p1, p2, speed(i), speed(i + 1), t);
			return iPoint;
		}
	}
	PointCenter p1 = *points.getLast();
	PointCenter p2 = *points.getFirst();
	iPoint = hermite(p1, p2, speed((points.getSize())-1), speed(0), t);
	return iPoint;
}

void drawCatmullRom() {

	if (points.getSize() <= 2) {
		return;
	}

	Vector intersection;
	long intersectTime = 0;

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);

	for (long i = points.getFirst()->time; i <= points.getLast()->time; ++i) {
		Vector v = catmullRom(i);

		if (intersectTime == 0) {
			for (int j = 0; j < parabolicVecNum; ++j) {
				Vector p;
				p.x = xMeterToFloat(vectors[j].x);
				p.y = yMeterToFloat(vectors[j].y);
				Vector l = p - v;
				if ((l.Length() < 0.005) && 2 < points.getSize()) {
					intersection.x = p.x;
					intersection.y = p.y;
					intersectTime = i;
				}
			}
		}

		glVertex2f(v.x, v.y);
		for (int j = 0; j < points.getSize(); ++j) {
			if (points.getNth(j)->time == i) {
				PointCenter * pc = points.getNth(j);
				glVertex2f(xMeterToFloat(pc->x), yMeterToFloat(pc->y));
			}
		}
	}


	glEnd();
	if (!animate) {
		calcDrawLines(intersection.x, intersection.y, intersectTime);
	}
}

void calculateParabolicCurve() {
	if (points.getSize() != 4 || calculated) {
		return;
	}

	Vector p1;
	p1.x = xMeterToFloat(points.getFirst()->x);
	p1.y = yMeterToFloat(points.getFirst()->y);
	Vector p2;
	p2.x = xMeterToFloat(points.getNth(1)->x);
	p2.y = yMeterToFloat(points.getNth(1)->y);
	Vector p3;
	p3.x = xMeterToFloat(points.getNth(2)->x);
	p3.y = yMeterToFloat(points.getNth(2)->y);

	Vector d = p1 - p2;

	float angle = atan(d.y / d.x) * 180 / M_PI;
	if (d.x < 0) {
		angle = 180 + angle;
	}


	vectorAngle = -1 * angle * M_PI / 180;
	Vector rotated = rotate(p3, vectorAngle);
	Vector rotatedD = rotate(p1, vectorAngle);

	float parameter = rotated.y - rotatedD.y;


	for (float x = -4; x <= 4; x = x + 0.01) {
		float y = (1 / (2 * parameter)*(x - rotated.x)*(x - rotated.x)) + rotatedD.y + parameter/2;
		Vector result(x, y, 0);
		Vector drawVect = rotate(result, -1 * vectorAngle);
		vectors[parabolicVecNum].x = floatToMeter(drawVect.x);
		vectors[parabolicVecNum].y = floatToMeter(drawVect.y);
		++parabolicVecNum;
	}

	calculated = true;
}

void drawParabolicCurve() {
	if (points.getSize() < 3) {
		return;
	}

	glColor3f(1, 1, 0);

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < parabolicVecNum; ++i) {
		glVertex2f(xMeterToFloat(vectors[i].x), yMeterToFloat(vectors[i].y));
	}

	glEnd();
}

void onInitialization( ) { 
	glViewport(0, 0, screenWidth, screenHeight);

}

void onDisplay( ) {
    glClearColor(0.011f, 0.756f, 0.756f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	calculateParabolicCurve();
	drawParabolicCurve();

	if (animate) {
		drawLines();
	}

	drawCatmullRom();
	
	PointCenter * pc = points.getFirst();
	while (pc != NULL) {
		drawCircle(xMeterToFloat(pc->x), yMeterToFloat(pc->y));
		pc = pc->next;
	}

    glutSwapBuffers();

}


void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 

	if (key == ' ') {
		animate = true;
		fieldHeight = 500;
		fieldWidth = 500;
		lastFrame = glutGet(GLUT_ELAPSED_TIME);
		glutPostRedisplay();
	}

}

void onKeyboardUp(unsigned char key, int x, int y) {

}

void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !animate) {
		
		float relx = xPixelToMeter(x);
		float rely = yPixelToMeter(y);
		points.add(relx, rely, glutGet(GLUT_ELAPSED_TIME));
		
		glutPostRedisplay();
	}
}

void onMouseMotion(int x, int y)
{

}

void onIdle( ) {
	if (animate) {
		long time = glutGet(GLUT_ELAPSED_TIME) - lastFrame;
		viewPos = viewPos + viewSpeed*time / 1000;
		if (250 <= viewPos.x || viewPos.x <= -250) {
			viewSpeed.x = -1 * viewSpeed.x;
		}
		if (250 <= viewPos.y || viewPos.y <= -250) {
			viewSpeed.y = -1 * viewSpeed.y;
		}
		glutPostRedisplay();
		lastFrame = glutGet(GLUT_ELAPSED_TIME);
	}
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel 
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse); 
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);

    glutMainLoop();					// Esemenykezelo hurok
    
    return 0;
}

