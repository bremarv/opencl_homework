#ifndef _CLTOOLS_H_
#define _CLTOOLS_H_

#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <assert.h>

#include <CL/cl.h>

#if defined(__gl_h_) || defined(__GL_H__) //gl included
#ifdef _WIN32
#include <wingdi.h>
#else // Linux code
  #include <GL/glx.h>
#endif
  #include <CL/cl_gl.h>
#endif
#define ASSERT_CL(assert_cl_err_num) do { clTools::assertCL(assert_cl_err_num, __FILE__, __LINE__ ); } while(0)

namespace clTools
{
inline
void Tokenize(std::string str,
              std::vector<std::string> &tokens,
              const std::string &delimiters = " ");

inline
  bool extensionSupported(cl_platform_id platform_id, std::string ext);


inline 
  std::string clErrNumToString(cl_int errNum);

inline
  void assertCL(cl_int errNum, std::string fname, int line);

inline
void getDeviceInfo(std::vector<char> &text, cl_device_id id,
                   cl_device_info pname);

inline
  bool checkGLInterop(cl_platform_id platform_id);

inline
  bool deviceSupport(cl_device_id id, std::string required_extensions);


inline
void getDeviceIDs (std::vector<cl_device_id> &device_id,
                   std::string required_extensions, 
                   bool openGL,
                   bool verbose=false);


inline
void initOpenCL (cl_context &context,
                 cl_device_id &id,
                 cl_command_queue &queue,
                 std::string required_extensions, 
                 bool openGL,
                 bool verbose=false);


} //namespace clTools
#include<clTools_impl.h>

#endif // _CLTOOLS_H_
