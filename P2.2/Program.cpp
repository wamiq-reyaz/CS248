/**************************************************************************************************************

(c) 2004-05 by Jens Schneider, TUM.3D
	mailto:jens.schneider@in.tum.de
	Computer Graphics and Visualization Group
    Institute for Computer Science I15
	Technical University of Munich
	
**************************************************************************************************************/


#include "Program.h"

bool CProgram::m_bGlewInitialized=false;		///< GL Extension Wrangler (glew) is initialized on first instantiation

/**
 * Default Constructor.
 * Initializes glew on first instantiation. 
 * \param void
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 * \see Initialize()
 */
CProgram::CProgram(void) :
  m_bInitialized(false),
  m_bEnabled(false),
  m_hProgram(0),
  m_hVS(0),
  m_hFS(0)
{
    Initialize();
}

/**
 * Copy Constructor.
 * Initializes glew on first instantiation. 
 * \param other - CProgram object to initialize this with
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \remark this object will not be automatically enabled, even if other was.
 * \date Aug.2004
 * \see Initialize()
 */
CProgram::CProgram(const CProgram &other) {

	debugmessage("CProgram::CProgram(const CProgram &other) - calling the copy constructor is _slow_\n");

	m_bInitialized = other.m_bInitialized;
	m_bEnabled     = false;	
	
	// Retrieve handles for all attached shaders
	GLint iNumAttachedShaders;
	glGetProgramiv(other.m_hProgram,GL_ATTACHED_SHADERS,&iNumAttachedShaders);
	if (iNumAttachedShaders==0) {
		message("CProgram::CProgram(const CProgram &other) - Error copying vertex shader.\n");
		m_bInitialized=false;
		return;
	}
	GLuint *hShader=new GLuint[iNumAttachedShaders];
	glGetAttachedShaders(other.m_hProgram,iNumAttachedShaders,&iNumAttachedShaders,hShader);

	// create a new program object
	m_hProgram=glCreateProgram();

	// Attach all shaders that were attached to other also to this
	for (int i=0; i<iNumAttachedShaders; i++) glAttachShader(m_hProgram,hShader[i]);
	
	// link the program together
	glLinkProgram(m_hProgram);

	// check for errors
	GLint iLinked;
	glGetProgramiv(m_hProgram,GL_LINK_STATUS,&iLinked);
				
	if (CheckGLError("CProgram(const CProgram &other)") || iLinked!=int(GL_TRUE)) {
		glDeleteProgram(m_hProgram);
		m_hProgram=0;
		m_bInitialized=false;
		return;
	}
	else {
		m_bInitialized=true;
	}
}



/**
 * Specialized Constructor.
 * Loads any combination of vertex and fragment shader from disk.
 * \param VSFile - name of the file containing the vertex shader
 * \param FSFile - name of the file containing the fragment shader
 * \param src - selects the source of vertex and fragment shader. Can be either TUM3D_DISK or TUM3D_STRING
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 * \see Initialize()
 * \see Load()
 * \see TUM3D_SOURCE
 */
CProgram::CProgram(const char *VSFile, const char *FSFile,TUM3D_SOURCE src) :
  m_bInitialized(false),
  m_bEnabled(false),
  m_hProgram(0)
{
	Initialize();
	Load(VSFile,FSFile,src);
}



/**
 * Standard Destructor.
 * Cleans up the memory automatically.
 * \param void
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 */
CProgram::~CProgram() {
	glDeleteProgram(m_hProgram);
	m_hProgram=0;
}



/**
 * Returns the handle to this shader.
 * \param void
 * \return a const handle. If this is an invalid shader, the handle will be 0.
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Feb.2005
 */
CProgram::operator GLuint(void) const {
	return m_hProgram;
}

/**
 * Initializes the class.
 * If CProgram is initialized for the first time, initialize GLEW
 * \param void
 * \return void 
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 * \see m_bGlewInitialized
 */
void CProgram::Initialize(void) {
	if (!m_bGlewInitialized) {
		if (GLEW_OK!=glewInit()) message("CProgram::Initialize() - GLEW initialization failed!\n");		
		if (GLEW_VERSION_2_0) message("OpenGL 2.0 supported\n");
		else { // check for ARB extensions
			if (glewGetExtension("GL_ARB_shader_objects")) message("ARB_shader_objects supported.\n");
			else {
				message("ARB_shader_objects not supported!\n");
				exit(255);
			}
			if (glewGetExtension("GL_ARB_shading_language_100")) message("ARB_shading_language_100 supported.\n");
			else {
				message("ARB_shading_language_100 not supported!\n");
				exit(255);
			}
		}
		m_bGlewInitialized=true;
	}
#ifdef _TUM3D_DEBUG_	// Anti-Joachim Tactics...
	else {
		if (glMultiTexCoord2f==NULL) message("CProgram::Initialize() - GLEW must be initialized. Set CProgram::m_bGlewInitialized = false in Shader.cpp\n");
	}
#endif
}



/**
 * Loads vertex and fragment shader from disk/memory.
 * Loads any combination of vertex and fragment shader from disk or from a memory position.
 * Generates error/information messages to stdout during loading.
 * If nor successful the handle of the shader will be set to 0.
 * \param VSFile - name of the file containing the vertex shader
 * \param FSFile - name of the file containing the fragment shader
 * \param src - selects the source of vertex and fragment shader. Can be either TUM3D_DISK or TUM3D_STRING
 * \return void
 * \warning Uses glGetError()
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 * \see TUM3D_SOURCE
 */
void CProgram::Load(const char *VSFile, const char *FSFile,TUM3D_SOURCE src) {
	CheckGLError();
	// load
	if (m_hVS) glDeleteShader(m_hVS);        
	if (m_hFS) glDeleteShader(m_hFS);
	m_hVS=0;
	m_hFS=0;
	m_bVSSuccess=true;	// fixed function pipeline is always working
	if (VSFile!=NULL) {
		message("\nVERTEX SHADER:\n");
		if (src==TUM3D_DISK) message(VSFile);
		m_hVS=LoadShader(VSFile,GL_VERTEX_SHADER,src);
		if (m_hVS!=0) message(" OK.\n");
		else {
			m_bVSSuccess=false;
			if (src==TUM3D_DISK) {
				message("ERROR IN: ");
				message(VSFile);
				message("\n");
			}
			else {
				message("---------- ERROR -----------\n");
				int iPos=0;
				int iLine=1;
				char chLine[32];
				char *chVerbose=new char[strlen(VSFile)+1];
				memcpy(chVerbose,VSFile,strlen(VSFile)+1);
				for (int i=0; i<int(strlen(VSFile)); i++) {
					if (chVerbose[i]=='\n') {
						chVerbose[i]='\0';
						sprintf_s(chLine,32,"(%.4i) ",iLine++);
						message(chLine);
						message(&chVerbose[iPos]);
						message("\n");
						iPos=i+1;
					}
				}
				delete[] chVerbose;
			}
		}
	}
	m_bFSSuccess=true;	// fixed function pipeline is always working
	if (FSFile!=NULL) {
		message("\nFRAGMENT SHADER:\n");
		m_hFS=LoadShader(FSFile,GL_FRAGMENT_SHADER,src);
		if (src==TUM3D_DISK) message(FSFile);
		if (m_hFS!=0) message(" OK.\n");
		else {
			m_bFSSuccess=false;
			if (src==TUM3D_DISK) {
				message("ERROR IN: ");
				message(FSFile);
				message("\n");
			}
			else {
				message("---------- ERROR -----------\n");
				int iPos=0;
				int iLine=1;
				char chLine[32];
				char *chVerbose=new char[strlen(FSFile)+1];
				memcpy(chVerbose,FSFile,strlen(FSFile)+1);
				for (int i=0; i<int(strlen(FSFile)); i++) {
					if (chVerbose[i]=='\n') {
						chVerbose[i]='\0';
						sprintf_s(chLine,32,"(%.4i) ",iLine++);
						message(chLine);
						message(&chVerbose[iPos]);
						message("\n");
						iPos=i+1;
					}
				}
				delete[] chVerbose;
			} 
		}		
	}
	
	// attach to program object
	m_hProgram=glCreateProgram();
	if (m_hVS) glAttachShader(m_hProgram,m_hVS); 
	if (m_hFS) glAttachShader(m_hProgram,m_hFS);
	Link();
}

void CProgram::Link(void) {
	// link the program together
	message("\nPROGRAM OBJECT: \n");
	if (m_bVSSuccess && m_bFSSuccess) {
		glLinkProgram(m_hProgram);

		// check for errors
		GLint iLinked;
		glGetProgramiv(m_hProgram,GL_LINK_STATUS,&iLinked);
		WriteInfoLog(m_hProgram,true);
			
		// flag shaders such that they can be deleted when they get detached
		if (m_hVS) glDeleteShader(m_hVS);        
		if (m_hFS) glDeleteShader(m_hFS);
		m_hVS=0;
        m_hFS=0;
		if (CheckGLError("Load()") || iLinked!=GLint(GL_TRUE)) {
			glDeleteProgram(m_hProgram);
			m_hProgram=0;
			m_bInitialized=false;
			return;
		}
		else {
			message(" OK.\n\n");
			m_bInitialized=true;
		}
	}
	else {
		if (m_hVS) glDeleteShader(m_hVS);
		if (m_hFS) glDeleteShader(m_hFS);
		m_hVS=0;
        m_hFS=0;
		glDeleteProgram(m_hProgram);
		m_hProgram=0;
		m_bInitialized=false;
		if (!m_bVSSuccess && !m_bFSSuccess) message("Error in vertex and fragment shaders\n");
		else if (!m_bVSSuccess) message("Error in vertex shader\n");
		else if (!m_bFSSuccess) message("Error in fragment shader\n");
	}
}



/**
 * Writes errors/information messages to stdout.
 * Gets the InfoLogARB of hObject and messages it.
 * \param hObject - a handle to the object.
 * \param bProgram - if true, hObject is a program object, otherwise it is a shader object.
 * \return true: InfoLogARB non-empty and TUM3D_CPROGRAM_STRICT defined OR only warning, false otherwise
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \see message()
 * \date Aug.2004
 */
bool CProgram::WriteInfoLog(GLuint hObject, bool bProgram) {
	// Check for errors
	GLint iLength;
	if (bProgram)	glGetProgramiv(hObject,GL_INFO_LOG_LENGTH,&iLength);
	else			glGetShaderiv(hObject,GL_INFO_LOG_LENGTH,&iLength);

	GLboolean bAtMostWarnings=true;
	if (iLength>1) {		
		GLchar *pcLogInfo=new GLchar[iLength];
		if (bProgram) {
			glGetProgramInfoLog(hObject,iLength,&iLength,pcLogInfo);
			bAtMostWarnings=glIsProgram(hObject);
		}
		else {
			glGetShaderInfoLog(hObject,iLength,&iLength,pcLogInfo);
			bAtMostWarnings=glIsShader(hObject);
		}
		message(pcLogInfo);
		message("\n");
		delete[] pcLogInfo;	
#ifdef TUM3D_CPROGRAM_STRICT
		return true;
#endif
	}
	return !bool(bAtMostWarnings==GL_TRUE); // error occured?	
}



/**
 * Loads a vertex or fragment shader.
 * Loads either a vertex or fragment shader and tries to compile it.
 * \param ShaderDesc - name of the file containing the shader
 * \param Type - either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
 * \param src - defines the source of the shader. Can be either TUM3D_DISK or TUM3D_STRING.
 * \return a handle to the compiled shader if successful, 0 otherwise
 * \warning uses glGetError()
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 * \see TUM3D_SOURCE
 */
GLuint CProgram::LoadShader(const char *ShaderDesc,GLenum Type,TUM3D_SOURCE src) {
	// assert right type
	assert(Type==GL_VERTEX_SHADER || Type==GL_FRAGMENT_SHADER);

	CheckGLError();

	unsigned long lFileSize;
	GLchar *pcShader;
	FILE *fptr;

	// Load and compile vertex shader
	errno_t error=0;
	switch(src) {
		case TUM3D_DISK:
			 error = fopen_s(&fptr,ShaderDesc,"rb");
   			if (error) {
				message("CProgram::LoadShader() - Error opening file ");
				message(ShaderDesc);
				message("!\n");
				return 0;
			}
			if (fseek(fptr,0,SEEK_END)) {
				fclose(fptr);
				message("Error reading file ");
				message(ShaderDesc);
				message("\n");
				return 0;
			}
			lFileSize=ftell(fptr)/sizeof(GLchar);
			fseek(fptr,0,SEEK_SET);
			pcShader=new GLchar[lFileSize+1];
			pcShader[lFileSize]='\0';
			if (lFileSize!=fread(pcShader,sizeof(GLchar),lFileSize,fptr)) {
				fclose(fptr);
				delete[] pcShader;
				debugmessage("Error reading file ");
				debugmessage(ShaderDesc);
				debugmessage("\n");
				return 0;
			}
			fclose(fptr);
			break;
		case TUM3D_STRING:
			pcShader=(GLchar*)ShaderDesc;
			lFileSize=long(strlen(pcShader));
			break;
		default:
			message("CProgram::LoadShader(const char*,GLenum,TUM3D_SOURCE) - Unknown source\n");
			return 0;
			break;
	}
	GLuint hShader=glCreateShader(Type);	
	glShaderSource(hShader,1,(const GLchar**)&pcShader,NULL);	// upload null-terminated shader
	glCompileShader(hShader);
	if (pcShader!=ShaderDesc) delete[] pcShader;

	// Check for compile status
	GLint iCompiled;
	glGetShaderiv(hShader,GL_COMPILE_STATUS,&iCompiled);

	// Check for errors
	bool bError=false;
	if (WriteInfoLog(hShader,false)) {
		glDeleteShader(hShader);
		bError=true;
	}

	if (CheckGLError("LoadProgram()") || iCompiled!=GLint(GL_TRUE)) {
		glDeleteShader(hShader);
		bError=true;
	}

	if (bError) return 0;
	return hShader;
}



/**
 * Enables the program for rendering.
 * Generates error messages if something went wrong (i.e. program not initialized etc.)
 * \param void
 * \return void
 * \warning uses glGetError()
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 */
void CProgram::Enable(void) {
	if (m_bInitialized) {
		CheckGLError();
		glUseProgram(m_hProgram);
		if (!CheckGLError("Enable()")) m_bEnabled=true;
	}
	else debugmessage("CProgram::Enable() - Load program first!\n");
}



/**
 * Disables the program for rendering.
 * Generates error messages if something went wrong (i.e. program not initialized etc.)
 * \param void
 * \return void
 * \warning uses glGetError()
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 */
void CProgram::Disable(void) {
	if (m_bInitialized) {
		CheckGLError();
		glUseProgram(0);
		if (!CheckGLError("Disable()")) m_bEnabled=false;
	}
	else debugmessage("CProgram::Disable() - Load program first!\n");
}



/**
 * Checks and handles glErrors.
 * This routine is verbose when run with _TUM3D_DEBUG_ defined, only.
 * If called with NULL as parameter, queries glGetError() and returns true if glGetError() did not return GL_NO_ERROR.
 * If called with a non-NULL parameter, queries glGetError() and concatenates pcError and the verbosed glError.
 * If in debug mode, the error is output to stderr, otherwise it is silently ignored.
 * \param pcError first part of an error message. May be NULL.
 * \param pcAdditional additional part of error message. May be NULL.
 * \return bool specifying if an error occured (true) or not (false)
 * \warning uses glGetError()
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 */
bool CProgram::CheckGLError(const char *pcError, const char *pcAdditional) const{
#ifdef _TUM3D_DEBUG_
	if (pcError==NULL) {	// Simply check for error, true if an error occured.
#endif

		return (glGetError()!=GL_NO_ERROR);

#ifdef _TUM3D_DEBUG_
	}
	else {	// print out error
		GLenum iError=glGetError();
		char *pcMessage;
		if (pcAdditional) {
			size_t len=16+strlen(pcError)+(pcAdditional ? strlen(pcAdditional) : 0);
			pcMessage=new char[len];
			sprintf_s(pcMessage,len,pcError,pcAdditional);
		}
		else pcMessage=(char*)pcError;

		size_t stLen=strlen(pcMessage)+128;
		char *output=new char[stLen];
		switch (iError) {
			case GL_NO_ERROR:
				if (pcMessage!=pcError) delete[] pcMessage;
				return false;
				break;
			case GL_INVALID_ENUM:		sprintf_s(output,stLen,"CProgram::%s - %s\n",pcMessage,"GL_INVALID_ENUM");		break;
			case GL_INVALID_VALUE:		sprintf_s(output,stLen,"CProgram::%s - %s\n",pcMessage,"GL_INVALID_VALUE");		break;
			case GL_INVALID_OPERATION:	sprintf_s(output,stLen,"CProgram::%s - %s\n",pcMessage,"GL_INVALID_OPERATION");	break;
			case GL_STACK_OVERFLOW:		sprintf_s(output,stLen,"CProgram::%s - %s\n",pcMessage,"GL_STACK_OVERFLOW");	break;
			case GL_STACK_UNDERFLOW:	sprintf_s(output,stLen,"CProgram::%s - %s\n",pcMessage,"GL_STACK_UNDERFLOW");	break;
			case GL_OUT_OF_MEMORY:		sprintf_s(output,stLen,"CProgram::%s - %s\n",pcMessage,"GL_OUT_OF_MEMORY");		break;
			default:					sprintf_s(output,stLen,"CProgram::%s - unknown GL_ERROR\n",pcError);			break;		
		}
		if (pcMessage!=pcError) delete[] pcMessage;
		delete[] output;

		// display the error.
		debugmessage(output);

		return true;	
	}
#endif
}
