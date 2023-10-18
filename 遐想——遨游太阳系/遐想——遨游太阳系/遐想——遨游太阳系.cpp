// ���롪������̫��ϵ.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "windows.h"   //�������������ͷ�ļ�
#include"mmsystem.h"   //�������������ͷ�ļ�
#include <stdlib.h> 
#include <math.h> 
#include <GL/glew.h> 
#include <gl\glut.h>
#include <gl\glaux.h>   //�������������ͷ�ļ�

// ���̽���ȫ�ֱ���
const float piover180 = 0.0174532925f;//����Ⱥͻ��ȵ���������3.1415926/180
float heading;                        //ͷ������ƫת�ĽǶ�
float xpos;//�˴����徵ͷ��xozƽ�����ƶ���x�����z����
float zpos;
GLfloat	yrotl;				// Y ��ת�Ƕ�
GLfloat walkbias = 0;        //��������ʱͷ���������°ڶ��ķ��ȡ�����ʹ�ü򵥵�sin���Ҳ������ھ�ͷ��Y��λ�á�
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;    //�������¿�����ת�Ƕ�
GLfloat	z = 0.0f;				

//��תȫ�ֱ���
GLfloat corner1 = 0.0, corner2 = 0.0, corner3 = 0.0,
corner4 = 0.0, corner5 = 0.0, corner6 = 0.0, corner7 = 0.0,
corner8 = 0.0;

GLuint flag = 1,mark=1;

//��꽻��ȫ�ֱ���
GLfloat xrot;    // ��X����ת�ĽǶ�
GLfloat yrot;    // ��Y����ת�ĽǶ�
int     LastXPos, LastYPos;   //�ϴ��������x��y����
BOOL    IsLBDown = FALSE;    //�������Ƿ���

//����ȫ�ֱ���
GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //������
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //ɢ���
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };  //λ�ù�

//����ȫ�ֱ���
GLuint texture[11];

//����λͼ
AUX_RGBImageRec *LoadBMP(char *Filename)   
{
	FILE *File = NULL;         
	if (!Filename)          
	{
		return NULL;         
	}
	File = fopen(Filename, "r");     //ֻ���ļ�  
	if (File)           
	{
		fclose(File);         
		return auxDIBImageLoad(Filename);    
	}
	return NULL;         
}

//����λͼͼƬ����������������
int LoadGLTexture()									
{
	int Status = TRUE;					 //״ָ̬ʾ��				

	AUX_RGBImageRec *TextureImage[1];		  //��������Ĵ洢�ռ�			
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //����ε���ʾ��ʽ,ģʽ���������������������������ʽ
	memset(TextureImage, 0, sizeof(void *)* 1); // ��ָ����ΪNULL

	if (TextureImage[0] = LoadBMP("sun.bmp"))  //̫������
	{
		glGenTextures(1, &texture[0]);  //��������
		glBindTexture(GL_TEXTURE_2D, texture[0]); //ʹ������λͼ������������
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 
			0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//��С�˲�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�Ŵ��˲�
	}

	if (TextureImage[0] = LoadBMP("mercury.bmp")) //ˮ������
	{
		glGenTextures(1, &texture[1]);					
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("venus.bmp")) //��������
	{
		glGenTextures(1, &texture[2]);					
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("earth.bmp"))  //��������
	{
		glGenTextures(1, &texture[3]);				
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("mars.bmp")) //��������
	{
		glGenTextures(1, &texture[4]);					
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (TextureImage[0] = LoadBMP("jupiter.bmp")) //ľ������
	{
		glGenTextures(1, &texture[5]);				
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("saturn.bmp")) //��������
	{
		glGenTextures(1, &texture[6]);					
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("sky.bmp")) //�ǿ������ڣ�
	{
		glGenTextures(1, &texture[7]);					
		glBindTexture(GL_TEXTURE_2D, texture[7]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("3.bmp")) //�ǿ�����1
	{
		glGenTextures(1, &texture[8]);					
		glBindTexture(GL_TEXTURE_2D, texture[8]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("4.bmp"))//�ǿ�����2
	{
		glGenTextures(1, &texture[9]);					
		glBindTexture(GL_TEXTURE_2D, texture[9]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("6.bmp"))//�ǿ�����3
	{
		glGenTextures(1, &texture[10]);					
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (TextureImage[0])									
	{
		if (TextureImage[0]->data)							
		{
			free(TextureImage[0]->data);					
		}

		free(TextureImage[0]);								
	}

	return Status;										
}
//���������С
GLvoid ReSizeGLScene(GLsizei width, GLsizei height) 
{
	if (height == 0)          
	{
		height = 1;        
	}
	glViewport(0, 0, width, height);     
	glMatrixMode(GL_PROJECTION);      
	glLoadIdentity();        
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);      
	glLoadIdentity();        
}
//��ʼ��
int InitGL(GLvoid)         
{
	//��������
	if (!LoadGLTexture())      
	{
		return FALSE;       
	}
	//����������ͼ
	glEnable(GL_TEXTURE_2D);       
	glShadeModel(GL_SMOOTH);      //���ڿ���opengl�л���ָ���������������ɫ�Ĺ���ģʽ 
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);   
	//��ȼ��
	glClearDepth(1.0f);         
	glEnable(GL_DEPTH_TEST);     
	glDepthFunc(GL_LEQUAL);        //���Ŀ������zֵ<����ǰ����zֵ�������Ŀ������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   //ָ����ɫ����������Ĳ�ֵ������ѡ���������ѡ��
	//���ù�Դ
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); 
	glEnable(GL_LIGHT1);
	return TRUE;          
}
//��������ǰ���ƶ�
void carama()
{
	GLfloat xtrans = -xpos;               // ������Ϸ����X��ƽ��ʱ�Ĵ�С
	GLfloat ztrans = -zpos;               // ������Ϸ����Z��ƽ��ʱ�Ĵ�С
	GLfloat ytrans = -walkbias - 0.25f;    // ����ͷ�������°ڶ�
	GLfloat sceneroty = 360.0 - yrotl;   // ʹ�����Ҽ�����ת����һ��
	/*�˴����β�δ�����ƶ��ӵ�ķ�ʽ��ʵ�֡�����ͨ����ת���ƶ�ͼ�ζ������������εĴ��
	*�����û���ָ����ת���任��ͷλ�á�
	*Χ��ԭ�㣬���뾵ͷ�෴����ת��������ת���硣(���˲�����ͷ��ת�Ĵ��)
	*���뾵ͷƽ�Ʒ�ʽ�෴�ķ�ʽ��ƽ������(���˲�����ͷ�ƶ��Ĵ��)��
	*/
	glRotatef(lookupdown, 1.0f, 0.0f, 0.0f);       //��ӦPageUp��PageDown��������/���¿�����ת,��x����ת
	glRotatef(sceneroty, 0.0f, 1.0f, 0.0f);        // ������Ϸ���������Է���������������ת
	glTranslatef(xtrans, ytrans, ztrans);       //����Ϸ��Ϊ���ĵ�ƽ�Ƴ���

}
//��������
void draw()
{
	GLUquadricObj* qobj;  //��������ָ��
	qobj = gluNewQuadric(); //����һ��������������
	glPushMatrix();//̫��
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//��������������
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  //�Դ�
	glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(-corner1, 0.0, -0.2, 0.5);
	gluSphere(qobj, 30.0f, 50.0f, 50.0f);//�������� qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//ˮ��
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef(corner1, 0.0, 1, 0);  //��ת
	glTranslatef(38.0f, 0.0f, 0.0f);
	gluSphere(qobj, 1.0f, 50.0f, 50.0f);//�������� qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//����
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef(corner2, 0.0, 1, 0);
	glTranslatef(72.0f, 0.0f, 0.0f);
	gluSphere(qobj, 8.6f, 50.0f, 50.0f);//�������� qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//����
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef(corner3, 0.0, 1.0, 0.0);
	glTranslatef(100.0f, 0.0f, 0.0f);
	gluSphere(qobj, 10.0f, 50.0f, 50.0f);//�������� qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//����
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef(corner4, 0.0, 1, 0);
	glTranslatef(150.0f, 0.0f, 0.0f);
	gluSphere(qobj, 3.0f, 50.0f, 50.0f);//�������� qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//ľ��
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef(corner5, 0.0, 1, 0);
	glTranslatef(320.0f, 0.0f, 0.0f);
	gluSphere(qobj, 25.0f, 50.0f, 50.0f);//�������� qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//����
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef(corner6, 0.0, 1, 0);
	glTranslatef(450.0f, 0.0f, 0.0f);
	gluSphere(qobj, 20.0f, 50.0f, 50.0f);//�������� qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
//���ƻ���1
void Scence1()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -5.0f);     //������������ƽ��5����λ������꽻��
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);    //��x����ת
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);    //��y����ת
	float x = 800, y = 200, z = 400;

	//����6���岢�ֱ���������
	// Front Face
	glEnable(GL_TEXTURE_2D);//���ö�ά����
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, -y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-x, y, z);
	glEnd();

	// Back Face
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, y, -z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, -z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, -y, -z);
	glEnd();

	// Top Face
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-x, y, -z);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, -z);
	glEnd();

	// Bottom Face
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(x, -y, -z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, -y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, z);
	glEnd();

	// Right face
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(x, -y, -z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, -z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, -y, z);
	glEnd();

	// Left Face
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, y, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-x, y, -z);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();


}
//���ƻ���2����
void DrawGLScene(GLvoid)        
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glLoadIdentity();									// ����ģ����ͼ����
	carama();

	glDisable(GL_LIGHTING);
	glPushMatrix();               //�Թ��������ת
	glTranslatef(0.0f, 0.0f, -250.0f);
	glRotatef(corner8, 0.5f, 0.0f, 0.0f);
	//glTranslatef(0.0f, 3.0f, 0.0f);
	draw();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);     //������������ƽ��5����λ
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);    //��x����ת
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);    //��y����ת

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);   //�ǿ�
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//������
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  //�Գ�������������ת
	glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(-corner7, 0.0, -0.2, 0.5);
	gluSphere(qobj, 950.0f, 100.0f, 100.0f);//�������� qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glutSwapBuffers();
}
//��������̵Ľ�����Ӧ����
void specialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:      //����PageUp�����Ͽ�
		z -= 100.0f;
		lookupdown -= 10.0f;
		break;
	case GLUT_KEY_PAGE_DOWN:        //����PageDown�����¿�
		z += 100.0f;
		lookupdown += 10.0f;
		break;
	case GLUT_KEY_UP:
		//���㾵ͷ��x��z�᷽���ϵ��ƶ�����headingΪͷ��ƫ�Ʒ�����z��н�
		xpos -= (float)sin(heading*piover180) * 5.0f;// ����Ϸ�����ڵ�Xƽ���ƶ�
		zpos -= (float)cos(heading*piover180) * 5.0f;// ����Ϸ�����ڵ�Zƽ���ƶ�
		if (walkbiasangle >= 359.0f)         // ���walkbiasangle����359��
		{
			walkbiasangle = 0.0f;           // �� walkbiasangle ��Ϊ0
		}
		else
		{
			walkbiasangle += 10;           // ��� walkbiasangle < 359 �������� 10
		}
		// ʹ��Ϸ�߲�����Ծ��
		walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
		break;
	case GLUT_KEY_DOWN:
		xpos += (float)sin(heading*piover180) * 5.0f;// ����Ϸ�����ڵ�Xƽ���ƶ�
		zpos += (float)cos(heading*piover180) * 5.0f;// ����Ϸ�����ڵ�Zƽ���ƶ�
		if (walkbiasangle <= 1.0f)          // ���walkbiasangleС��1��
		{
			walkbiasangle = 359.0f;           // ʹ walkbiasangle ���� 359
		}
		else
		{
			walkbiasangle -= 10;       // ��� walkbiasangle > 1 ��ȥ 10
		}
		// ʹ��Ϸ�߲�����Ծ��
		walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
		break;
	case GLUT_KEY_LEFT:  //���ƹ����ת
		corner8 += 1;
		break;
	case GLUT_KEY_RIGHT:  //���ƹ����ת
		corner8 -= 1;
		break;
	}

}
//�������ǹ�ת
void moving()
{
	if (flag == 1)
	{
		corner1 += 0.47f;  //ˮ��
		corner2 += 0.35f;  //����
		corner3 += 0.30f;  //����
		corner4 += 0.24f;  //����
		corner5 += 0.13f;  //ľ��
		corner6 += 0.09f;  //����
		corner7 += 0.01f;  //��ϵ
		glutPostRedisplay();
	}
}
//�������ֲ���
void music()
{

	MCI_OPEN_PARMS op; 
	op.dwCallback = NULL;
	op.lpstrAlias = NULL;
	op.lpstrDeviceType = "MPEGAudio";
	op.lpstrElementName = "14.mp3";
	op.wDeviceID = NULL;
	UINT rs;
	rs = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&op); //���豸
	if (rs == 0)
	{
		MCI_PLAY_PARMS pp;
		pp.dwCallback = NULL;
		pp.dwFrom = 0;
		mciSendCommand(op.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&pp); //�����豸
	}
}
//����ͨ���̵Ľ�����Ӧ����
void inputKey(unsigned char key, int x, int y)
{
	if (key == 65)                 //A��,���й����л�,����
	{
	
		    glEnable(GL_LIGHT1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LightAmbient);//���϶Ի�����ķ������
		
	}
	if (key == 68)                 //D��,���й����л����ص�
	{
		glDisable(GL_LIGHT1);		
	}
	if (key == 32)                 //�ո��,���г����л�
	{
		glutIdleFunc(DrawGLScene);
		glutDisplayFunc(DrawGLScene);
		glutIdleFunc(moving);
	}
	if (key == 66)                 //B��,���й�ת�ٶȼӿ�
	{
		if (flag == 1)
		{
			corner1 += 0.47f;  //ˮ��
			corner2 += 0.35f;  //����
			corner3 += 0.30f;  //����
			corner4 += 0.24f;  //����
			corner5 += 0.13f;  //ľ��
			corner6 += 0.09f;  //����
			corner7 += 0.01f;  //��ϵ
			glutPostRedisplay();
		}
	}
}
//������갴����Ӧ
void processMouse(int button, int state, int x, int y)
{   
	//����������        
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		int xPos, yPos;
		xPos = x;
		yPos = y;
		LastXPos = xPos;
		LastYPos = yPos;
		IsLBDown = TRUE;
	}
	//����������
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		IsLBDown = FALSE;
	}

}
//��������϶��¼�
void processMouseActiveMotion(int x, int y)
{
	if (IsLBDown)       //��������������
	{
		int xPos, yPos;
		xPos = x;
		yPos = y;
		xrot += GLfloat(yPos - LastYPos) / GLfloat(5.0);
		yrot += GLfloat(xPos - LastXPos) / GLfloat(5.0);

		LastXPos = xPos;
		LastYPos = yPos;
	}

}
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("���롪����̫��ϵ");
	InitGL();
	glutIdleFunc(Scence1);             //���Ƶ�һ������
	glutDisplayFunc(Scence1);
	glutReshapeFunc(ReSizeGLScene);
	music();                         //��������
	glutKeyboardFunc(inputKey);      //��Ӧ��ͨ���¼�����ص�
	glutSpecialFunc(specialKey);     //��Ӧ������¼�����ص�
	glutMouseFunc(processMouse);     //��Ӧ�������¼�����ص�
	glutMotionFunc(processMouseActiveMotion);

	glutMainLoop();
}
