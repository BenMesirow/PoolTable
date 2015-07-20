#define NUM_OPENGL_LIGHTS 8

#include <iostream>
#include <fstream>
#include <string>
#include <GLUI/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"
#include <chrono>
#include "pool.h"
#include <mutex>

using namespace std;

chrono::high_resolution_clock::time_point t_prior;

int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 45;
float eyeX = 0;
float eyeY = 10;
float eyeZ = 0;
float lookX = 0;
float lookY = -1;
float lookZ = 0;

float Force_X = 0;
float Force_Z = 0;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "xml/pool.xml";
GLUI_EditText* filenameTextField = NULL;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
Shape* shape = NULL;
SceneParser* parser = NULL;
Camera* camera = new Camera();
//Puck* puck = new Puck();

SceneNode *root = NULL;

PoolGame *pool = new PoolGame();

void setupCamera();

void callback_load(int id) {
	char curDirName [2048];
	if (filenameTextField == NULL) {
		return;
	}

    t_prior = chrono::high_resolution_clock::now();

	printf ("%s\n", filenameTextField->get_text());

	if (parser != NULL) {
		delete parser;
	}
	parser = new SceneParser (filenamePath);
	cout << "success? " << parser->parse() << endl;

    root = parser->getRootNode();

	setupCamera();
}

void renderShape (int shapeType) {
	switch (shapeType) {
	case SHAPE_CUBE:
		shape = cube;
		break;
	case SHAPE_CYLINDER:
		shape = cylinder;
		break;
	case SHAPE_CONE:
		shape = cone;
		break;
	case SHAPE_SPHERE:
		shape = sphere;
		break;
	default:
		shape = sphere;
	}
	shape->draw();
}

/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	camera->SetScreenSize(x, y);

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();  //note that this is a new function. Be sure to set values for near and far plane!
	camera->SetViewAngle(cameraData.heightAngle);
	if (cameraData.isDir == true) {
		camera->Orient(cameraData.pos, cameraData.look, cameraData.up);
	}
	else {
		camera->Orient(cameraData.pos, cameraData.lookAt, cameraData.up);
	}

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}



/***************************************** setLight() *****************/
void setLight(const SceneLightData &light)
{
    // Zero ambient light because the ambient light for the scene is
    // set with GL_LIGHT_MODEL_AMBIENT
    float zero[] = { 0, 0, 0, 0 };

    // There are 8 lights in OpenGL, GL_LIGHT0 to GL_LIGHT7, and
    // each GL_LIGHT* is one greater than the previous one.  This
    // means the nth light is GL_LIGHT0 + n.
    int id = GL_LIGHT0 + light.id;

    glLightfv(id, GL_AMBIENT, zero);
    glLightfv(id, GL_DIFFUSE, &light.color.r);
    glLightfv(id, GL_SPECULAR, &light.color.r);
    glLightf(id, GL_CONSTANT_ATTENUATION, light.function[0]);
    glLightf(id, GL_LINEAR_ATTENUATION, light.function[1]);
    glLightf(id, GL_QUADRATIC_ATTENUATION, light.function[2]);

    if (glIsEnabled(id))
        cout << "warning: GL_LIGHT" << light.id << " enabled more than once, not supposed to happen" << endl;

    switch (light.type)
    {
        case LIGHT_POINT:
        {
            // Convert from double[] to float[] and make sure the w coordinate is correct 
            float position[] = { light.pos[0], light.pos[1], light.pos[2], light.pos[3] };
            glLightfv(id, GL_POSITION, position);
			glEnable(id);
            break;
        }

        case LIGHT_DIRECTIONAL:
        {
            // Convert from double[] to float[] and make sure the direction vector is normalized (it isn't for a lot of scene files)
            Vector direction = -light.dir;
			direction.normalize();
            float position[] = { direction[0], direction[1], direction[2], 0 };
            glLightfv(id, GL_POSITION, position);
			glEnable(id);
            break;
        }

        case LIGHT_SPOT:
        {
            // Convert from double[] to float[] and make sure the direction vector is normalized (it isn't for a lot of scene files)
            Vector direction = -light.dir;
			direction.normalize();
            float position[] = { direction[0], direction[1], direction[2], direction[3] };
            float direction3[] = { direction[0], direction[1], direction[2]};
            glLightfv(id, GL_POSITION, position);
            glLightfv(id, GL_SPOT_DIRECTION, direction3);
            glLightf(id, GL_SPOT_CUTOFF, light.angle);
            glLightf(id, GL_SPOT_EXPONENT, (light.radius / light.penumbra) * 128);
			glEnable(id);
            break;
        }

        case LIGHT_AREA:
        {
            // Convert from double[] to float[] and make sure the direction vector is normalized (it isn't for a lot of scene files)
            Vector direction = -light.dir;
			direction.normalize();
            float position[] = { direction[0], direction[1], direction[2], direction[3] };
            glLightfv(id, GL_POSITION, position);
			glEnable(id);
            break;
        }


        default:
        {
        	printf("Light type cannot be applied!\n");
        	break;       	
        }
    }
}


/***************************************** applyMaterial() *****************/
void applyMaterial(const SceneMaterial &material)
{
	SceneGlobalData globalData;
	parser->getGlobalData(globalData);

	SceneMaterial material_local = material;
	material_local.cAmbient.r *= globalData.ka;
	material_local.cAmbient.g *= globalData.ka;
	material_local.cAmbient.b *= globalData.ka;
	material_local.cDiffuse.r *= globalData.kd;
	material_local.cDiffuse.g *= globalData.kd;
	material_local.cDiffuse.b *= globalData.kd;
	material_local.cSpecular.r *= globalData.ks;
	material_local.cSpecular.g *= globalData.ks;
	material_local.cSpecular.b *= globalData.ks;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_local.cAmbient.channels);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_local.cDiffuse.channels);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_local.cEmissive.channels);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_local.cSpecular.channels);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_local.shininess);
}

void drawSceneRecursive(SceneNode *node) {

    if (node != NULL) {
	glPushMatrix();
        double *t, *s, *r;
		for (int i = 0; i < node->transformations.size(); i++) {
			switch(node->transformations[i]->type) {
				case TRANSFORMATION_TRANSLATE:
                    t = node->transformations[i]->translate.unpack();
					glTranslated(t[0],t[1],t[2]);
                    break;
				case TRANSFORMATION_SCALE:
                    s = node->transformations[i]->scale.unpack();
					glScaled(s[0],s[1],s[2]);
                    break;
				case TRANSFORMATION_ROTATE:
                    r = node->transformations[i]->rotate.unpack();
					glRotated(node->transformations[i]->angle, r[0], r[1], r[2]);
                    break;
				case TRANSFORMATION_MATRIX:
					glMultMatrixd(node->transformations[i]->matrix.unpack());
                    break;
				default:
					break;
			}
		}

		for (int i = 0; i < node->primitives.size(); ++i) {
			applyMaterial(node->primitives[i]->material);
			renderShape(node->primitives[i]->type);
		}

	    for (int i = 0; i < node->children.size(); ++i) {
            drawSceneRecursive(node->children[i]);
	    }
    glPopMatrix();
    }
}

void drawTable(void) {
    drawSceneRecursive(root);
}

void drawBalls() {

    chrono::high_resolution_clock::time_point t_now = chrono::high_resolution_clock::now();

    double ms = chrono::duration_cast<chrono::milliseconds>(t_now-t_prior).count();


   for (int i = 0; i < pool->balls.size(); ++i) {
        if (!pool->balls[i].isInHole()) {
            glPushMatrix();
            pool->updateMotion(ms/1000);
            GLdouble *p = pool->balls[i].position.unpack();
            glTranslated(p[0], p[1], p[2]);
            applyMaterial(pool->balls[i].material);
            renderShape(SHAPE_SPHERE);
            glPopMatrix();
        }
    }
    t_prior = chrono::high_resolution_clock::now();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	camera->SetViewAngle(viewAngle);
	glMatrixMode(GL_PROJECTION);
	Matrix projection = camera->GetProjectionMatrix();
	glLoadMatrixd(projection.unpack());

	camera->Orient(Point(eyeX, eyeY, eyeZ), Vector(lookX, lookY, lookZ), Vector(0, 1, 0));
	camera->RotateV(camRotV);
	camera->RotateU(camRotU);
	camera->RotateW(camRotW);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(camera->GetModelViewMatrix().unpack());

	//disable all the lights, and then enable each one...
	for (int i = 0; i < NUM_OPENGL_LIGHTS; i++) {
		glDisable(GL_LIGHT0 + i);
	}

	int numLights = parser->getNumLights();
	for (int i = 0; i < numLights; i++) {
		SceneLightData lightData;
		parser->getLightData(i, lightData);
		setLight(lightData);
	}

	glEnable(GL_LIGHTING);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    drawTable();
    drawBalls();

	glDisable(GL_LIGHTING);
	
	camera->RotateV(-camRotV);
	camera->RotateU(-camRotU);
	camera->RotateW(-camRotW);

	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
	if (parser != NULL) {
		delete parser;
	}

    delete pool;
}


void callback_strike(int id) {
    pool->strikeBall(SOLID_WHITE,Vector(Force_X, 0, Force_Z),0.1);
}
/*
void callback_reset(int id) {
    pool->reset();
}*/
/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(700, 500);

	main_window = glutCreateWindow("COMP 175 Final Assignment");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	glShadeModel (GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	// Specular reflections will be off without this, since OpenGL calculates
	// specular highlights using an infinitely far away camera by default, not
	// the actual location of the camera
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// Show all ambient light for the entire scene (not one by default)
	GLfloat one[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);

	glPolygonOffset(1, 1);



	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	
    GLUI_Panel *motion_panel = glui->add_panel("Motion");
    glui->add_button("Strike Cue Ball", 0, callback_strike);

    GLUI_Spinner* force_X = glui->add_spinner_to_panel(motion_panel, "Fx:",GLUI_SPINNER_FLOAT, &Force_X);
    force_X->set_float_limits(-10,10);
    GLUI_Spinner* force_Z = glui->add_spinner_to_panel(motion_panel, "Fz:",GLUI_SPINNER_FLOAT, &Force_Z);
    force_Z->set_float_limits(-10,10);

    glui->add_column_to_panel(motion_panel, true);
	glui->add_column(true);
	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);
	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}



