#ifndef _DARKENMANAGER_H_
#define _DARKENMANAGER_H_

#include <CL/cl.h>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>

class DarkenManager
{
  public:
    enum {WINDOW_SIZE_WIDTH  = 512, WINDOW_SIZE_HEIGHT = 256,
	  WORKGROUP_DIM_X=16, WORKGROUP_DIM_Y=16};
    DarkenManager();
    void init(int argc, char **argv);
    void render();
    void postProcess();

  private:
    void setupGL(int argc, char **argv);
    void setupData();
    void setupShaders();
    void setupCLprog();


    cl_context m_context;
    cl_device_id m_id;
    cl_command_queue m_queue;

    GLuint m_tex;
    GLuint m_quad_vao; 
    GLuint m_quad_vbo; 
    GLuint m_geom_vao; 
    GLuint m_geom_vbo; 
    GLuint m_fbo;     
    GLuint m_src_pbo;     
    GLuint m_dst_pbo;     

    GLuint  m_const_col_prog;
    GLuint  m_tex_prog;

    cl_mem m_cl_src_buffer;     
    cl_mem m_cl_dst_buffer;  
    cl_program m_darken_program;
    cl_kernel m_darken_kernel;

    int  m_frames;
// windows timer
    UINT64 m_ticksPerSecond;
    UINT64 m_prev_fps_ticks;
    UINT64 m_start_ticks;


};


#endif // _DARKENMANAGER_H_
