/**************************************************************************************************************

(c) 2004-05 by Jens Schneider, TUM.3D
	mailto:jens.schneider@in.tum.de
	Computer Graphics and Visualization Group
    Institute for Computer Science I15
	Technical University of Munich
	
**************************************************************************************************************/

#include<sstream>

/**
 * Outputs a message to stderr.
 * Change this method to put your text elsewhere.
 * \param msg - a C-string containing a message.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::message(const char *msg) const {
#ifndef TUM3D_SHUT_PIETRAP
	std::stringstream os;
	size_t s=strlen(msg);
	for (size_t n=0; n<s; n++) {
		if (msg[n]=='%') os << "%%";
		else if (msg[n]=='\\') os << "\\";
		else os << msg[n];
	}
	fprintf(stderr,os.str().c_str());
	os.clear();
	fflush(stderr);
#endif
}



/**
 * Outputs a message to stderr iff _TUM3D_DEBUG_ is set.
 * This method builds on message().
 * \param msg - a C-string containing a message.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::debugmessage(const char *msg) const {
#ifndef TUM3D_SHUT_PIETRAP
#ifdef _TUM3D_DEBUG_
	message(msg);
#endif
#endif
}


/**
 * Returns true if this program is valid. 
 * \param void
 * \return true if this program was initialized properly
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Jun.2005
 */
inline bool CProgram::IsValid(void) {
	return m_bInitialized;
}

/**
 * Sets an uniform vector parameter.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param x,y,z,w - up to four float components of the vector to set.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 */
inline void CProgram::SetUniformVector(const GLchar *name,float x, float y, float z, float w) const{
	assert(m_bEnabled);
	CheckGLError();
	
	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformVector(%s,float,...) [getting adress]",name)) return;

	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformVector(");
		debugmessage(name);
		debugmessage(",float,...) - Error getting address\n");
		return;	
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformVector(%s,float,...) [getting type]",name)) return;	

	switch (iType) {
		case GL_FLOAT:						glUniform1f(iLocation,x); break;
		case GL_FLOAT_VEC2:					glUniform2f(iLocation,x,y); break;
		case GL_FLOAT_VEC3:					glUniform3f(iLocation,x,y,z); break;
		case GL_FLOAT_VEC4:					glUniform4f(iLocation,x,y,z,w); break;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_INT:		
		case GL_SAMPLER_1D: 
		case GL_INT_SAMPLER_1D:
		case GL_UNSIGNED_INT_SAMPLER_1D:
		case GL_SAMPLER_2D: 
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_INT_SAMPLER_3D:
		case GL_UNSIGNED_INT_SAMPLER_3D:
		case GL_SAMPLER_BUFFER:
		case GL_INT_SAMPLER_BUFFER:
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		case GL_SAMPLER_CUBE:
		case GL_INT_SAMPLER_CUBE:
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D_RECT_ARB:
		case GL_SAMPLER_2D_RECT_SHADOW_ARB:	glUniform1i(iLocation,int(x)); break;

		case GL_INT_VEC2:					glUniform2i(iLocation,int(x),int(y)); break;
		case GL_INT_VEC3:					glUniform3i(iLocation,int(x),int(y),int(z)); break;
		case GL_INT_VEC4:					glUniform4i(iLocation,int(x),int(y),int(z),int(w)); break;

		case GL_UNSIGNED_INT:				glUniform1ui(iLocation,static_cast<unsigned int>(x)); break;
		case GL_UNSIGNED_INT_VEC2:			glUniform2ui(iLocation,static_cast<unsigned int>(x),static_cast<unsigned int>(y)); break;
		case GL_UNSIGNED_INT_VEC3:			glUniform3ui(iLocation,static_cast<unsigned int>(x),static_cast<unsigned int>(y),static_cast<unsigned int>(z)); break;
		case GL_UNSIGNED_INT_VEC4:			glUniform4ui(iLocation,static_cast<unsigned int>(x),static_cast<unsigned int>(y),static_cast<unsigned int>(z),static_cast<unsigned int>(w)); break;

		case GL_BOOL:  						glUniform1f(iLocation,x); break;
		case GL_BOOL_VEC2:					glUniform2f(iLocation,x,y); break;
		case GL_BOOL_VEC3:					glUniform3f(iLocation,x,y,z); break;
		case GL_BOOL_VEC4:					glUniform4f(iLocation,x,y,z,w); break;
#endif

		default: 
			debugmessage("CProgram::SetUniformVector(");
			debugmessage(name);
			debugmessage("float,...) - Unknown type\n"); 
			exit(0);
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformVector(%s,float,...)",name);
#endif
}



/**
 * Sets an uniform vector parameter.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param x,y,z,w - up to four bool components of the vector to set.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::SetUniformVector(const GLchar *name,bool x, bool y, bool z, bool w) const {
	assert(m_bEnabled);
	CheckGLError();
	
	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformVector(%s,bool,...) [getting adress]",name)) return;

	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformVector(");
		debugmessage(name);
		debugmessage(",bool,...) - Error getting address\n");
		return;	
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformVector(%s,bool,...) [getting type]",name)) return;	

	switch (iType) {
		case GL_BOOL:						glUniform1i(iLocation,(x ? 1 : 0)); break;
		case GL_BOOL_VEC2:					glUniform2i(iLocation,(x ? 1 : 0),(y ? 1 : 0)); break;
		case GL_BOOL_VEC3:					glUniform3i(iLocation,(x ? 1 : 0),(y ? 1 : 0),(z ? 1 : 0)); break;
		case GL_BOOL_VEC4:					glUniform4i(iLocation,(x ? 1 : 0),(y ? 1 : 0),(z ? 1 : 0),(w ? 1 : 0)); break;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_FLOAT:						glUniform1f(iLocation,(x ? 1.0f : 0.0f)); break;
		case GL_FLOAT_VEC2:					glUniform2f(iLocation,(x ? 1.0f : 0.0f),(y ? 1.0f : 0.0f)); break;
		case GL_FLOAT_VEC3:					glUniform3f(iLocation,(x ? 1.0f : 0.0f),(y ? 1.0f : 0.0f),(z ? 1.0f : 0.0f)); break;
		case GL_FLOAT_VEC4:					glUniform4f(iLocation,(x ? 1.0f : 0.0f),(y ? 1.0f : 0.0f),(z ? 1.0f : 0.0f),(w ? 1.0f : 0.0f)); break;
		case GL_INT:						glUniform1i(iLocation,(x ? 1 : 0)); break;
		case GL_INT_VEC2:					glUniform2i(iLocation,(x ? 1 : 0),(y ? 1 : 0)); break;
		case GL_INT_VEC3:					glUniform3i(iLocation,(x ? 1 : 0),(y ? 1 : 0),(z ? 1 : 0)); break;
		case GL_INT_VEC4:					glUniform4i(iLocation,(x ? 1 : 0),(y ? 1 : 0),(z ? 1 : 0),(w ? 1 : 0)); break;
		case GL_UNSIGNED_INT:				glUniform1ui(iLocation,(x ? 1 : 0)); break;
		case GL_UNSIGNED_INT_VEC2:			glUniform2ui(iLocation,(x ? 1 : 0),(y ? 1 : 0)); break;
		case GL_UNSIGNED_INT_VEC3:			glUniform3ui(iLocation,(x ? 1 : 0),(y ? 1 : 0),(z ? 1 : 0)); break;
		case GL_UNSIGNED_INT_VEC4:			glUniform4ui(iLocation,(x ? 1 : 0),(y ? 1 : 0),(z ? 1 : 0),(w ? 1 : 0)); break;

#endif

		default: 
			debugmessage("CProgram::SetUniformVector(");
			debugmessage(name);
			debugmessage(",bool,...) - Unknown type\n"); 
			exit(0);
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformVector(%s,bool,...)",name);
#endif
}



/**
 * Sets an uniform vector parameter.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param x,y,z,w - four int components of the vector to set.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 */
inline void CProgram::SetUniformVector(const GLchar *name,int x,int y,int z,int w) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocationARB(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformVector(%s,int,...) [getting adress]" )) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformVector(");
		debugmessage(name);
		debugmessage(",int,...) - Error getting address\n");
		return;	
	}


	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformVector(%s,int,...) [getting type]",name)) return;

	switch (iType) {		
		case GL_INT:
		case GL_SAMPLER_1D: 
		case GL_INT_SAMPLER_1D:
		case GL_UNSIGNED_INT_SAMPLER_1D:
		case GL_SAMPLER_2D: 
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_INT_SAMPLER_3D:
		case GL_UNSIGNED_INT_SAMPLER_3D:
		case GL_SAMPLER_BUFFER:
		case GL_INT_SAMPLER_BUFFER:
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		case GL_SAMPLER_CUBE:
		case GL_INT_SAMPLER_CUBE:
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D_RECT_ARB:
		case GL_SAMPLER_2D_RECT_SHADOW_ARB:	glUniform1i(iLocation,x); break;

		case GL_INT_VEC2:					glUniform2i(iLocation,x,y); break;
		case GL_INT_VEC3:					glUniform3i(iLocation,x,y,z); break;
		case GL_INT_VEC4:					glUniform4i(iLocation,x,y,z,w); break;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_UNSIGNED_INT:				glUniform1ui(iLocation,static_cast<unsigned int>(x)); break;
		case GL_UNSIGNED_INT_VEC2:			glUniform2ui(iLocation,static_cast<unsigned int>(x),static_cast<unsigned int>(y)); break;
		case GL_UNSIGNED_INT_VEC3:			glUniform3ui(iLocation,static_cast<unsigned int>(x),static_cast<unsigned int>(y),static_cast<unsigned int>(z)); break;
		case GL_UNSIGNED_INT_VEC4:			glUniform4ui(iLocation,static_cast<unsigned int>(x),static_cast<unsigned int>(y),static_cast<unsigned int>(z),static_cast<unsigned int>(w)); break;

		case GL_BOOL:						glUniform1i(iLocation,x); break;
		case GL_BOOL_VEC2:					glUniform2i(iLocation,x,y); break;
		case GL_BOOL_VEC3:					glUniform3i(iLocation,x,y,z); break;
		case GL_BOOL_VEC4:					glUniform4i(iLocation,x,y,z,w); break;

		case GL_FLOAT:						glUniform1f(iLocation,float(x)); break;
		case GL_FLOAT_VEC2:					glUniform2f(iLocation,float(x),float(y)); break;
		case GL_FLOAT_VEC3:					glUniform3f(iLocation,float(x),float(y),float(z)); break;
		case GL_FLOAT_VEC4:					glUniform4f(iLocation,float(x),float(y),float(z),float(w)); break;
#endif

		default: 
			debugmessage("CProgram::SetUniformVector(");
			debugmessage(name);
			debugmessage(",int,...) - Unknown type\n"); 
			exit(0);
			break;
	}	
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformVector(%s,int,...)",name);
#endif
}

/**
 * Sets an uniform vector parameter.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param x,y,z,w - four unsigned int components of the vector to set.
 * \return void
 * \author <a href="mailto:jens.schneider@kaust.edu.sa">Jens Schneider</a>
 * \date Mar.2010
 */
inline void CProgram::SetUniformVector(const GLchar *name,unsigned int x,unsigned int y,unsigned int z,unsigned int w) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocationARB(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformVector(%s,uint,...) [getting adress]" )) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformVector(");
		debugmessage(name);
		debugmessage(",uint,...) - Error getting address\n");
		return;	
	}


	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformVector(%s,uint,...) [getting type]",name)) return;

	switch (iType) {				
		case GL_SAMPLER_1D: 
		case GL_INT_SAMPLER_1D:
		case GL_UNSIGNED_INT_SAMPLER_1D:
		case GL_SAMPLER_2D: 
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_INT_SAMPLER_3D:
		case GL_UNSIGNED_INT_SAMPLER_3D:
		case GL_SAMPLER_BUFFER:
		case GL_INT_SAMPLER_BUFFER:
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		case GL_SAMPLER_CUBE:
		case GL_INT_SAMPLER_CUBE:
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D_RECT_ARB:
		case GL_SAMPLER_2D_RECT_SHADOW_ARB:	glUniform1i(iLocation,x); break;

		case GL_UNSIGNED_INT:				glUniform1ui(iLocation,x); break;
		case GL_UNSIGNED_INT_VEC2:			glUniform2ui(iLocation,x,y); break;
		case GL_UNSIGNED_INT_VEC3:			glUniform3ui(iLocation,x,y,z); break;
		case GL_UNSIGNED_INT_VEC4:			glUniform4ui(iLocation,x,y,z,w); break;

		
#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_INT:						glUniform1i(iLocation,static_cast<int>(x)); break;
		case GL_INT_VEC2:					glUniform2i(iLocation,static_cast<int>(x),static_cast<int>(y)); break;
		case GL_INT_VEC3:					glUniform3i(iLocation,static_cast<int>(x),static_cast<int>(y),static_cast<int>(z)); break;
		case GL_INT_VEC4:					glUniform4i(iLocation,static_cast<int>(x),static_cast<int>(y),static_cast<int>(z),static_cast<int>(w)); break;

		case GL_BOOL:						glUniform1i(iLocation,x); break;
		case GL_BOOL_VEC2:					glUniform2i(iLocation,x,y); break;
		case GL_BOOL_VEC3:					glUniform3i(iLocation,x,y,z); break;
		case GL_BOOL_VEC4:					glUniform4i(iLocation,x,y,z,w); break;

		case GL_FLOAT:						glUniform1f(iLocation,float(x)); break;
		case GL_FLOAT_VEC2:					glUniform2f(iLocation,float(x),float(y)); break;
		case GL_FLOAT_VEC3:					glUniform3f(iLocation,float(x),float(y),float(z)); break;
		case GL_FLOAT_VEC4:					glUniform4f(iLocation,float(x),float(y),float(z),float(w)); break;
#endif

		default: 
			debugmessage("CProgram::SetUniformVector(");
			debugmessage(name);
			debugmessage(",int,...) - Unknown type\n"); 
			exit(0);
			break;
	}	
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformVector(%s,int,...)",name);
#endif
}



/**
 * Sets an uniform vector parameter.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param v - a float vector containing up to four elements.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 */
inline void CProgram::SetUniformVector(const GLchar *name,const float *v) const {	
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformVector(%s,float*) [getting adress]",name)) return;	

	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformVector(");
		debugmessage(name);
		debugmessage(",float*) - Error getting address\n");
		return;	
	}


	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformVector(%s,float*) [getting type]" ,name)) return;

	switch (iType) {
		case GL_FLOAT:						glUniform1fv(iLocation,1,v); break;
		case GL_FLOAT_VEC2:					glUniform2fv(iLocation,1,v); break;
		case GL_FLOAT_VEC3:					glUniform3fv(iLocation,1,v); break;
		case GL_FLOAT_VEC4:					glUniform4fv(iLocation,1,v); break;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_INT:		
		case GL_SAMPLER_1D: 
		case GL_INT_SAMPLER_1D:
		case GL_UNSIGNED_INT_SAMPLER_1D:
		case GL_SAMPLER_2D: 
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_INT_SAMPLER_3D:
		case GL_UNSIGNED_INT_SAMPLER_3D:
		case GL_SAMPLER_BUFFER:
		case GL_INT_SAMPLER_BUFFER:
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		case GL_SAMPLER_CUBE:
		case GL_INT_SAMPLER_CUBE:
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D_RECT_ARB:
		case GL_SAMPLER_2D_RECT_SHADOW_ARB:	glUniform1i(iLocation,int(v[0])); break;

		case GL_UNSIGNED_INT:				glUniform1ui(iLocation,static_cast<unsigned int>(v[0])); break;
		case GL_UNSIGNED_INT_VEC2:			glUniform2ui(iLocation,static_cast<unsigned int>(v[0]),static_cast<unsigned int>(v[1])); break;
		case GL_UNSIGNED_INT_VEC3:			glUniform3ui(iLocation,static_cast<unsigned int>(v[0]),static_cast<unsigned int>(v[1]),static_cast<unsigned int>(v[2])); break;
		case GL_UNSIGNED_INT_VEC4:			glUniform4ui(iLocation,static_cast<unsigned int>(v[0]),static_cast<unsigned int>(v[1]),static_cast<unsigned int>(v[2]),static_cast<unsigned int>(v[2])); break;

		case GL_INT_VEC2:					glUniform2i(iLocation,int(v[0]),int(v[1])); break;
		case GL_INT_VEC3:					glUniform3i(iLocation,int(v[0]),int(v[1]),int(v[2])); break;
		case GL_INT_VEC4:					glUniform4i(iLocation,int(v[0]),int(v[1]),int(v[2]),int(v[3])); break;
		case GL_BOOL:						glUniform1fv(iLocation,1,v); break;
		case GL_BOOL_VEC2:					glUniform2fv(iLocation,1,v); break;
		case GL_BOOL_VEC3:					glUniform3fv(iLocation,1,v); break;
		case GL_BOOL_VEC4:					glUniform4fv(iLocation,1,v); break;
#endif

		default: 
			debugmessage("CProgram::SetUniformVector(");
			debugmessage(name);
			debugmessage(",float*) - Unknown type\n"); 
			exit(0);
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformVector(%s,float*)",name);
#endif
}



/**
 * Sets an uniform vector parameter.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param i - an int vector containing up to 4 elements.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Aug.2004
 */
inline void CProgram::SetUniformVector(const GLchar *name,const int *i) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformVector(%s,int*) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformVector(");
		debugmessage(name);
		debugmessage(",int*) - Error getting address\n");
		return;	
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformVector(%s,int*) [getting type]",name)) return;

	switch (iType) {
		case GL_INT:		
		case GL_SAMPLER_1D: 
		case GL_INT_SAMPLER_1D:
		case GL_UNSIGNED_INT_SAMPLER_1D:
		case GL_SAMPLER_2D: 
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_INT_SAMPLER_3D:
		case GL_UNSIGNED_INT_SAMPLER_3D:
		case GL_SAMPLER_BUFFER:
		case GL_INT_SAMPLER_BUFFER:
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		case GL_SAMPLER_CUBE:
		case GL_INT_SAMPLER_CUBE:
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D_RECT_ARB:
		case GL_SAMPLER_2D_RECT_SHADOW_ARB:	glUniform1i(iLocation,i[0]); break;

		case GL_INT_VEC2:					glUniform2iv(iLocation,1,i); break;
		case GL_INT_VEC3:					glUniform3iv(iLocation,1,i); break;
		case GL_INT_VEC4:					glUniform4iv(iLocation,1,i); break;
#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_UNSIGNED_INT:				glUniform1ui(iLocation,static_cast<unsigned int>(i[0])); break;
		case GL_UNSIGNED_INT_VEC2:			glUniform2ui(iLocation,static_cast<unsigned int>(i[0]),static_cast<unsigned int>(i[1])); break;
		case GL_UNSIGNED_INT_VEC3:			glUniform3ui(iLocation,static_cast<unsigned int>(i[0]),static_cast<unsigned int>(i[1]),static_cast<unsigned int>(i[2])); break;
		case GL_UNSIGNED_INT_VEC4:			glUniform4ui(iLocation,static_cast<unsigned int>(i[0]),static_cast<unsigned int>(i[1]),static_cast<unsigned int>(i[2]),static_cast<unsigned int>(i[2])); break;
		case GL_BOOL:						glUniform1iv(iLocation,1,i); break;
		case GL_BOOL_VEC2:					glUniform2iv(iLocation,1,i); break;
		case GL_BOOL_VEC3:					glUniform3iv(iLocation,1,i); break;
		case GL_BOOL_VEC4:					glUniform4iv(iLocation,1,i); break;
		case GL_FLOAT:						glUniform1f(iLocation,float(i[0])); break;
		case GL_FLOAT_VEC2:					glUniform2f(iLocation,float(i[0]),float(i[1])); break;
		case GL_FLOAT_VEC3:					glUniform3f(iLocation,float(i[0]),float(i[1]),float(i[2])); break;
		case GL_FLOAT_VEC4:					glUniform4f(iLocation,float(i[0]),float(i[1]),float(i[2]),float(i[3])); break;
#endif
		default: 
			debugmessage("CProgram::SetUniformVector(");
			debugmessage(name);
			debugmessage(",int*) - Unknown type\n");
			exit(0);
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformVector(%s,int*)",name);
#endif
}



/**
 * Sets an uniform vector parameter.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param i - an unsigned int vector containing up to 4 elements.
 * \return void
 * \author <a href="mailto:jens.schneider@kaust.edu.sa">Jens Schneider</a>
 * \date Mar.2010
 */
inline void CProgram::SetUniformVector(const GLchar *name,const unsigned int *i) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformVector(%s,int*) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformVector(");
		debugmessage(name);
		debugmessage(",int*) - Error getting address\n");
		return;	
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformVector(%s,int*) [getting type]",name)) return;

	switch (iType) {
		case GL_SAMPLER_1D: 
		case GL_INT_SAMPLER_1D:
		case GL_UNSIGNED_INT_SAMPLER_1D:
		case GL_SAMPLER_2D: 
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_INT_SAMPLER_3D:
		case GL_UNSIGNED_INT_SAMPLER_3D:
		case GL_SAMPLER_BUFFER:
		case GL_INT_SAMPLER_BUFFER:
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		case GL_SAMPLER_CUBE:
		case GL_INT_SAMPLER_CUBE:
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D_RECT_ARB:
		case GL_SAMPLER_2D_RECT_SHADOW_ARB:	glUniform1i(iLocation,i[0]); break;

		case GL_UNSIGNED_INT:				glUniform1uiv(iLocation,1,i); break;
		case GL_UNSIGNED_INT_VEC2:			glUniform2uiv(iLocation,1,i); break;
		case GL_UNSIGNED_INT_VEC3:			glUniform3uiv(iLocation,1,i); break;
		case GL_UNSIGNED_INT_VEC4:			glUniform4uiv(iLocation,1,i); break;
		
#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_INT:						glUniform1i(iLocation,static_cast<int>(i[0])); break;
		case GL_INT_VEC2:					glUniform2i(iLocation,static_cast<int>(i[0]),static_cast<int>(i[1])); break;
		case GL_INT_VEC3:					glUniform3i(iLocation,static_cast<int>(i[0]),static_cast<int>(i[1]),static_cast<int>(i[2])); break;
		case GL_INT_VEC4:					glUniform4i(iLocation,static_cast<int>(i[0]),static_cast<int>(i[1]),static_cast<int>(i[2]),static_cast<int>(i[3])); break;
		case GL_BOOL:						glUniform1uiv(iLocation,1,i); break;
		case GL_BOOL_VEC2:					glUniform2uiv(iLocation,1,i); break;
		case GL_BOOL_VEC3:					glUniform3uiv(iLocation,1,i); break;
		case GL_BOOL_VEC4:					glUniform4uiv(iLocation,1,i); break;
		case GL_FLOAT:						glUniform1f(iLocation,float(i[0])); break;
		case GL_FLOAT_VEC2:					glUniform2f(iLocation,float(i[0]),float(i[1])); break;
		case GL_FLOAT_VEC3:					glUniform3f(iLocation,float(i[0]),float(i[1]),float(i[2])); break;
		case GL_FLOAT_VEC4:					glUniform4f(iLocation,float(i[0]),float(i[1]),float(i[2]),float(i[3])); break;
#endif
		default: 
			debugmessage("CProgram::SetUniformVector(");
			debugmessage(name);
			debugmessage(",int*) - Unknown type\n");
			exit(0);
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformVector(%s,int*)",name);
#endif
}



/**
 * Sets an uniform vector parameter.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param b - a bool vector containing up to 4 elements.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::SetUniformVector(const GLchar *name,const bool *b) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformVector(%s,bool*) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformVector(");
		debugmessage(name);
		debugmessage(",bool*) -  Error getting address\n");
		return;	
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformVector(%s,bool*) [getting type]",name)) return;

	switch (iType) {
		case GL_BOOL:						glUniform1i(iLocation,(b[0] ? 1 : 0)); break;
		case GL_BOOL_VEC2:					glUniform2i(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0)); break;
		case GL_BOOL_VEC3:					glUniform3i(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0),(b[2] ? 1 : 0)); break;
		case GL_BOOL_VEC4:					glUniform4i(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0),(b[2] ? 1 : 0),(b[3] ? 1 : 0)); break;
#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_UNSIGNED_INT:				glUniform1ui(iLocation,(b[0] ? 1 : 0)); break;
		case GL_UNSIGNED_INT_VEC2:			glUniform2ui(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0)); break;
		case GL_UNSIGNED_INT_VEC3:			glUniform3ui(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0),(b[2] ? 1 : 0)); break;
		case GL_UNSIGNED_INT_VEC4:			glUniform4ui(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0),(b[2] ? 1 : 0),(b[3] ? 1 : 0)); break;

		case GL_INT:						glUniform1i(iLocation,(b[0] ? 1 : 0)); break;
		case GL_INT_VEC2:					glUniform2i(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0)); break;
		case GL_INT_VEC3:					glUniform3i(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0),(b[2] ? 1 : 0)); break;
		case GL_INT_VEC4:					glUniform4i(iLocation,(b[0] ? 1 : 0),(b[1] ? 1 : 0),(b[2] ? 1 : 0),(b[3] ? 1 : 0)); break;
		case GL_FLOAT:						glUniform1f(iLocation,(b[0] ? 1.0f : 0.0f)); break;
		case GL_FLOAT_VEC2:					glUniform2f(iLocation,(b[0] ? 1.0f : 0.0f),(b[1] ? 1.0f : 0.0f)); break;
		case GL_FLOAT_VEC3:					glUniform3f(iLocation,(b[0] ? 1.0f : 0.0f),(b[1] ? 1.0f : 0.0f),(b[2] ? 1.0f : 0.0f)); break;
		case GL_FLOAT_VEC4:					glUniform4f(iLocation,(b[0] ? 1.0f : 0.0f),(b[1] ? 1.0f : 0.0f),(b[2] ? 1.0f : 0.0f),(b[3] ? 1.0f : 0.0f)); break;
#endif
		default: 
			debugmessage("CProgram::SetUniformVector(");
			debugmessage(name);
			debugmessage(",bool*) - Unknown type\n");
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformVector(%s,bool*)",name);
#endif
}



/**
 * Sets an uniform matrix.
 * Matrices are always of type float.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param m - a float array containing up to 16 floats for the matrix.
 * \param bTranspose - if true, the matrix will be transposed before uploading.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::SetUniformMatrix(const GLchar *name,const float *m,bool bTranspose) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocationARB(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformMatrix(%s,float*,bool) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformMatrix(");
		debugmessage(name);
		debugmessage(",float*,bool) - Error getting address\n");
		return;
	}

	glGetActiveUniformARB(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformMatrix(%s,float*,bool) [getting type]",name)) return;

	switch (iType) {
		case GL_FLOAT_MAT2:					glUniformMatrix2fv(iLocation,1,bTranspose,m); break;
		case GL_FLOAT_MAT3:					glUniformMatrix3fv(iLocation,1,bTranspose,m); break;
		case GL_FLOAT_MAT4:					glUniformMatrix4fv(iLocation,1,bTranspose,m); break;
		default: 
			debugmessage("CProgram::SetUniformMatrix(");
			debugmessage(name);
			debugmessage(",float*,bool) - Unknown type\n");
		break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformMatrix(%s,float*,bool)",name);
#endif
}



#ifdef TUM3D_ALLOW_IMPLICIT_CASTS

/**
 * Sets an uniform matrix.
 * Matrices are always of type float.
 * \warning uses glGetError();
 * \remark only available if TUM3D_ALLOW_IMPLICIT_CASTS is defined.
 * \param name - name of the parameter
 * \param m - an int array containing up to 16 ints for the matrix. Ints are converted to float before uploading.
 * \param bTranspose - if true, the matrix will be transposed before uploading.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::SetUniformMatrix(const GLchar *name,const int *m, bool bTranspose) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformMatrix(%s,int*,bool) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformMatrix(");
		debugmessage(name);
		debugmessage(",int*,bool) - Error getting address\n");
		return;
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformMatrix(%s,float*,bool) [getting type]",name)) return;

	float M[16];
	switch (iType) {
		case GL_FLOAT_MAT2:
			for (unsigned int ui=0; ui<4; ui++) M[ui]=float(m[ui]);
			glUniformMatrix2fv(iLocation,1,bTranspose,M); 
			break;
		case GL_FLOAT_MAT3:
			for (unsigned int ui=0; ui<9; ui++) M[ui]=float(m[ui]);
			glUniformMatrix3fv(iLocation,1,bTranspose,M); 
			break;
		case GL_FLOAT_MAT4:
			for (unsigned int ui=0; ui<16; ui++) M[ui]=float(m[ui]);
			glUniformMatrix4fv(iLocation,1,bTranspose,M); 
			break;
		default: 
			debugmessage("CProgram::SetUniformMatrix(");
			debugmessage(name);
			debugmessage(",int*,bool) - Unknown type\n"); 
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformMatrix(%s,int*,bool)",name);
#endif
}



/**
 * Sets an uniform matrix.
 * Matrices are always of type float.
 * \warning uses glGetError();
 * \remark only available if TUM3D_ALLOW_IMPLICIT_CASTS is defined.
 * \param name - name of the parameter
 * \param m - an int array containing up to 16 ints for the matrix. Ints are converted to float before uploading.
 * \param bTranspose - if true, the matrix will be transposed before uploading.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::SetUniformMatrix(const GLchar *name,const bool *m, bool bTranspose) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocationARB(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformMatrix(%s,int*,bool) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformMatrix(");
		debugmessage(name);
		debugmessage(",bool*,bool) - Error getting address\n");
		return;
	}

	glGetActiveUniformARB(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformMatrix(%s,float*,bool) [getting type]",name)) return;

	float M[16];
	switch (iType) {
		case GL_FLOAT_MAT2:
			for (unsigned int ui=0; ui<4; ui++) M[ui]=(m[ui] ? 1.0f : 0.0f);
			glUniformMatrix2fv(iLocation,1,bTranspose,M); 
			break;
		case GL_FLOAT_MAT3:
			for (unsigned int ui=0; ui<9; ui++) M[ui]=(m[ui] ? 1.0f : 0.0f);
			glUniformMatrix3fv(iLocation,1,bTranspose,M); 
			break;
		case GL_FLOAT_MAT4:
			for (unsigned int ui=0; ui<16; ui++) M[ui]=(m[ui] ? 1.0f : 0.0f);
			glUniformMatrix4fv(iLocation,1,bTranspose,M); 
			break;
		default: 
			debugmessage("CProgram::SetUniformMatrix(");
			debugmessage(name);
			debugmessage(",bool*,bool) - Unknown type\n"); 
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformMatrix(%s,int*,bool)",name);
#endif
}

#endif // TUM3D_ALLOW_IMPLICIT_CASTS



/**
 * Sets an uniform array.
 * Sets the entire array at once. Single positions can still be set using the other SetUniform*() methods.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param a - a float array containing enough floats to fill the entire uniform array.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::SetUniformArray(const GLchar *name,const float *a) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformArray(%s,float*) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformArray(");
		debugmessage(name);
		debugmessage(",float*) -  Error getting address\n");
		return;	
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformArray(%s,float*) [getting type]",name)) return;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
	int *iArray;
#endif

	switch (iType) {
		case GL_FLOAT:						glUniform1fv(iLocation,iSize,a); break;
		case GL_FLOAT_VEC2:					glUniform2fv(iLocation,iSize,a); break;
		case GL_FLOAT_VEC3:					glUniform3fv(iLocation,iSize,a); break;
		case GL_FLOAT_VEC4:					glUniform4fv(iLocation,iSize,a); break;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_BOOL:						glUniform1fv(iLocation,iSize,a); break;
		case GL_BOOL_VEC2:					glUniform2fv(iLocation,iSize,a); break;
		case GL_BOOL_VEC3:					glUniform3fv(iLocation,iSize,a); break;
		case GL_BOOL_VEC4:					glUniform4fv(iLocation,iSize,a); break;
		
		case GL_INT:		
		case GL_SAMPLER_1D: 
		case GL_INT_SAMPLER_1D:
		case GL_UNSIGNED_INT_SAMPLER_1D:
		case GL_SAMPLER_2D: 
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_INT_SAMPLER_3D:
		case GL_UNSIGNED_INT_SAMPLER_3D:
		case GL_SAMPLER_BUFFER:
		case GL_INT_SAMPLER_BUFFER:
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		case GL_SAMPLER_CUBE:
		case GL_INT_SAMPLER_CUBE:
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D_RECT_ARB:
		case GL_SAMPLER_2D_RECT_SHADOW_ARB:		iArray=new int[iSize];
			for (int i=0; i<iSize; i++) iArray[i]=int(a[i]);
			glUniform1iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;

		case GL_INT_VEC2:
			iArray=new int[2*iSize];
			for (int i=0; i<2*iSize; i++) iArray[i]=int(a[i]);
			glUniform2iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;
		case GL_INT_VEC3:
			iArray=new int[3*iSize];
			for (int i=0; i<3*iSize; i++) iArray[i]=int(a[i]);
			glUniform3iv(iLocation,iSize,iArray);
			delete[] iArray;
			break;
		case GL_INT_VEC4:
			iArray=new int[4*iSize];
			for (int i=0; i<4*iSize; i++) iArray[i]=int(a[i]);
			glUniform4iv(iLocation,iSize,iArray);
			delete[] iArray;
			break;
#endif

		default: 
			debugmessage("CProgram::SetUniformArray(");
			debugmessage(name);
			debugmessage(",float*) - Unknown type\n");
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformArray(%s,float*)",name);
#endif
}



/**
 * Sets an uniform array.
 * Sets the entire array at once. Single positions can still be set using the other SetUniform*() methods.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param a - an int array containing enough floats to fill the entire uniform array.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::SetUniformArray(const GLchar *name,const int   *a) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformArray(%s,int*) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformArray(");
		debugmessage(name);
		debugmessage(",int*) -  Error getting address\n");
		return;	
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformArray(%s,int*) [getting type]",name)) return;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
	float *fArray;
#endif

	switch (iType) {
		case GL_INT:		
		case GL_SAMPLER_1D: 
		case GL_INT_SAMPLER_1D:
		case GL_UNSIGNED_INT_SAMPLER_1D:
		case GL_SAMPLER_2D: 
		case GL_INT_SAMPLER_2D:
		case GL_UNSIGNED_INT_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_INT_SAMPLER_3D:
		case GL_UNSIGNED_INT_SAMPLER_3D:
		case GL_SAMPLER_BUFFER:
		case GL_INT_SAMPLER_BUFFER:
		case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		case GL_SAMPLER_CUBE:
		case GL_INT_SAMPLER_CUBE:
		case GL_UNSIGNED_INT_SAMPLER_CUBE:
		case GL_SAMPLER_1D_SHADOW:
		case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D_RECT_ARB:
		case GL_SAMPLER_2D_RECT_SHADOW_ARB:	glUniform1iv(iLocation,iSize,a); break;

		case GL_INT_VEC2:					glUniform2iv(iLocation,iSize,a); break;
		case GL_INT_VEC3:					glUniform3iv(iLocation,iSize,a); break;
		case GL_INT_VEC4:					glUniform4iv(iLocation,iSize,a); break;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_BOOL:						glUniform1iv(iLocation,iSize,a); break;
		case GL_BOOL_VEC2:					glUniform2iv(iLocation,iSize,a); break;
		case GL_BOOL_VEC3:					glUniform3iv(iLocation,iSize,a); break;
		case GL_BOOL_VEC4:					glUniform4iv(iLocation,iSize,a); break;
		
		case GL_FLOAT:
			fArray=new float[iSize];
			for (int i=0; i<iSize; i++) fArray[i]=float(a[i]);
			glUniform1fv(iLocation,iSize,fArray); 
			delete[] fArray;
			break;
		case GL_FLOAT_VEC2:
			fArray=new float[2*iSize];
			for (int i=0; i<2*iSize; i++) fArray[i]=float(a[i]);
			glUniform2fv(iLocation,iSize,fArray); 
			delete[] fArray;
			break;
		case GL_FLOAT_VEC3:
			fArray=new float[3*iSize];
			for (int i=0; i<3*iSize; i++) fArray[i]=float(a[i]);
			glUniform3fv(iLocation,iSize,fArray);
			delete[] fArray;
			break;
		case GL_FLOAT_VEC4:
			fArray=new float[4*iSize];
			for (int i=0; i<4*iSize; i++) fArray[i]=float(a[i]);
			glUniform4fv(iLocation,iSize,fArray);
			delete[] fArray;
			break;
#endif

		default: 
			debugmessage("CProgram::SetUniformArray(");
			debugmessage(name);
			debugmessage(",int*) - Unknown type\n");
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformArray(%s,int*)",name);
#endif
}



/**
 * Sets an uniform array.
 * Sets the entire array at once. Single positions can still be set using the other SetUniform*() methods.
 * \warning uses glGetError();
 * \param name - name of the parameter
 * \param a - a bool array containing enough floats to fill the entire uniform array.
 * \return void
 * \author <a href="mailto:jens.schneider@in.tum.de">Jens Schneider</a>
 * \date Mar.2005
 */
inline void CProgram::SetUniformArray(const GLchar *name,const bool  *a) const {
	assert(m_bEnabled);
	CheckGLError();

	GLint iSize;
	GLenum iType;
	GLint iLocation;
	iLocation=glGetUniformLocation(m_hProgram,name); // Position of uniform var

	if (CheckGLError("SetUniformArray(%s,bool*) [getting adress]",name)) return;
	if(iLocation==-1) {
		debugmessage("CProgram::SetUniformArray(");
		debugmessage(name);
		debugmessage(",bool*) -  Error getting address\n");
		return;	
	}

	glGetActiveUniform(m_hProgram,iLocation,0,NULL,&iSize,&iType,NULL);

	if (CheckGLError("SetUniformArray(%s,bool*) [getting type]",name)) return;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
	float *fArray;	
#endif
	int   *iArray;
	switch (iType) {
		case GL_BOOL:
			iArray=new int[iSize];
			for (int i=0; i<iSize; i++) iArray[i]=(a[i] ? 1 : 0);
			glUniform1iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;
		case GL_BOOL_VEC2:
			iArray=new int[2*iSize];
			for (int i=0; i<2*iSize; i++) iArray[i]=(a[i] ? 1 : 0);
			glUniform2iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;
		case GL_BOOL_VEC3:
			iArray=new int[3*iSize];
			for (int i=0; i<3*iSize; i++) iArray[i]=(a[i] ? 1 : 0);
			glUniform3iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;
		case GL_BOOL_VEC4:
			iArray=new int[4*iSize];
			for (int i=0; i<4*iSize; i++) iArray[i]=(a[i] ? 1 : 0);
			glUniform4iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;

#ifdef TUM3D_ALLOW_IMPLICIT_CASTS
		case GL_INT:
			iArray=new int[iSize];
			for (int i=0; i<iSize; i++) iArray[i]=(a[i] ? 1 : 0);
			glUniform1iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;
		case GL_INT_VEC2:
			iArray=new int[2*iSize];
			for (int i=0; i<2*iSize; i++) iArray[i]=(a[i] ? 1 : 0);
			glUniform2iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;
		case GL_INT_VEC3:
			iArray=new int[3*iSize];
			for (int i=0; i<3*iSize; i++) iArray[i]=(a[i] ? 1 : 0);
			glUniform3iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;
		case GL_INT_VEC4:
			iArray=new int[4*iSize];
			for (int i=0; i<4*iSize; i++) iArray[i]=(a[i] ? 1 : 0);
			glUniform4iv(iLocation,iSize,iArray); 
			delete[] iArray;
			break;
		case GL_FLOAT:
			fArray=new float[iSize];
			for (int i=0; i<iSize; i++) fArray[i]=(a[i] ? 1.0f : 0.0f);
			glUniform1fv(iLocation,iSize,fArray); 
			delete[] fArray;
			break;
		case GL_FLOAT_VEC2:
			fArray=new float[2*iSize];
			for (int i=0; i<2*iSize; i++) fArray[i]=(a[i] ? 1.0f : 0.0f);
			glUniform2fv(iLocation,iSize,fArray); 
			delete[] fArray;
			break;
		case GL_FLOAT_VEC3:
			fArray=new float[3*iSize];
			for (int i=0; i<3*iSize; i++) fArray[i]=(a[i] ? 1.0f : 0.0f);
			glUniform3fv(iLocation,iSize,fArray);
			delete[] fArray;
			break;
		case GL_FLOAT_VEC4:
			fArray=new float[4*iSize];
			for (int i=0; i<4*iSize; i++) fArray[i]=(a[i] ? 1.0f : 0.0f);
			glUniform4fv(iLocation,iSize,fArray);
			delete[] fArray;
			break;
#endif

		default: 
			debugmessage("CProgram::SetUniformArray(");
			debugmessage(name);
			debugmessage(",bool*) - Unknown type\n");
			break;
	}
#ifdef _TUM3D_DEBUG_
	CheckGLError("SetUniformArray(%s,bool*)",name);
#endif
}
