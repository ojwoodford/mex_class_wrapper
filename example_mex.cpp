#include "mex.h"
#include "class_handle.hpp"

// The class that we are interfacing to
class example
{
public:
    example() { mexPrintf("Calling constructor\n"); }
    ~example() { mexPrintf("Calling destructor\n"); }
    void train() { mexPrintf("Calling train\n"); };
    void test() { mexPrintf("Calling test\n"); };
private:
};

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{	
    // Get the command string
    char cmd[64];
	if (nrhs < 1 || mxGetString(prhs[0], cmd, sizeof(cmd)))
		mexErrMsgTxt("First input should be a command string less than 64 characters long.");
        
    // New
    if (!strcmp("new", cmd)) {
        // Check parameters
        if (nlhs != 1)
            mexErrMsgTxt("New: One output expected.");
        // Return a handle to a new C++ instance
        plhs[0] = convertPtr2Mat<example>(new example);
        return;
    }
    
    // Check there is a second input, which should be the class instance handle
    if (nrhs < 2)
		mexErrMsgTxt("Second input should be a class instance handle.");
    
    // Delete
    if (!strcmp("delete", cmd)) {
        // Destroy the C++ object
        destroyObject<example>(prhs[1]);
        // Warn if other commands were ignored
        if (nlhs != 0 || nrhs != 2)
            mexWarnMsgTxt("Delete: Unexpected arguments ignored.");
        return;
    }
    
    // Get the class instance pointer from the second input
    example* example_instance = convertMat2Ptr<example>(prhs[1]);
    
    // Call the various class methods
    // Train    
    if (!strcmp("train", cmd)) {
        // Check parameters
        if (nlhs < 0 || nrhs < 2)
            mexErrMsgTxt("Train: Unexpected arguments.");
        // Call the method
        example_instance->train();
        return;
    }
    // Test    
    if (!strcmp("test", cmd)) {
        // Check parameters
        if (nlhs < 0 || nrhs < 2)
            mexErrMsgTxt("Test: Unexpected arguments.");
        // Call the method
        example_instance->test();
        return;
    }
    
    // Got here, so command not recognized
    mexErrMsgTxt("Command not recognized.");
}
