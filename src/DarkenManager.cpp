#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GlTools.h"
#include "clTools.h"
#include "DarkenManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <stdexcept>

DarkenManager::DarkenManager()
{
}

void 
DarkenManager::setupGL(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(20, 20);
  glutInitWindowSize(WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
  glutInitContextVersion (3, 2); 
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG); 

  glutCreateWindow( argv[0] );

  glewInit();
    glewExperimental = GL_TRUE; // Required in openGL 3.1 forward compatible contexts because
    // GL_EXTENSIONS is deprecated argument for glGetString
    GLenum glewErr = glewInit();
    if(glewErr != GLEW_OK)
    {
        std::stringstream err;
        err << "Error initializing GLEW: " << glewGetErrorString(glewErr);
        throw std::runtime_error(err.str());
    }
    glGetError(); // glewInit finner ikke extension stringen
    // men setter funksjonspekere for core funksjoner

}


void 
DarkenManager::setupData()
{
  cl_int errNum;

  glGenTextures( 1, &m_tex );
  glBindTexture( GL_TEXTURE_2D, m_tex );
  glTexImage2D(GL_TEXTURE_2D, 0, 
               GL_RGBA, WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, 0, GL_RGBA, 
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glBindTexture( GL_TEXTURE_2D, 0 );
  ASSERT_GL;
  
  // create buffer object
  glGenBuffers(1, &m_src_pbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_src_pbo);
  glBufferData(GL_ARRAY_BUFFER, 4*WINDOW_SIZE_WIDTH*WINDOW_SIZE_HEIGHT*sizeof(GLubyte), NULL, GL_DYNAMIC_DRAW);

  glGenBuffers(1, &m_dst_pbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_dst_pbo);
  glBufferData(GL_ARRAY_BUFFER, 4*WINDOW_SIZE_WIDTH*WINDOW_SIZE_HEIGHT*sizeof(GLubyte), NULL, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);


    // --- create FBO for render-to-texture -----------------------------------
  glGenFramebuffers( 1, &m_fbo );
  glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );
  glFramebufferTexture2D( GL_FRAMEBUFFER,
                          GL_COLOR_ATTACHMENT0,
                          GL_TEXTURE_2D,
                          m_tex,
                          0 );
  glDrawBuffer( GL_COLOR_ATTACHMENT0 );
  ASSERT_FBO;
  glBindFramebuffer( GL_FRAMEBUFFER, 0 );

  // Setup cl buffer to pont to gl buffer
  m_cl_src_buffer = clCreateFromGLBuffer(m_context, CL_MEM_READ_ONLY, 
                                     m_src_pbo, &errNum);
  ASSERT_CL(errNum);
  m_cl_dst_buffer = clCreateFromGLBuffer(m_context, CL_MEM_WRITE_ONLY, 
                                     m_dst_pbo, &errNum);
  ASSERT_CL(errNum);

  // setup vbos
	GLfloat quad[4*3] = {
	-1, 1, -0.5,
	-1, -1, -0.5, 
	1, 1, -0.5, 
	1, -1, -0.5, 
	};

	glGenVertexArrays( 1, &m_quad_vao );
    glBindVertexArray( m_quad_vao );
  	glGenBuffers( 1, &m_quad_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_quad_vbo );
	glBufferData( GL_ARRAY_BUFFER,
				  sizeof(GLfloat)*3*4,
				  &quad[0],
				  GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, (GLvoid*)(sizeof(GLfloat)*0) );
	glEnableVertexAttribArray( 0 );
	ASSERT_GL;

	GLfloat triangle[3*3] = {
	-0.5, 0.5, -0.5,
	-0.5, -0.5, -0.5, 
	0.5, 0.5, -0.5, 
	};
	glGenVertexArrays( 1, &m_geom_vao );
    glBindVertexArray( m_geom_vao );
  	glGenBuffers( 1, &m_geom_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_geom_vbo );
	glBufferData( GL_ARRAY_BUFFER,
				  sizeof(GLfloat)*3*3,
				  &triangle[0],
				  GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, (GLvoid*)(sizeof(GLfloat)*0) );
	ASSERT_GL;
	glEnableVertexAttribArray( 0 );
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	ASSERT_GL;

  // --- set up state -------------------------------------------------------
  glDisable( GL_CULL_FACE );
  glDisable( GL_DEPTH_TEST );
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

void
DarkenManager::setupShaders()
{
  m_const_col_prog = glCreateProgram();
  GLuint vs;
  GLuint fs;

  vs = GlTools::compileShaderFromFile( "../glsl/constCol.vert", GL_VERTEX_SHADER);
  fs = GlTools::compileShaderFromFile( "../glsl/constCol.frag", GL_FRAGMENT_SHADER);

  glAttachShader( m_const_col_prog, vs);
  glAttachShader( m_const_col_prog, fs);
  glBindAttribLocation( m_const_col_prog, 0, "in_v_TexCoord" );
  GlTools::linkProgram( m_const_col_prog );
  glUseProgram(m_const_col_prog);
  GLint col_loc = glGetUniformLocation(m_const_col_prog, "color");
  glUniform4f(col_loc, 0.5f, 0.9f, 0.0f, 1.0f);

  m_tex_prog = glCreateProgram();
  fs = GlTools::compileShaderFromFile( "../glsl/tex.frag", GL_FRAGMENT_SHADER);
  glAttachShader( m_tex_prog, vs);
  glAttachShader( m_tex_prog, fs);
  glBindAttribLocation( m_tex_prog, 0, "in_v_TexCoord" );
  GlTools::linkProgram( m_tex_prog );
  glUseProgram(m_tex_prog);
  GLint tex_loc = glGetUniformLocation(m_tex_prog, "tex");
  glUniform1i(col_loc, 0);


  glUseProgram(0);
}

void
DarkenManager::setupCLprog()
{
	cl_int errNum;
	std::string fileName("../OpenCL/darken.cl");
	std::ifstream file(fileName.c_str() );
	if (!file)
	{
	  std::stringstream s;
	  s << "error opening " << fileName << " in " 
		<< __FILE__ << " at " << __LINE__ << std::endl;
	  throw std::runtime_error(s.str());
	}
    std::string source = std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
    const char *souce_sting=source.c_str();
    
    m_darken_program=clCreateProgramWithSource(m_context,
                              1,
                              &souce_sting, NULL, &errNum);
    ASSERT_CL(errNum);
    errNum=clBuildProgram(m_darken_program, 0, NULL, "-cl-fast-relaxed-math",
                          NULL, NULL);
    if (errNum==CL_BUILD_PROGRAM_FAILURE)
    {
      size_t logSize;
      errNum = clGetProgramBuildInfo(m_darken_program, m_id,
                                CL_PROGRAM_BUILD_LOG,
                                0, NULL, &logSize);
      ASSERT_CL(errNum);
      std::vector<char> log_vec;
      log_vec.resize(logSize);
      errNum = clGetProgramBuildInfo(m_darken_program, m_id,
                                CL_PROGRAM_BUILD_LOG,
                                logSize, &log_vec[0], &logSize);
      ASSERT_CL(errNum);
      std::string log(&log_vec[0]);
      std::string errMessage("Error compiling: ");
      errMessage += fileName;
      errMessage += "\n";
      errMessage += log;
      std::cout << errMessage << std::endl;
      throw std::runtime_error(errMessage);
    }
    ASSERT_CL(errNum);
//	m_darken_kernel = clCreateKernel(m_darken_program, "darken", &errNum);
//	m_darken_kernel = clCreateKernel(m_darken_program, "average_NO_LOCAL", &errNum);
	m_darken_kernel = clCreateKernel(m_darken_program, "average", &errNum);
	ASSERT_CL(errNum);
	errNum = clSetKernelArg(m_darken_kernel, 0, sizeof(m_cl_src_buffer), &m_cl_src_buffer);
	errNum |= clSetKernelArg(m_darken_kernel, 1, sizeof(m_cl_dst_buffer), &m_cl_dst_buffer);
	cl_int w = WINDOW_SIZE_WIDTH;
	cl_int h = WINDOW_SIZE_HEIGHT;
	errNum |= clSetKernelArg(m_darken_kernel, 2, sizeof(cl_int), &w);
	errNum |= clSetKernelArg(m_darken_kernel, 3, sizeof(cl_int), &h);
	errNum |= clSetKernelArg(m_darken_kernel, 4, 
		(WORKGROUP_DIM_X+2)*(WORKGROUP_DIM_Y+2)*4, NULL);
	ASSERT_CL(errNum);


}


void 
DarkenManager::init(int argc, char **argv)
{
	setupGL(argc, argv);

	clTools::initOpenCL (m_context, m_id, m_queue, "", true, true);
	setupData();
	setupCLprog();
	setupShaders();

	QueryPerformanceFrequency((LARGE_INTEGER*)&m_ticksPerSecond);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_start_ticks);
	m_frames = 0;

}

void
DarkenManager::render()
{
	m_frames++;
	{
		UINT64 ticks;
		QueryPerformanceCounter((LARGE_INTEGER*)&ticks);
		UINT64 curr_ticks = ticks - m_prev_fps_ticks;
		double t = (double)curr_ticks/m_ticksPerSecond;

		if (t >= 1)
		{
			float fps = (float)(m_frames / ((t)));
			std::stringstream caption;
			caption << "cl_gl@" << (int)fps << "fps"; 
			glutSetWindowTitle(caption.str().c_str());
			m_prev_fps_ticks = ticks;
			m_frames = 0;
		}
	}
	glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );
    glClearColor ( 0.2f, 0.2f, 0.2f, 0.0f );
	glClear ( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(m_const_col_prog);
	glBindVertexArray( m_geom_vao );
	glDrawArrays(GL_TRIANGLES, 0, 3 );
	// do OpenCL
	postProcess();

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glClearColor ( 0.2f, 0.0f, 0.0f, 0.0f );
	glClear ( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(m_tex_prog);
	glBindVertexArray( m_quad_vao );
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4 );
	glUseProgram(0);
}

void DarkenManager::postProcess()
{
	cl_int errNum;
    // activate destination buffer
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, m_src_pbo);

    //// read data into pbo. note: use BGRA format for optimal performance
    glReadPixels(0, 0, WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, NULL); 
	glFlush();
	clEnqueueAcquireGLObjects(m_queue, 1, &m_cl_src_buffer, 0, NULL, NULL);
	clEnqueueAcquireGLObjects(m_queue, 1, &m_cl_dst_buffer, 0, NULL, NULL);

    size_t *global_work_offset = NULL;
	size_t global_work_size[] = {WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT};
    size_t local_work_size[] = {WORKGROUP_DIM_X, WORKGROUP_DIM_Y};
	if(1)
	{
		errNum=clEnqueueNDRangeKernel(m_queue, m_darken_kernel,
                                  2, 
                                  global_work_offset,
                                  global_work_size,
                                  local_work_size,
                                  0,
                                  NULL,
                                  NULL);
		ASSERT_CL(errNum);
	}
	else
	{
		std::vector<unsigned char> tmp(WINDOW_SIZE_HEIGHT*WINDOW_SIZE_WIDTH*4, 0);
		clEnqueueWriteBuffer(m_queue, m_cl_dst_buffer, CL_TRUE,
			0, WINDOW_SIZE_HEIGHT*WINDOW_SIZE_HEIGHT*4, &tmp[0],
                       0, NULL, NULL);
	}

    clEnqueueReleaseGLObjects(m_queue,1, &m_cl_src_buffer, 0, NULL, NULL);
    clEnqueueReleaseGLObjects(m_queue,1, &m_cl_dst_buffer, 0, NULL, NULL);
	clFinish(m_queue);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, m_dst_pbo);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
		WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, 
                    GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	ASSERT_GL;

}
