#include <OpenCL/opencl.h>
# include <vector>
# include <string>
# include <fstream>
# include <iostream>

# include "Timer.hpp"

void			getPlatformInfo(std::vector<cl_platform_id> platformId)
{
	const cl_platform_info attributeTypes[5] = { CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION, CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS };
	char* info;
	size_t infoSize;
	const char* attributeNames[5] = { "Name", "Vendor", "Version", "Profile", "Extensions" };
	const int attributeCount = sizeof(attributeNames) / sizeof(char*);

	for (cl_platform_id p : platformId) {
		std::cout << "Platform id: " << p << std::endl;
		for (int j = 0; j < attributeCount; j++) {	
			clGetPlatformInfo(p, attributeTypes[j], 0, NULL, &infoSize);
			info = (char*) malloc(infoSize);
			clGetPlatformInfo(p, attributeTypes[j], infoSize, info, NULL);
			std::cout << "\t" << attributeNames[j] << ":\t" << info << std::endl;
		}
		std::cout << std::endl;
	}
}

void			getDeviceInfo(std::vector<cl_device_id> deviceId)
{ 
	char*		value;
	size_t		valueSize;
	cl_uint		maxComputeUnits;

	for (cl_device_id d : deviceId) {
		std::cout << "Device id: " << d << std::endl;

		// print device name
		clGetDeviceInfo(d, CL_DEVICE_NAME, 0, NULL, &valueSize);
		value = (char*) malloc(valueSize);
		clGetDeviceInfo(d, CL_DEVICE_NAME, valueSize, value, NULL);
		std::cout << "\tDevice name: " << value << std::endl;
		free(value);

		// print hardware device version
		clGetDeviceInfo(d, CL_DEVICE_VERSION, 0, NULL, &valueSize);
		value = (char*) malloc(valueSize);
		clGetDeviceInfo(d, CL_DEVICE_VERSION, valueSize, value, NULL);
		std::cout << "\tHardware version: " << value << std::endl;
		free(value);

		// print software driver version
		clGetDeviceInfo(d, CL_DRIVER_VERSION, 0, NULL, &valueSize);
		value = (char*) malloc(valueSize);
		clGetDeviceInfo(d, CL_DRIVER_VERSION, valueSize, value, NULL);
		std::cout << "\tSoftware version: " << value << std::endl;
		free(value);

		// print c version supported by compiler for device
		clGetDeviceInfo(d, CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
		value = (char*) malloc(valueSize);
		clGetDeviceInfo(d, CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
		std::cout << "\tOpenCL C version: " << value << std::endl;
		free(value);

		// print parallel compute units
		clGetDeviceInfo(d, CL_DEVICE_MAX_COMPUTE_UNITS,
				sizeof(maxComputeUnits), &maxComputeUnits, NULL);
		std::cout << "\tParallel compute units: " << maxComputeUnits << std::endl << std::endl;
	}
}

void			check_error(cl_int err, std::string action)
{
	if (err != CL_SUCCESS)
	{
		std::cout << "error: " << action << " with code " << err << std::endl;
		exit(0);
	}
}

std::string			*getSourceContent(std::string filename)
{
	std::ifstream	ifs(filename);
	std::string		*content;

	content = new std::string(std::istreambuf_iterator<char>(ifs),
							  std::istreambuf_iterator<char>());
	return content;
}

int				main()
{
	cl_uint									platformIdCount = 0;
	cl_int									error;
	std::string								*content;
	const char								*content_str;
	size_t									local_size = 4;
	size_t									global_size = 10000;
	Timer									t;

	clGetPlatformIDs(0, nullptr, &platformIdCount);

	std::vector<cl_platform_id>				platformIds(platformIdCount);
	clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);
//	getPlatformInfo(platformIds);

	cl_uint									deviceIdCount = 0;
	clGetDeviceIDs(platformIds [0], CL_DEVICE_TYPE_GPU, 0, nullptr, &deviceIdCount);

	std::vector<cl_device_id>				deviceIds(deviceIdCount);
	clGetDeviceIDs(platformIds [0], CL_DEVICE_TYPE_GPU, deviceIdCount, deviceIds.data(), nullptr);

//	getDeviceInfo(deviceIds);

	const cl_context_properties contextProperties [] =
	{
		CL_CONTEXT_PLATFORM,
		reinterpret_cast<cl_context_properties> (platformIds [0]),
		0, 0
	};

	// Context
	cl_context context = clCreateContext (
			contextProperties, deviceIdCount,
			deviceIds.data (), nullptr,
			nullptr, &error);
	check_error(error, "building context");

	// Program
	content = getSourceContent("program.cl");
	content_str = content->c_str();
	cl_program program = clCreateProgramWithSource(context, 1, &content_str, NULL, &error);
	check_error(error, "creating program");
	error = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (error != CL_SUCCESS) {
		char    buildlog[16384];
		clGetProgramBuildInfo(program, deviceIds[0], CL_PROGRAM_BUILD_LOG, sizeof(buildlog), buildlog, NULL);
		std::cerr << "Error on compilation(" << "program.cl" << "):" << std::endl;
		std::cerr << buildlog << std::endl;
	}

	// Kernel
	cl_kernel kernel = clCreateKernel(program, "test", &error);
	check_error(error, "creating kernel");

	// Command Queue
	cl_command_queue queue = clCreateCommandQueue(context, deviceIds[0], 0, &error);
	check_error(error, "creating command queue");

	// Buffer
	std::vector<float>in(10000);
	std::vector<float>out(10000);
	for (int i = 0 ; i < 10000 ; i++) {
		in[i] = 2.0f;
	}

	cl_mem inBuf = clCreateBuffer (context,
			CL_MEM_COPY_HOST_PTR,
			sizeof(float) * (in.size()),
			in.data(), &error);
	check_error(error, "creating buffer");

	// Kernel arg
	error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &inBuf);
	check_error(error, "setting kernel arg");

	t.start();
	// Enqueue kernel
	error = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, 
			NULL, 0, NULL, NULL); 
	check_error(error, "enqueuing kernel");

	// Enqueu Read
	error = clEnqueueReadBuffer(queue, inBuf, CL_TRUE, 0, 
			sizeof(float)*10000, out.data(), 0, NULL, NULL);
	t.stop();
	std::cout << "took " << t.getTime() << std::endl;

	for (int i = 0; i < 50 ; i++)
		std::cout << out[i] << std::endl;

	return (0);
}
