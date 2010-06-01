namespace clTools
{
inline
 void breakpnt()
 {
   int i =0;
   i++;
 }

inline
void Tokenize(std::string str,
              std::vector<std::string> &tokens,
              const std::string &delimiters )
{
  size_t last_pos = str.find_first_not_of(delimiters, 0);
  size_t pos = str.find_first_of(delimiters, last_pos);

  //  while(last_pos!=string::npos)
  while(pos!=std::string::npos || last_pos!=std::string::npos)
  {
    tokens.push_back(str.substr(last_pos, pos-last_pos));
    last_pos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, last_pos);
  }
}

 

inline
  bool extensionSupported(cl_platform_id platform_id, std::string ext)
{
  cl_int errNum;
  size_t stringLen;
  std::vector<char> text_vec;
  errNum = clGetPlatformInfo(platform_id, CL_PLATFORM_EXTENSIONS,
                             0, NULL, &stringLen);
  assert(errNum == CL_SUCCESS);
  text_vec.resize(stringLen);
  errNum = clGetPlatformInfo(platform_id, CL_PLATFORM_EXTENSIONS,
                             stringLen, &text_vec[0], &stringLen);
  assert(errNum == CL_SUCCESS);
  std::string text(&text_vec[0]);

  std::vector<std::string> extensions;
  Tokenize(text, extensions);
  for (size_t i=0; i<extensions.size(); i++)
  {
    if (extensions[i]==ext)
      return true;
  }
  return false;
}


inline 
  std::string clErrNumToString(cl_int errNum)
{
  std::string err;
  switch(errNum)
  {
  case CL_SUCCESS:
    break;
  case CL_DEVICE_NOT_FOUND:
    err = "CL_DEVICE_NOT_FOUND";
      break;
    case CL_DEVICE_NOT_AVAILABLE:
      err = "CL_DEVICE_NOT_AVAILABLE";
      break;
    case CL_COMPILER_NOT_AVAILABLE:
      err = "CL_COMPILER_NOT_AVAILABLE"; 
      break;
    case  CL_MEM_OBJECT_ALLOCATION_FAILURE:
      err = "CL_MEM_OBJECT_ALLOCATION_FAILURE";
      break;
    case  CL_OUT_OF_RESOURCES:
      err = "CL_OUT_OF_RESOURCES";
      break;
    case CL_OUT_OF_HOST_MEMORY:
      err = "CL_OUT_OF_HOST_MEMORY";
      break;
    case CL_PROFILING_INFO_NOT_AVAILABLE:
      err = "CL_PROFILING_INFO_NOT_AVAILABLE";
      break;
    case CL_MEM_COPY_OVERLAP:
      err = "CL_MEM_COPY_OVERLAP";
      break;
    case CL_IMAGE_FORMAT_MISMATCH:
      err = "CL_IMAGE_FORMAT_MISMATCH";
      break;
    case CL_IMAGE_FORMAT_NOT_SUPPORTED:
      err = "CL_IMAGE_FORMAT_NOT_SUPPORTED";
      break;
    case CL_BUILD_PROGRAM_FAILURE:
      err = "CL_BUILD_PROGRAM_FAILURE";
      break;
    case CL_MAP_FAILURE:
      err = "CL_MAP_FAILURE";
      break;
    case CL_INVALID_VALUE:
      err = "CL_INVALID_VALUE";
      break;
    case CL_INVALID_DEVICE_TYPE:
      err = "CL_INVALID_DEVICE_TYPE";
      break;
    case CL_INVALID_PLATFORM:
      err = "CL_INVALID_PLATFORM";
      break;
    case CL_INVALID_DEVICE:
      err = "CL_INVALID_DEVICE";
      break;
    case CL_INVALID_CONTEXT:
      err = "CL_INVALID_CONTEXT";
      break;
    case CL_INVALID_QUEUE_PROPERTIES:
      err = "CL_INVALID_QUEUE_PROPERTIES";
      break;
    case CL_INVALID_COMMAND_QUEUE:
      err = "CL_INVALID_COMMAND_QUEUE";
      break;
    case CL_INVALID_HOST_PTR:
      err = "CL_INVALID_HOST_PTR";
      break;
    case CL_INVALID_MEM_OBJECT:
      err = "CL_INVALID_MEM_OBJECT";
      break;
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
      err = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
      break;
    case CL_INVALID_IMAGE_SIZE:
      err = "CL_INVALID_IMAGE_SIZE";
      break;
    case CL_INVALID_SAMPLER:
      err = "CL_INVALID_SAMPLER";
      break;
    case CL_INVALID_BINARY:
      err = "CL_INVALID_BINARY";
      break;
    case CL_INVALID_BUILD_OPTIONS:
      err = "CL_INVALID_BUILD_OPTIONS";
      break;
    case CL_INVALID_PROGRAM:
      err = "CL_INVALID_PROGRAM";
      break;
    case CL_INVALID_PROGRAM_EXECUTABLE:
      err = "CL_INVALID_PROGRAM_EXECUTABLE";
      break;
    case CL_INVALID_KERNEL_NAME:
      err = "CL_INVALID_KERNEL_NAME";
      break;
    case CL_INVALID_KERNEL_DEFINITION:
      err = "CL_INVALID_KERNEL_DEFINITION";
      break;
    case CL_INVALID_KERNEL:
      err = "CL_INVALID_KERNEL";
      break;
    case CL_INVALID_ARG_INDEX:
      err = "CL_INVALID_ARG_INDEX";
      break;
    case CL_INVALID_ARG_VALUE:
      err = "CL_INVALID_ARG_VALUE";
      break;
    case CL_INVALID_ARG_SIZE:
      err = "CL_INVALID_ARG_SIZE";
      break;
    case CL_INVALID_KERNEL_ARGS:
      err = "CL_INVALID_KERNEL_ARGS";
      break;
    case CL_INVALID_WORK_DIMENSION:
      err = "CL_INVALID_WORK_DIMENSION";
      break;
    case CL_INVALID_WORK_GROUP_SIZE:
      err = "CL_INVALID_WORK_GROUP_SIZE";
      break;
    case CL_INVALID_WORK_ITEM_SIZE:
      err = "CL_INVALID_WORK_ITEM_SIZE";
      break;
    case CL_INVALID_GLOBAL_OFFSET:
      err = "CL_INVALID_GLOBAL_OFFSET";
      break;
    case CL_INVALID_EVENT_WAIT_LIST:
      err = "CL_INVALID_EVENT_WAIT_LIST";
      break;
    case CL_INVALID_EVENT:
      err = "CL_INVALID_EVENT";
      break;
    case CL_INVALID_OPERATION:
      err = "CL_INVALID_OPERATION";
      break;
    case CL_INVALID_GL_OBJECT:
      err = "CL_INVALID_GL_OBJECT";
      break;
    case CL_INVALID_BUFFER_SIZE:
      err = "CL_INVALID_BUFFER_SIZE";
      break;
    case CL_INVALID_MIP_LEVEL:
      err = "CL_INVALID_MIP_LEVEL";
      break;
    case CL_INVALID_GLOBAL_WORK_SIZE:
      err = "CL_INVALID_GLOBAL_WORK_SIZE";
      break;
  default:
    err="unknown";
  }
  return err;
    
}

inline
void assertCL(cl_int errNum, std::string fname, int line)
{
  if (errNum != CL_SUCCESS)
  {
    std::stringstream ss;
    std::string errString = clErrNumToString(errNum);
    ss << fname << ":" << line << "\n";
    ss << "OpenCL error: ";
    ss << errNum << " " << errString;
    throw std::runtime_error(ss.str());
  }
}

inline
void getDeviceInfo(std::vector<char> &text, cl_device_id id,
                   cl_device_info pname)
{
  cl_int errNum;
  size_t stringLen;
  errNum = clGetDeviceInfo(id, pname,
                           0, NULL, &stringLen);
  ASSERT_CL(errNum);
  text.resize(stringLen);
  errNum = clGetDeviceInfo(id, pname,
                           stringLen, &text[0], &stringLen);
  ASSERT_CL(errNum);

}

inline
bool checkGLInterop(cl_platform_id platform_id)
{
  return extensionSupported(platform_id, "cl_khr_gl_sharing");
}

inline
bool deviceSupport(cl_device_id id, std::string required_extensions)
{
  std::vector<std::string> req_extensions;
  Tokenize(required_extensions, req_extensions);


  std::vector<char> text_vec;
  getDeviceInfo(text_vec, id, CL_DEVICE_EXTENSIONS);
  std::string text(&text_vec[0]);

  std::vector<std::string> extensions;
  Tokenize(text, extensions);

  for (size_t i=0; i<req_extensions.size(); i++)
  {
    bool found=false;
    for (size_t j=0; j<extensions.size(); j++)
    {
      if (req_extensions[i]==extensions[j])
      {
        found = true;
        break;
      }
    }
    if (found==false)
      return false;
  }
  return true;
}


inline
void getDeviceIDs (std::vector<cl_device_id> &device_id,
                   std::string required_extensions, 
                   bool openGL,
                   bool verbose)
{
  cl_int errNum;
  std::vector<cl_platform_id> platform_id;
  cl_uint num_platforms;
  size_t stringLen;
  std::vector<char> text_vec;

  if (required_extensions.size()>30)
    breakpnt();
  errNum =  clGetPlatformIDs(0,
                             NULL,
                             &num_platforms);
  ASSERT_CL(errNum);
  platform_id.resize(num_platforms);
  errNum =  clGetPlatformIDs(num_platforms,
                             &platform_id[0],
                             &num_platforms);
  assert(num_platforms >0);
  for (cl_uint i = 0; i< num_platforms; i++)
  {
    if (verbose)
    {
      errNum = clGetPlatformInfo(platform_id[i], CL_PLATFORM_NAME,
                                 0, NULL, &stringLen);
      ASSERT_CL(errNum);
      text_vec.resize(stringLen);
      errNum = clGetPlatformInfo(platform_id[i], CL_PLATFORM_NAME,
                                 stringLen, &text_vec[0], NULL);
      ASSERT_CL(errNum);
      std::cout << "Platform " << i << " " << &text_vec[0] << std::endl;

      errNum = clGetPlatformInfo(platform_id[i], CL_PLATFORM_EXTENSIONS,
                                 0, NULL, &stringLen);
      ASSERT_CL(errNum);
      text_vec.resize(stringLen);
      errNum = clGetPlatformInfo(platform_id[i], CL_PLATFORM_EXTENSIONS,
                                 stringLen, &text_vec[0], NULL);
      ASSERT_CL(errNum);
      std::cout << "Extensions: " << &text_vec[0] << std::endl;
    }

    {
      std::vector<cl_device_id> curr_devices;
      cl_uint curr_num_devices;
      errNum = clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_GPU, 
                              0,
                              NULL, &curr_num_devices);
      ASSERT_CL(errNum);
      assert(curr_num_devices>0);
      curr_devices.resize(curr_num_devices);
      errNum = clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_GPU, 
                              curr_num_devices, 
                              &curr_devices[0], &curr_num_devices);
      ASSERT_CL(errNum);
      for (cl_uint j = 0; j<curr_num_devices; j++)
      {
        if (verbose)
        {
          getDeviceInfo(text_vec, curr_devices[j], CL_DEVICE_NAME);
          std::cout << "  Device " << j << " " << &text_vec[0] << std::endl;
          getDeviceInfo(text_vec, curr_devices[j], CL_DEVICE_EXTENSIONS);
          std::cout << "   " << &text_vec[0] << std::endl;
		  {			
			  size_t max_work_group_size;
			  errNum = clGetDeviceInfo(curr_devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE,
				  sizeof(size_t), &max_work_group_size, NULL);
			  std::cout << "   CL_DEVICE_MAX_WORK_GROUP_SIZE: " << max_work_group_size << std::endl;
			  ASSERT_CL(errNum);
		  }
		  {
			  cl_ulong max_local_mem_size;
			  errNum = clGetDeviceInfo(curr_devices[j], CL_DEVICE_LOCAL_MEM_SIZE,
				  sizeof(max_local_mem_size), &max_local_mem_size, NULL);
			  std::cout << "   CL_DEVICE_LOCAL_MEM_SIZE: " << max_local_mem_size << std::endl;
			  ASSERT_CL(errNum);
		  }
        }

		if (openGL && !deviceSupport(curr_devices[j], "cl_khr_gl_sharing"))
        {
          device_id.push_back(curr_devices[j]);
          if (verbose)
            std::cout << "    Added\n";
        }

		if (deviceSupport(curr_devices[j], required_extensions))
        {
          device_id.push_back(curr_devices[j]);
          if (verbose)
            std::cout << "    Added\n";
        }
      }
    }
  }

  if (device_id.empty())
  {
    throw std::runtime_error("No OpenCL devices found");
  }
}


inline
void initOpenCL (cl_context &context,
                 cl_device_id &id,
                 cl_command_queue &queue,
                 std::string required_extensions, 
                 bool openGL,
                 bool verbose)
{
  cl_int errNum;
  std::vector<cl_device_id> id_vec;
  if (required_extensions.size()>30)
    breakpnt();
  
  getDeviceIDs (id_vec, required_extensions, openGL, verbose);
  if (required_extensions.size()>30)
    breakpnt();
  assert(id_vec.size()>0);
  id = id_vec[0];
  cl_platform_id platform;
  errNum = clGetDeviceInfo(id, CL_DEVICE_PLATFORM,
                           sizeof(platform), &platform, NULL);
  ASSERT_CL(errNum);

  if (openGL)
  {
#if defined(__gl_h_) || defined(__GL_H__) //gl included
#  ifdef _WIN32
    cl_context_properties props[] = 
    {
        CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), 
        CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), 
        CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 
        0
    };
    context = clCreateContext(props, 1, &id, NULL, NULL, &errNum);
#  else // Linux
    // Start Linux code
    cl_context_properties props[] = 
      {
        CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(), 
        CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(), 
        CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 
        0
      };
    context = clCreateContext(props, 1, &id, NULL, NULL, &errNum);
#    endif
#else
    throw (std::runtime_error("initOpenCL with OpenGL support requires inclusion of gl headers"));
#endif // gl included
  } else
  {
    context = clCreateContext(NULL, 1, &id, NULL, NULL, &errNum);
  }
  ASSERT_CL(errNum);
  queue = clCreateCommandQueue(context,id, 0, &errNum);
  ASSERT_CL(errNum);
}



} //namespace clTools

