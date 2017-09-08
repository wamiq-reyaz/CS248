#include <stdio.h>	
#include <stdlib.h>
#include <time.h>			// for time()
#include <iostream>			// for std::cerr, std::cout
#include "GL/glew.h"
#include "GL/wglew.h"
#include "GL/glut.h"
#include "timing.h"			// handling of timers
#include "Program.h"		// shader wrapper
#include "model.h"			// obj file loader

#include "vec.h"			// Your implementation of 4D vectors
#include "mat.h"			// Your implementation of 4D matrices
#include "quat.h"			// Your implementation of quaternions

#include "arcball.h"		// Your implementation of the arcball
#include "camera.h"			// Your implementation of the camera

#include "GL/glu.h"

//============================================================================================
//   VARIABLE DECLARATION
//============================================================================================
struct state_t {
	state_t(void);
	std::string		app_name;			// application name
	CTimer			fps_timer;			// fps timer, don't touch
	int				frame_count;		// fps counter, don't touch
	CTimer			sys_timer;			// sys timer for application jobs
	int				win_width;			// window width in pixels
	int				win_height;			// window height in pixels
	camf			camera;				// ego-camera
	arcballf		arcball;			// arcball for object rotation
	vecf			background;			// background color
	float			mouse_dx;			// mouse movement
	float			mouse_dy;			// mouse movement
	double			mouse_btn_down_time;// mouse button down timestamp
	int				menu_handle;		// GLUT menu handle
	int				model_menu;			// GLUT model menu handle

	GLuint			hVAO;				// GL Vertex Array Object
	GLuint			hVertexBuffer;		// GL Vertex Buffer Object
	GLuint			hColorBuffer;		// GL Color Buffer Object
	GLuint			hIndexBuffer;		// GL Index Buffer Object
	GLuint			hNormalBuffer;		// GL Normal Buffer Object
	GLuint			hTexcoordBuffer;	// GL Texture Coordinate Buffer Object
	GLsizei			nTriangles;			// amount of triangles in model

	CProgram*		pShader;			// Shader program
} g_State;

state_t::state_t(void) {
	app_name = std::string("OpenGL Sample");
	frame_count = 0;
	sys_timer.Reset();
	fps_timer.Reset();
	win_width = 512;
	win_height = 512;
	camera.set_eye(vecf(0.0f, 0.0f, 6.0f));
	camera.set_up(vecf(0.0f, 1.0f, 0.0f));
	camera.set_center(vecf(0.0f, 0.0f, 0.0f));
	camera.set_FOV(45.0f);
	camera.set_nearplane(0.1f);
	camera.set_farplane(100.0f);
	background = vecf(0.2f, 0.2f, 0.4f, 0.0f);
	hVAO = 0;
	hVertexBuffer = 0;
	hColorBuffer = 0;
	hIndexBuffer = 0;
	hNormalBuffer = 0;
	hTexcoordBuffer = 0;
	nTriangles = 0;
	pShader = NULL;
}


//***** GLUT STATE *****
int				g_iLFlag		= 0;				// left mouse button pressed ?
int				g_iRFlag		= 0;				// right mouse button pressed ?
int				g_iMouseX		= 0;				// mouse X-position for interaction
int				g_iMouseY		= 0;				// mouse Y-position for interaction
bool			g_bShift		= false;			// shift button pressed ?


//***** LOGFILE *****
FILE *logfile=NULL;									// log file to record GPU caps etc.

//============================================================================================
//   AUXILIARY FUNCTIONS
//============================================================================================

/**
 * writes gfx-hardware description to logfile
 */
void logHardware(void) {
	if (!logfile) return;
	int arg;
	fprintf(logfile,"\nRendering Hardware:\n");
	fprintf(logfile,"    Renderer   : ");
	fprintf(logfile,(const char*)glGetString(GL_RENDERER));
	fprintf(logfile,"\n");
	fprintf(logfile,"    Vendor     : ");
	fprintf(logfile,(const char*)glGetString(GL_VENDOR));
	fprintf(logfile,"\n");
	fprintf(logfile,"    Version    : OpenGL ");	
	fprintf(logfile,(const char*)glGetString(GL_VERSION));
	fprintf(logfile,"\n");
	fprintf(logfile,"    Visual     : ");
	glGetIntegerv(GL_RED_BITS,&arg);
	fprintf(logfile,"R%i-",arg);
	glGetIntegerv(GL_GREEN_BITS,&arg);
	fprintf(logfile,"G%i-",arg);
	glGetIntegerv(GL_BLUE_BITS,&arg);
	fprintf(logfile,"B%i-",arg);
	glGetIntegerv(GL_ALPHA_BITS,&arg);
	fprintf(logfile,"A%i",arg);
	glGetIntegerv(GL_DEPTH_BITS,&arg);
	fprintf(logfile," Depth%i",arg);
	glGetIntegerv(GL_STENCIL_BITS,&arg);
	fprintf(logfile," Stencil%i\n",arg);
	fprintf(logfile,"    Extensions :\n        ");
	const char *ext;
	ext=(const char*)(glGetString(GL_EXTENSIONS));
	for (unsigned int i=0; i<strlen(ext); i++) {
		if (ext[i]==' ') fprintf(logfile,"\n        ");
		else fprintf(logfile,"%c",ext[i]);
	}
	fprintf(logfile,"\n");
}

/** 
 * Parse the Command line parameter(s)
 */
void ParseCommandLine(int *argc,char **argv) {	
	if (logfile) fprintf(logfile,"\nParseCommandLine:\n");

	// ------------------------------------------------------------------
	// \TODO: INSERT YOUR CODE HERE IF YOU WANT TO PARSE THE COMMAND LINE
	// ------------------------------------------------------------------

	if (logfile) fprintf(logfile,"\n");
}

/**
 * Display fps-count in the title bar.
 */
void UpdateTitle(float fps=0.0f) {
	char text[100];
	sprintf_s(text,100,"%s @ %.2f fps",g_State.app_name.c_str(),fps);
	glutSetWindowTitle(text);
}


/** This function creates a GL_ARRAY_BUFFER and uploads data to it.
 *  \param handle	: the OpenGL handle of the ARRAY_BUFFER
 *  \param id		: the id of the buffer
 *  \param data		: pointer to the data to be uploaded
 *  \param datasize : size of data in bytes
 *  \param data_type: type of the buffer, e.g., GL_FLOAT
 *  \param vectordim: dimensionality of the data, e.g., 3 for 3D vectors
 *  \returns		glError if any, GL_NO_ERROR otherwise
 */
GLint GLVertexArray(GLuint& handle, GLuint id, const void* data, size_t datasize, GLenum data_type, size_t vectordim) {
	glGetError();
	glDeleteBuffers(1, &handle);										// delete old buffer (if any)
	glGenBuffers(1, &handle);											// generate new buffer object
	glBindBuffer(GL_ARRAY_BUFFER, handle);								// bind buffer
	glBufferData(GL_ARRAY_BUFFER, datasize, data, GL_STATIC_DRAW);		// upload data to the GPU
	glVertexAttribPointer(id, vectordim, data_type, GL_FALSE, 0, NULL);	// assign vertex attribute id to the buffer and tell OpenGL its type and dimensionality
	glEnableVertexAttribArray(id);										// enable the use of this buffer by OpenGL
	return glGetError();
}

/** This function creates a GL_ELEMENT_ARRAY_BUFFER (index buffer) and uploads data to it.
 *  \param handle	: the OpenGL handle of the ELEMENT_ARRAY_BUFFER
 *	\param data		: pointer to the data to be uploaded
 *	\param datasize : size of data in bytes
 *  \returns		glError if any, GL_NO_ERROR otherwise
 */
GLint GLIndexBuffer(GLuint& handle, const void* data, size_t datasize) {
	glGetError();
	glDeleteBuffers(1, &handle);											// delete old buffer (if any)	
	glGenBuffers(1, &handle);												// creeate new Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);							// bind as Element Array Buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize, data, GL_STATIC_DRAW);	// upload the data to the GPU. We do not need to tell OpenGL to use this type of buffer, and there can be only one active at any time.	
	return glGetError();
}

/**
 * Generate a cube and return the vertex array object for the cube
 * This creates 3 buffers: A vertex buffer containing the corner points
 *                         A color buffer containing the colors at the corner points
 *                         An index buffer containing three vertex indices per triangle
 */
void generateCube(void) {
	// 8 corners of the cube as xyz, xyz, ...
	const float vertices[] = {
		-1.0f,-1.0f,-1.0f,	// vertex 0
		 1.0f,-1.0f,-1.0f,  // vertex 1
		-1.0f, 1.0f,-1.0f,  // vertex 2
		 1.0f, 1.0f,-1.0f,  // vertex 3
		-1.0f,-1.0f, 1.0f,  // vertex 4
		 1.0f,-1.0f, 1.0f,  // vertex 5
		-1.0f, 1.0f, 1.0f,  // vertex 6
		 1.0f, 1.0f, 1.0f   // vertex 7
	};
	// 8 colors for the corners of the cube as rgb, rgb, ...
	const float colors[] = {
		0.0f,0.0f,0.0f,	 // color at vertex 0
		1.0f,0.0f,0.0f,  // color at vertex 1
		0.0f,1.0f,0.0f,  // color at vertex 2
		1.0f,1.0f,0.0f,  // color at vertex 3
		0.0f,0.0f,1.0f,  // color at vertex 4
		1.0f,0.0f,1.0f,  // color at vertex 5
		0.0f,1.0f,1.0f,  // color at vertex 6
		1.0f,1.0f,1.0f   // color at vertex 7
	};
	// 12 quadratic faces of the cube, specified as 24 triangles
	// Each triangle has 3 indices into the vertex buffer
	const unsigned int faces[] = {
		4,5,7,		4,7,6,	// front:  Z=+1
		0,3,1,		0,2,3,	// back:   Z=-1
		4,0,2,		4,2,6,	// left:   X=-1
		1,5,7,		1,7,3,	// right:  X=+1
		2,3,7,		2,7,6,	// top:    Y=+1
		4,5,1,		4,1,0	// bottom: Y=-1
	};

	// create and bind the Vertex Array Object (VAO)
	glDeleteVertexArrays(1,&g_State.hVAO);
	glGenVertexArrays(1,&g_State.hVAO);
	glBindVertexArray(g_State.hVAO);
	
	// now this VAO will store all of the following state	
	if (GLVertexArray(g_State.hVertexBuffer, 0, vertices, sizeof(vertices), GL_FLOAT, 3) != GL_NO_ERROR) {	// the GPU buffer containing vertices	
		std::cout << "GL_ERROR loading VertexBuffer" << std::endl;
	}
	if (GLVertexArray(g_State.hColorBuffer, 1, colors, sizeof(colors), GL_FLOAT, 3) != GL_NO_ERROR) { // the GPU buffer containing colors
		std::cout << "GL_ERROR loading ColorBuffer" << std::endl;
	}
	if (GLIndexBuffer(g_State.hIndexBuffer, faces, sizeof(faces)) != GL_NO_ERROR) { // the index buffer (aka. Element Array) containing the face indices
		std::cout << "GL_ERROR loading IndexBuffer" << std::endl;
	}
	g_State.hNormalBuffer = 0;		// reserved for later
	g_State.hTexcoordBuffer = 0;	// reserved for later
	g_State.nTriangles = 12;
}


/** Sets the resources (buffers) for the shader. This information will "stick" to the shader's state.
 *  \param pShader	: pointer to shader object
 *  \returns		glError if any, GL_NO_ERROR otherwise
 */
GLuint setShaderBuffers(CProgram*& pShader) {
	glGetError();
	pShader->Enable();
	glBindAttribLocation((GLuint)(*pShader), 0, "position");
	glBindAttribLocation((GLuint)(*pShader), 1, "vertex_color");
	pShader->Disable();
	return glGetError();
}

/** Loads and compiles a shader from disk.
 *  \param pShader : pointer to shader object
 *	\returns		glError if any, GL_NO_ERROR otherwise
 */
GLuint generateShaders(CProgram*& pShader) {
	if (pShader) delete pShader;
	glGetError();
	pShader = new CProgram("vsTransform.txt", "fsColor.txt", TUM3D_DISK); // read vertex and fragment shader pair from files stored on disk
	// check whether this compiled ok
	if (!pShader->IsValid()) {
		printf("generateShader() -- Error compiling shader\n");
		pShader = NULL;
		exit(0);
	}
	// the shader has compiled and we have a valid object.
	// Now, tell the shader how to obtain input data
	GLuint err = setShaderBuffers(pShader);
	if (err != GL_NO_ERROR) return err;
	return glGetError();	
}

// Enumeration of the models available
enum model_t {
	MODEL_CUBE = 1,			///< Box.obj
	MODEL_SPHERE = 2,		///< Sphere.obj
	MODEL_TORUS = 3,		///< Torus.obj
	MODEL_CYLINDER = 4,		///< Cylinder.obj
	MODEL_BOUNDARY = 5,		///< Boundary.obj
};

// Load a model
bool loadModel(const std::string& name) {
	model M;										// Model
	M.load(name);									// load model from obj file
	if (!M.get_blurb().empty()) {					// if loading generated any messages, ...
		std::cout << M.get_blurb() << std::endl;		// ... output said messages
	}
	if (!M.is_ok()) {
		generateCube();								// generate a cube as fallback model
		return false;								// did not load properly
	}
	M.center();										// center the model around the origin
	M.rescale(2.0f);								// rescale to [-1,1]^3
	if (!M.has_colors())	M.generate_colors();		// generate colors if the model did not have them
	if (!M.has_normals())	M.generate_normals();	// generate normals if the model did not have them

	// TODO: P2.2 - 1.7 upload the model M to OpenGL (see generateCube for an example)


	return true;
}

/**
 * Application specific initialization.
 * Called by the framework.
 */
void initialize(void) {

	glGetError();						// essentially resets GL's error state

	if (logfile) fprintf(logfile,"\nInitialize:\n");
	
	// GLEW init
	if (GLEW_OK!=glewInit()) {
		printf("Error initializing GLEW\n");
		exit(255);
	}

	// OpenGL init
	glClearColor(g_State.background(0), g_State.background(1), g_State.background(2), g_State.background(3)); // background color for glClear
	glDepthFunc(GL_LEQUAL);		// depth test:   let frontmost pixels survive
	glClearDepth(1.0f);			// depth test:   clear depth buffer with farthest value (depth range 0.0..1.0)
	glEnable(GL_DEPTH_TEST);	// depth test:   enable depth testing
	glDisable(GL_CULL_FACE);	// face culling: disabled
	
	// reset the global timer
	g_State.sys_timer.Reset();

	// generate a cube residing on the GPU
	generateCube();

	// generate the shaders on the GPU (vertex & fragment shaders)
	generateShaders(g_State.pShader);

	if (glGetError()!=GL_NO_ERROR) {	// check for GL errors during initialization
		printf("initialize() -- GL Errors\n");
		exit(0);
	}

	if (logfile) fprintf(logfile,"\n");
} 

void cleanup(void) {
	printf("Cleanup\n");
	time_t ltime;
	if (logfile) {
		time( &ltime );
		fprintf(logfile,"Cleaning up ");
	}

	if (g_State.pShader) delete g_State.pShader;
	// GPU objects will be automatically released and deleted
	
	if (logfile) {
		fprintf(logfile,"done\n");
		fprintf(logfile,"Exiting program normally.\n");
		char tmp[512];
		errno_t error = ctime_s(tmp,512,&ltime);
		fprintf(logfile,"%s\n\n", (error ? "Error reading time." : tmp));
		fclose(logfile);
	}
}

/**
 * The displayfunc is called whenever a glutPostRedisplay() is issued.
 * In this framework, the idlefunc calls glutPostRedisplay() to ensure
 * continuous rendering.
 */
void displayfunc(void) {
	glGetError();

	// clear the canvas and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	float time = float(g_State.sys_timer.Query()); // time in seconds since the program start

	// TODO: P2.2 - 1.3 & 1.4  Replace below code by your own matrix code
	float ModelViewProjection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, ModelViewProjection);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(ModelViewProjection);
	glTranslatef(0.0f, 0.0f, -6.0f);
	// TODO: P2.2 - 1.3 Tilt the cube by about 30 degrees to the viewer
	// TODO: P2.2 - 1.3 Animate the cube by rotating it around the Y-axis by 36deg per second. Use float time for that.
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelViewProjection);
	
	
	// TODO P2.2 - 1.5 use g_State.arcball::getOrientation() to obtain a model matrix.
	// TODO P2.2 - 1.4 compute a new ModelViewProjection matrix
	
	// TODO P2.2 - 1.6: perform mouse-related camera events here
	//		            use time and g_State.mouse_btn_down_time to compute the difference between button down event and current time.
	//		            rotate camera left/right and up/down by about 72 degrees per second using g_State.mouse_dx & g_State.mouse_dy

	// TODO P2.2 - 1.6: use the g_State.camera to obtain a view and a projection matrix.
	//			        compute a new ModelViewProjection matrix from the camera and the arcball.

	// RENDER THE CUBE:
	glBindVertexArray(g_State.hVAO);	// bind the vertex array to render
	g_State.pShader->Enable();			// enable the shaders

		g_State.pShader->SetUniformMatrix("ModelViewProjection",ModelViewProjection,false);	// pass the matrix to the shader -- transpose flag is false.
		
		// actual render call: we want the indices to be understood as triangles, there are 12 triangles, 
		// each with 3 vertex indices, and indices are of type unsigned int. NULL is an optional offset in the buffer
		glDrawElements(GL_TRIANGLES,3*g_State.nTriangles,GL_UNSIGNED_INT,NULL);

	g_State.pShader->Disable();		// disable the shaders
	glBindVertexArray(0);		// "un-bind" the vertex array

	if (glGetError()!=GL_NO_ERROR) printf("GL ERRORS!\n");
	
	// Finally, buffers can be swapped. This makes the back buffer the current
	// front buffer (it will become visible) and the old front buffer the current
	// back buffer. Afterwards the frame counter for the fps-display is incremented.
	glutSwapBuffers();
	++g_State.frame_count;

	// Frames-Per-Second (fps) are updated roughly every 2 seconds.
	if (g_State.fps_timer.Query()>=2.0) {        
        UpdateTitle(float(double(g_State.frame_count)/g_State.fps_timer.Query()));
        g_State.frame_count=0;
		g_State.fps_timer.Reset();
    }
}


/**
 * The idlefunc is called whenever the CPU is idle.
 * To ensure continuous rendering, a glutPostRedisplay (triggering execution of the displayfunc)
 * is called, here.
 */
void idlefunc(void) {
    glutPostRedisplay();
}

/**
 * The reshapefunc is called whenever the user resizes the screen.
 * The new width and height of the viewport are passed as parameters.
 * It may also be called, whenever a window is maximized of minimized.
 * Here, one should set a new perspective projection matrix. Also the
 * logo done by this framework is layouted here.
 */
void reshapefunc(int width,int height) {
	g_State.win_width=width;				// copy current changes to global variables
    g_State.win_height=height;
    glViewport(0,0,width,height);			// Set the new viewport	

	// TODO: P2.2 - 1.4 Replace below code with your own matrix code
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,double(g_State.win_width)/double(g_State.win_height),0.1f,100.0f);
	// TODO: P2.2 - 1.5 update arcball's window size
	
	// ask for new frame to be rendered
    glutPostRedisplay();
}


/**
 * The mousefunc is called whenever a mouse button was pressed or released.
 * Information about the button (left, right, middle), the state  (down, up) and
 * the current mouse position (x,y) is supplied.
 */
int old_x = 0, old_y = 0;
bool left_mouse_down = false, right_mouse_down = false;
double old_time = 0.0;
void mousefunc(int button,int state,int x,int y) {
	g_bShift = glutGetModifiers()==GLUT_ACTIVE_SHIFT;	// track shift key
	if (button==GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			// TODO: P2.2 - 1.6 hook up camera event to this button.
			//                  in particular, store the current g_State.sys_timer in g_State.mouse_btn_down_time,
			//                  reset g_State.mouse_dx, g_State.mouse_dy
			left_mouse_down = true;
		}
		else {
			// TODO: P2.2 - 1.6 hook up camera events to this button release event.
			//                  in particular, reset g_State.mouse_dx and g_State.mouse_dy
			left_mouse_down = false;
		}
	}
	else if (button==GLUT_RIGHT_BUTTON) {		
		if (state == GLUT_DOWN) {
			// TODO: P2.2 - 1.5 hook up arcball here					
		}
		else {
			// TODO: P2.2 - 1.5 hook up arcball here			
		}		
	}
}

/**
 * The motionfunc is called whenever the user moves the mouse. Since only
 * the current position of the mouse (x,y) is supplied, some book-keeping
 * is necessary (pressed buttons, where pressed etc.)
 */
void motionfunc(int x,int y) {
	// TODO: P2.2 - 1.6
	printf("");
	if (left_mouse_down) {
		// TODO: update g_State.mouse_dx and g_State.mouse_dy in normalized device coordinates first.
		//		 Then, compensate for the aspect ratio and clip the range of each component to [-1,1]
		//		 You can check 
		
	}
	// TODO P2.2 - 1.5: hook up the arcball drag, but only if g_State.arcball.isClicked()	
}


/**
 * Print a little help text.
 * This function is called by the framework, when F1 is pressed.
 */
void printHelp(void) {	
    printf("%s (c) 2013 by J. Schneider\n",g_State.app_name.c_str());
	printf("Visual Computing Center\n");
	printf("King Abdullah University of Science and Technology\n");
	printf("contact: jens.schneider@kaust.edu.sa\n");
    printf("======================================================\n\n");
    printf("   q, ESC      : Quit\n");
	printf("   F1          : This help text\n\n");
}


/**
 * The keyboardfunc is called, whenever a "normal" key is pressed.
 * Parameters passed include the character corresponding to the key
 * being pressed, as well as the current mouse function (x,y)
 */
void keyboardfunc(unsigned char key,int x,int y) {
	errno_t error = 0;
    switch(key) {
	case 'q': 
	case 'Q':
    case 27:
		exit(0);
		break; 
	default:
        printf("undefined key\n");
    }
    glutPostRedisplay();
}


/**
 * The specialfunc is called, whenever a "special" key, such as F1..F12 etc is pressed.
 * These keys are identified via GLUT enumerators, otherwise the function behaves just
 * like the keyboardfunc.
 */
void specialfunc(int key,int x,int y) {	
	float shift_mod = 4.0f*float(glutGetModifiers() == GLUT_ACTIVE_SHIFT) + 1.0f;
	switch (key) {
    case GLUT_KEY_F1: 
        printHelp();
        break;
	case GLUT_KEY_LEFT:
		// TODO: hook this button up to a camera movement
		break;
	case GLUT_KEY_RIGHT:
		// TODO: hook this button up to a camera movement
		break;
	case GLUT_KEY_UP:
		// TODO: hook this button up to a camera movement
		break;
	case GLUT_KEY_DOWN:
		// TODO: hook this button up to a camera movement
		break;
	default:
        printf("undefined key\n");
    }
}


// TODO: P2.2 - 1.7 Callback function for the main menu
void main_menu(int item) {
	printf("main menu received %i\n", item);
}

// TODO: P2.2 - 1.7 Callback function for the model menu
void model_menu(int item) {
	switch (model_t(item)) {
	case 0: break; // TODO: replace this line
		// TODO: add one case statement per element in model_t and load the according obj file.
		// TODO: output an error message if the object could not be loaded properly.
	default:
		std::cout << "ERROR: model_menu not implemented for item=" << item << std::endl;
	}	
}

/**
 * The main function is naturally the entry point to the glut program.
 * Here, the various GLUT callback functions are being registered.
 * Furthermore, a logfile is started.
 */
int main(int argc, char **argv) {
	
	atexit(cleanup);

	// 1. INITIALIZE GLUT
	glutInit(&argc,argv);
    glutInitWindowSize(g_State.win_width,g_State.win_height);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_ALPHA|GLUT_DEPTH);


	// 2. CREATE A WINDOW
	char *tmp=new char[g_State.app_name.size()+2];
	sprintf_s(tmp,g_State.app_name.size()+2,"%s",g_State.app_name.c_str());
    glutCreateWindow(tmp);
	delete[] tmp;


	// 3. REGISTER CALLBACKS
	errno_t error = fopen_s(&logfile,"log.txt","w");
	if (error) logfile=NULL;
		
	if (logfile) {
		fprintf(logfile," done\n");
		fprintf(logfile,"    registering callbacks");
	}
    glutDisplayFunc(displayfunc);
    glutReshapeFunc(reshapefunc); 
    glutMouseFunc(mousefunc);
    glutMotionFunc(motionfunc);	
    glutKeyboardFunc(keyboardfunc);
    glutSpecialFunc(specialfunc);
	glutIdleFunc(idlefunc);
	
	// TODO: P2.2 - 1.7 Add a GLUT menu with a submenu called "Model", add 5 items to load the different obj files
	//		            Attach the menu to the middle mouse button.
	//	                For the main menu, use callback void main_menu(int item)
	//			        For the model menu, use callback void model_menu(int item)

	// TODO: P2.2 - 1.7 Add a GLUT menu, create a model submenu, offer models to select
		
	// 4. APPLICATION SPECIFIC STUFF:
	// timers, logfile, OpenGL initialization etc.
	logHardware();
	initialize();
	printHelp();
    UpdateTitle();
	if (logfile) {
		fprintf(logfile,"Entering main loop\n");
		fprintf(logfile,"Closing logfile\n");
		fclose(logfile);
	}


	// 5. ENTER GLUT MAINLOOP
    glutMainLoop();


    return(0);
}
