#include <FL/gl.h>
#include "modelerglobals.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "CartonModel.h"
#include "LSystem.h"
#include "CartonLSystem.h"
#include <vector>

#define LSYSTEM_COLOR 0.68f, 1.0f, 0.18f

extern std::vector<LSystem*>* CartonLSystems;


void CartonSetupLights()
{
	// change the light
	static GLfloat light0pos[4];
	static GLfloat light0diff[4];
	static GLfloat light1pos[4];
	static GLfloat light1diff[4];
	light0pos[0] = VAL(LIGHT0_X); light0pos[1] = VAL(LIGHT0_Y); light0pos[2] = VAL(LIGHT0_Z); light0pos[3] = 0;
	light1pos[0] = VAL(LIGHT1_X); light1pos[1] = VAL(LIGHT1_Y); light1pos[2] = VAL(LIGHT1_Z); light1pos[3] = 0;
	light0diff[0] = light0diff[1] = light0diff[2] = light0diff[3] = VAL(LIGHT0_DIFFUSE);
	light1diff[0] = light1diff[1] = light1diff[2] = light1diff[3] = VAL(LIGHT1_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diff);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1diff);

	// draw the light sources
	if (VAL(DRAW_LIGHT) > 0)
	{
		setDiffuseColor(1.0, 1.0, 1.0);
		glPushMatrix();
		{
			glTranslated(VAL(LIGHT0_X) - 0.1, VAL(LIGHT0_Y) - 0.1, VAL(LIGHT0_Z) - 0.1);
			drawBox(0.2, 0.2, 0.2);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslated(VAL(LIGHT1_X) - 0.1, VAL(LIGHT1_Y) - 0.1, VAL(LIGHT1_Z) - 0.1);
			drawBox(0.2, 0.2, 0.2);
		}
		glPopMatrix();
	}
}


	
void drawTorus(double posX, double posY, double posZ, double innerR, double outerR, int numc, int numt)
{
	glPushMatrix();
	glTranslated(posX, posY, posZ);
	int i, j, k;
	double s, t, x, y, z, twopi;

	twopi = 2 * (double)M_PI;
	for (i = 0; i < numc; i++) {
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= numt; j++) {
			for (k = 1; k >= 0; k--) {
				s = (i + k) % numc + 0.5;
				t = j % numt;

				x = (outerR + innerR*cos(s*twopi / numc))*cos(t*twopi / numt);
				y = (outerR + innerR*cos(s*twopi / numc))*sin(t*twopi / numt);
				z = innerR * sin(s * twopi / numc);
				glNormal3d((innerR*cos(s*twopi / numc))*cos(t*twopi / numt), (innerR*cos(s*twopi / numc))*sin(t*twopi / numt), innerR * sin(s * twopi / numc));
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
	glPopMatrix();
}


void CartonDrawLSystems()
{
	setDiffuseColor(LSYSTEM_COLOR);
	int lsysSel = 1;
	if (lsysSel >= 0 && lsysSel < CartonLSystems->size())
	{
		glPushMatrix();
		{
			glTranslated(-2, 0, 1);
			glRotated(90, -1, 0, 0);
			glRotated(90, 0, -1, 0);
			LSystem* sys = (*CartonLSystems)[lsysSel];
			sys->runIteration(VAL(DRAW_LSYSTEM));
			for (int opi : *(sys->ops))
			{
				if (sys->opMap.find(opi) != sys->opMap.end())
				{
					auto op = sys->opMap[opi];
					switch (op.first)
					{
					case CT_FORWARD:
						if (op.second <= 0)
							break;
						drawBox(0.05 * op.second, 0.02, 0.02);
						glTranslated(0.05 * op.second, 0, 0);
						break;
					case CT_ROTATE_LEFT:
						glRotated(-op.second, 0, 1, 0);
						break;
					case CT_ROTATE_RIGHT:
						glRotated(op.second, 0, 1, 0);
						break;
					case CT_ROTATE_LEFT_AND_PUSH:
						glRotated(-op.second, 0, 1, 0);
						glPushMatrix();
						break;
					case CT_POP_AND_ROTATE_RIGHT:
						glPopMatrix();
						glRotated(op.second, 0, 1, 0);
						break;
					default:
						break;
					}
				}
			}
		}
		glPopMatrix();
	}
}






