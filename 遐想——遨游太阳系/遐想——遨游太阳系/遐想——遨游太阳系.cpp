// 遐想——遨游太阳系.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"   //加载音乐所需的头文件
#include"mmsystem.h"   //加载音乐所需的头文件
#include <stdlib.h> 
#include <math.h> 
#include <GL/glew.h> 
#include <gl\glut.h>
#include <gl\glaux.h>   //加载纹理所需的头文件

// 键盘交互全局变量
const float piover180 = 0.0174532925f;//折算度和弧度的折算因子3.1415926/180
float heading;                        //头向左右偏转的角度
float xpos;//此处定义镜头在xoz平面上移动的x坐标和z坐标
float zpos;
GLfloat	yrotl;				// Y 旋转角度
GLfloat walkbias = 0;        //当人行走时头部产生上下摆动的幅度。我们使用简单的sin正弦波来调节镜头的Y轴位置。
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;    //定义上下看的旋转角度
GLfloat	z = 0.0f;				

//旋转全局变量
GLfloat corner1 = 0.0, corner2 = 0.0, corner3 = 0.0,
corner4 = 0.0, corner5 = 0.0, corner6 = 0.0, corner7 = 0.0,
corner8 = 0.0;

GLuint flag = 1,mark=1;

//鼠标交互全局变量
GLfloat xrot;    // 绕X轴旋转的角度
GLfloat yrot;    // 绕Y轴旋转的角度
int     LastXPos, LastYPos;   //上次鼠标点击处x、y坐标
BOOL    IsLBDown = FALSE;    //鼠标左键是否按下

//光照全局变量
GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //环境光
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //散射光
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };  //位置光

//纹理全局变量
GLuint texture[11];

//加载位图
AUX_RGBImageRec *LoadBMP(char *Filename)   
{
	FILE *File = NULL;         
	if (!Filename)          
	{
		return NULL;         
	}
	File = fopen(Filename, "r");     //只读文件  
	if (File)           
	{
		fclose(File);         
		return auxDIBImageLoad(Filename);    
	}
	return NULL;         
}

//加载位图图片，定义纹理，绑定纹理
int LoadGLTexture()									
{
	int Status = TRUE;					 //状态指示器				

	AUX_RGBImageRec *TextureImage[1];		  //创建纹理的存储空间			
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //多边形的显示方式,模式将适用于物体的所有面采用填充形式
	memset(TextureImage, 0, sizeof(void *)* 1); // 将指针设为NULL

	if (TextureImage[0] = LoadBMP("sun.bmp"))  //太阳纹理
	{
		glGenTextures(1, &texture[0]);  //创建纹理
		glBindTexture(GL_TEXTURE_2D, texture[0]); //使用来自位图数据生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 
			0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//缩小滤波
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大滤波
	}

	if (TextureImage[0] = LoadBMP("mercury.bmp")) //水星纹理
	{
		glGenTextures(1, &texture[1]);					
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("venus.bmp")) //金星纹理
	{
		glGenTextures(1, &texture[2]);					
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("earth.bmp"))  //地球纹理
	{
		glGenTextures(1, &texture[3]);				
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("mars.bmp")) //火星纹理
	{
		glGenTextures(1, &texture[4]);					
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (TextureImage[0] = LoadBMP("jupiter.bmp")) //木星纹理
	{
		glGenTextures(1, &texture[5]);				
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("saturn.bmp")) //土星纹理
	{
		glGenTextures(1, &texture[6]);					
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("sky.bmp")) //星空纹理（黑）
	{
		glGenTextures(1, &texture[7]);					
		glBindTexture(GL_TEXTURE_2D, texture[7]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("3.bmp")) //星空纹理1
	{
		glGenTextures(1, &texture[8]);					
		glBindTexture(GL_TEXTURE_2D, texture[8]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("4.bmp"))//星空纹理2
	{
		glGenTextures(1, &texture[9]);					
		glBindTexture(GL_TEXTURE_2D, texture[9]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0] = LoadBMP("6.bmp"))//星空纹理3
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
//调整画面大小
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
//初始化
int InitGL(GLvoid)         
{
	//加载纹理
	if (!LoadGLTexture())      
	{
		return FALSE;       
	}
	//启用纹理贴图
	glEnable(GL_TEXTURE_2D);       
	glShadeModel(GL_SMOOTH);      //用于控制opengl中绘制指定两点间其他点颜色的过渡模式 
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);   
	//深度检测
	glClearDepth(1.0f);         
	glEnable(GL_DEPTH_TEST);     
	glDepthFunc(GL_LEQUAL);        //如果目标像素z值<＝当前像素z值，则绘制目标像素
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   //指定颜色和纹理坐标的差值质量，选择最高质量选项
	//启用光源
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); 
	glEnable(GL_LIGHT1);
	return TRUE;          
}
//控制物体前后移动
void carama()
{
	GLfloat xtrans = -xpos;               // 用于游戏者沿X轴平移时的大小
	GLfloat ztrans = -zpos;               // 用于游戏者沿Z轴平移时的大小
	GLfloat ytrans = -walkbias - 0.25f;    // 用于头部的上下摆动
	GLfloat sceneroty = 360.0 - yrotl;   // 使得左右键与旋转方向一致
	/*此处漫游并未采用移动视点的方式来实现。而是通过旋转、移动图形对象来产生漫游的错觉
	*根据用户的指令旋转并变换镜头位置。
	*围绕原点，以与镜头相反的旋转方向来旋转世界。(让人产生镜头旋转的错觉)
	*以与镜头平移方式相反的方式来平移世界(让人产生镜头移动的错觉)。
	*/
	glRotatef(lookupdown, 1.0f, 0.0f, 0.0f);       //相应PageUp、PageDown进行向上/向下看的旋转,沿x轴旋转
	glRotatef(sceneroty, 0.0f, 1.0f, 0.0f);        // 根据游戏者正面所对方向所作的左右旋转
	glTranslatef(xtrans, ytrans, ztrans);       //以游戏者为中心的平移场景

}
//绘制行星
void draw()
{
	GLUquadricObj* qobj;  //定义曲面指针
	qobj = gluNewQuadric(); //建立一个二次曲面物体
	glPushMatrix();//太阳
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//二次曲面纹理函数
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  //自传
	glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(-corner1, 0.0, -0.2, 0.5);
	gluSphere(qobj, 30.0f, 50.0f, 50.0f);//二次曲面 qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//水星
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef(corner1, 0.0, 1, 0);  //公转
	glTranslatef(38.0f, 0.0f, 0.0f);
	gluSphere(qobj, 1.0f, 50.0f, 50.0f);//二次曲面 qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//金星
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef(corner2, 0.0, 1, 0);
	glTranslatef(72.0f, 0.0f, 0.0f);
	gluSphere(qobj, 8.6f, 50.0f, 50.0f);//二次曲面 qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//地球
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef(corner3, 0.0, 1.0, 0.0);
	glTranslatef(100.0f, 0.0f, 0.0f);
	gluSphere(qobj, 10.0f, 50.0f, 50.0f);//二次曲面 qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//火星
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef(corner4, 0.0, 1, 0);
	glTranslatef(150.0f, 0.0f, 0.0f);
	gluSphere(qobj, 3.0f, 50.0f, 50.0f);//二次曲面 qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//木星
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef(corner5, 0.0, 1, 0);
	glTranslatef(320.0f, 0.0f, 0.0f);
	gluSphere(qobj, 25.0f, 50.0f, 50.0f);//二次曲面 qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();//土星
	glTranslatef(0.0f, 0.0f, -250.0f);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef(corner6, 0.0, 1, 0);
	glTranslatef(450.0f, 0.0f, 0.0f);
	gluSphere(qobj, 20.0f, 50.0f, 50.0f);//二次曲面 qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
//绘制画面1
void Scence1()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -5.0f);     //将场景向里面平移5个单位便于鼠标交互
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);    //沿x轴旋转
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);    //沿y轴旋转
	float x = 800, y = 200, z = 400;

	//绘制6面体并分别进行纹理绑定
	// Front Face
	glEnable(GL_TEXTURE_2D);//启用二维纹理
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
//绘制画面2环境
void DrawGLScene(GLvoid)        
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glLoadIdentity();									// 重置模型视图矩阵
	carama();

	glDisable(GL_LIGHTING);
	glPushMatrix();               //对轨道进行旋转
	glTranslatef(0.0f, 0.0f, -250.0f);
	glRotatef(corner8, 0.5f, 0.0f, 0.0f);
	//glTranslatef(0.0f, 3.0f, 0.0f);
	draw();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);     //将场景向里面平移5个单位
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);    //沿x轴旋转
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);    //沿y轴旋转

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);   //星空
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_TRUE);//纹理函数
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  //对场景进行慢速旋转
	glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(-corner7, 0.0, -0.2, 0.5);
	gluSphere(qobj, 950.0f, 100.0f, 100.0f);//二次曲面 qobj
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glutSwapBuffers();
}
//对特殊键盘的交互响应处理
void specialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:      //按下PageUp，向上看
		z -= 100.0f;
		lookupdown -= 10.0f;
		break;
	case GLUT_KEY_PAGE_DOWN:        //按下PageDown，向下看
		z += 100.0f;
		lookupdown += 10.0f;
		break;
	case GLUT_KEY_UP:
		//计算镜头在x和z轴方向上的移动量，heading为头的偏移方向与z轴夹角
		xpos -= (float)sin(heading*piover180) * 5.0f;// 沿游戏者所在的X平面移动
		zpos -= (float)cos(heading*piover180) * 5.0f;// 沿游戏者所在的Z平面移动
		if (walkbiasangle >= 359.0f)         // 如果walkbiasangle大于359度
		{
			walkbiasangle = 0.0f;           // 将 walkbiasangle 设为0
		}
		else
		{
			walkbiasangle += 10;           // 如果 walkbiasangle < 359 ，则增加 10
		}
		// 使游戏者产生跳跃感
		walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
		break;
	case GLUT_KEY_DOWN:
		xpos += (float)sin(heading*piover180) * 5.0f;// 沿游戏者所在的X平面移动
		zpos += (float)cos(heading*piover180) * 5.0f;// 沿游戏者所在的Z平面移动
		if (walkbiasangle <= 1.0f)          // 如果walkbiasangle小于1度
		{
			walkbiasangle = 359.0f;           // 使 walkbiasangle 等于 359
		}
		else
		{
			walkbiasangle -= 10;       // 如果 walkbiasangle > 1 减去 10
		}
		// 使游戏者产生跳跃感
		walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
		break;
	case GLUT_KEY_LEFT:  //控制轨道旋转
		corner8 += 1;
		break;
	case GLUT_KEY_RIGHT:  //控制轨道旋转
		corner8 -= 1;
		break;
	}

}
//控制行星公转
void moving()
{
	if (flag == 1)
	{
		corner1 += 0.47f;  //水星
		corner2 += 0.35f;  //金星
		corner3 += 0.30f;  //地球
		corner4 += 0.24f;  //火星
		corner5 += 0.13f;  //木星
		corner6 += 0.09f;  //土星
		corner7 += 0.01f;  //星系
		glutPostRedisplay();
	}
}
//控制音乐播放
void music()
{

	MCI_OPEN_PARMS op; 
	op.dwCallback = NULL;
	op.lpstrAlias = NULL;
	op.lpstrDeviceType = "MPEGAudio";
	op.lpstrElementName = "14.mp3";
	op.wDeviceID = NULL;
	UINT rs;
	rs = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&op); //打开设备
	if (rs == 0)
	{
		MCI_PLAY_PARMS pp;
		pp.dwCallback = NULL;
		pp.dwFrom = 0;
		mciSendCommand(op.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&pp); //播放设备
	}
}
//对普通键盘的交互响应处理
void inputKey(unsigned char key, int x, int y)
{
	if (key == 65)                 //A键,进行光照切换,开灯
	{
	
		    glEnable(GL_LIGHT1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, LightAmbient);//材料对环境光的反射情况
		
	}
	if (key == 68)                 //D键,进行光照切换，关灯
	{
		glDisable(GL_LIGHT1);		
	}
	if (key == 32)                 //空格键,进行场景切换
	{
		glutIdleFunc(DrawGLScene);
		glutDisplayFunc(DrawGLScene);
		glutIdleFunc(moving);
	}
	if (key == 66)                 //B键,进行公转速度加快
	{
		if (flag == 1)
		{
			corner1 += 0.47f;  //水星
			corner2 += 0.35f;  //金星
			corner3 += 0.30f;  //地球
			corner4 += 0.24f;  //火星
			corner5 += 0.13f;  //木星
			corner6 += 0.09f;  //土星
			corner7 += 0.01f;  //星系
			glutPostRedisplay();
		}
	}
}
//定义鼠标按键响应
void processMouse(int button, int state, int x, int y)
{   
	//鼠标左键按下        
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		int xPos, yPos;
		xPos = x;
		yPos = y;
		LastXPos = xPos;
		LastYPos = yPos;
		IsLBDown = TRUE;
	}
	//鼠标左键弹起
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		IsLBDown = FALSE;
	}

}
//处理鼠标拖动事件
void processMouseActiveMotion(int x, int y)
{
	if (IsLBDown)       //若鼠标左键被按下
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
	glutCreateWindow("遐想—遨游太阳系");
	InitGL();
	glutIdleFunc(Scence1);             //绘制第一个场景
	glutDisplayFunc(Scence1);
	glutReshapeFunc(ReSizeGLScene);
	music();                         //播放音乐
	glutKeyboardFunc(inputKey);      //响应普通键事件处理回调
	glutSpecialFunc(specialKey);     //响应特殊键事件处理回调
	glutMouseFunc(processMouse);     //响应鼠标左键事件处理回调
	glutMotionFunc(processMouseActiveMotion);

	glutMainLoop();
}
