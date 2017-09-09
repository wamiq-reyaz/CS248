/**************************************************************************************************************

(c) 2004-05 by Jens Schneider, TUM.3D
	mailto:jens.schneider@in.tum.de
	Computer Graphics and Visualization Group
    Institute for Computer Science I15
	Technische Universitaet Muenchen
	Garching, Germany

(c) 2010 by Jens Schneider, GMSV
	mailto:jens.schneider@kaust.edu.sa
	Geometric Modeling and Scientific Visualization Center (GMSV)
    King Abdullah University of Science and Technology (KAUST)
	Thuwal, Kingdom of Saudi Arabia

	- - - H I S T O R Y - - -
	
	Nov.15	If TUM3D_CPROGRAM_STRICT is defined, treat compiler warnings as errors. This was default 
	        for v1.0b. If left undefined, warnings are written to stdout, but ignored as long as the 
			result is a valid program object.
	Apr.07  renamed to CProgram. Support for OpenGL 2.0.
	Apr.04	work-around for nVidia ß-driver bug with ARB_shader_objects
	Mar.05  added support for arrays
	Mar.05	added support for booleans 
	Mar.05	added support for matrices, including implicit casts from bool and int
	Mar.05	added support for programs in strings as opposed to programs in files
	Mar.05	improved mplicit casting of SetUniformVector().
			This is only enabled when TUM3D_ALLOW_IMPLICIT_CASTS is defined, otherwise SetUniform behaves 
			strict, i.e. no uploading of ints to float uniforms.
	Mar.05	all text output is now done by the method message(). By default, text is send to stderr. 
			If you want to change this behaviour, change CProgram::message() in Shader.inl
	
	Feb.05	added operator GLuint. This allows a CProgram object to be cast into its respective GL handle.

	Aug.04	hey, it runs

	Mar.10	added support for buffer sampler and unsigned ints

**************************************************************************************************************/


#ifndef __TUM3D__CPROGRAM__
#define __TUM3D__CPROGRAM__

#define TUM3D_ALLOW_IMPLICIT_CASTS		///< if enabled, SetUniformVector allows for implicit casting, which can cost performance
//#define TUM3D_SHUT_PIETRAP			///< disables any output
//#define TUM3D_CPROGRAM_STRICT			///< if enabled, GLSL-compiler warnings are treated as errors

#ifdef _DEBUG
#define _TUM3D_DEBUG_	///< switches on debugging output - can be changed per-class.
#endif
#ifdef _WIN32
#ifdef _TUM3D_DEBUG_
#pragma message("    [TUM.3D:CProgram] DEBUG ON.\n")
#else
#ifdef _WIN32
#pragma message("    [TUM.3D:CProgram] DEBUG OFF.\n")
#endif
#endif
#endif

#ifndef _DEBUG
#define TUM3D_SHUT_PIETRAP
#endif

#define CShader CProgram	///< deprecated alias

/**
 * Used to specify sources for shaders.
 * There are two different sources so far, either TUM3D_DISK, which means that
 * a shader is specified by a C-string containing a file name, or TUM3D_STRING,
 * in which case the shader is specified directly as sourcecode in a C-string.
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date March 2004
 */
#ifndef __TUM3D_HAS_DEF_SOURCE_
#define __TUM3D_HAS_DEF_SOURCE_
typedef enum {
	TUM3D_DISK=0,
	TUM3D_STRING
} TUM3D_SOURCE;
#endif

#include "GL/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "GL/glut.h"


/** 
 * Wrapper for handling OpenGL 2.0 conformant program objects.
 * \class CProgram
 * \version 1.0c
 * \warning include _before_ you include anything like gl.h, glut.h etc.
 * \warning requires the GL Extension Wrangler (glew) library.
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date November 2005
 */
class CProgram {
public:
	CProgram();																		///< Standard Constructor.
	CProgram(const CProgram &other);												///< Copy Constructor
	CProgram(const char *VSFile, const char *FSFile,TUM3D_SOURCE src=TUM3D_DISK);	///< Constructor. Loads any combination of vertex (VPFile) and fragment (FPFile) shaders either from disk or from a C-string.
	~CProgram();																	///< Destructor. Automatic clean-up.
	void Load(const char *VSFile, const char *FSFile,TUM3D_SOURCE src=TUM3D_DISK);	///< Loads any combination of vertex (VPFile) and fragment (FPFile) shaders either from disk or from a C-string.
	void Enable(void);																///< Enables this shader for rendering.
	void Disable(void);																///< Disables this shader for rendering (using fixed function pipeline again).
	void Link(void);																///< Link the programs
	operator GLuint(void) const;													///< Returns the handle of this shader.
	
	inline bool IsValid(void);		///< returns true if this program is valid

	inline void SetUniformVector(const GLchar *name,float x=0.0f, float y=0.0f, float z=0.0f, float w=0.0f) const;		///< Sets an uniform parameter.
	inline void SetUniformVector(const GLchar *name,int x=0, int y=0, int z=0, int w=0) const;							///< Sets an uniform parameter.
	inline void SetUniformVector(const GLchar *name,bool x=false, bool y=false,  bool z=false, bool w=false) const;		///< Sets an uniform parameter.
	inline void SetUniformVector(const GLchar *name,unsigned int x,unsigned int y,unsigned int z,unsigned int w) const;	///< Sets an uniform parameter.
	inline void SetUniformVector(const GLchar *name,const float *v) const;				///< Sets an uniform parameter.	
	inline void SetUniformVector(const GLchar *name,const int *i) const;				///< Sets an uniform parameter.	
	inline void SetUniformVector(const GLchar *name,const unsigned int *i) const;		///< Sets an uniform parameter.	
	inline void SetUniformVector(const GLchar *name,const bool *b) const;				///< Sets an uniform parameter.	
	
	
	inline void SetUniformMatrix(const GLchar *name,const float *m,bool bTranspose=false) const;		///< Sets an uniform matrix. Matrices are always float.

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
	inline void SetUniformMatrix(const GLchar *name,const int *m,bool bTranspose=false) const;		///< Sets an uniform matrix. Matrices are always float.
	inline void SetUniformMatrix(const GLchar *name,const bool *m,bool bTranspose=false) const;		///< Sets an uniform matrix. Matrices are always float.
#endif

	inline void SetUniformArray(const GLchar *name,const float *a) const;		///< Sets an uniform array. User has to take care that a is large enough.
	inline void SetUniformArray(const GLchar *name,const int   *a) const;		///< Sets an uniform array. User has to take care that a is large enough.
	inline void SetUniformArray(const GLchar *name,const bool  *a) const;		///< Sets an uniform array. User has to take care that a is large enough.

private:
	inline void message(const char *msg) const;		///< Outputs text to stderr. Change this if you want your text elsewhere.
	inline void debugmessage(const char *msg) const;
	void		Initialize(void);
	GLuint		LoadShader(const char*,GLenum,TUM3D_SOURCE src);
	bool		WriteInfoLog(GLuint,bool);
	bool        CheckGLError(const char *pcError=NULL,const char *pcAdditional=NULL) const;
	bool		m_bInitialized;
	bool		m_bEnabled;
	GLuint		m_hProgram;	
	static bool m_bGlewInitialized;
	bool		m_bVSSuccess;
	bool		m_bFSSuccess;
	GLuint		m_hVS;
	GLuint		m_hFS;
};

#include "Program.inl"

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
#undef TUM3D_ALLOW_IMPLICIT_CASTS
#endif

#endif

