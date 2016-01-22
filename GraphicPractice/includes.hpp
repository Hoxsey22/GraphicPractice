/*
includes.hpp

Description:

Author: Joseph Hoxsey

*/

# define __INCLUDES__
# define GLM_FORCE_PURE  // for latest version of GLM w/ error C2719: ,,,  __declspec(align('16')) won't be aligned
# define BUFFER_OFFSET(x)  ((const GLvoid *) (x))  // OpenGL PG 8th ed. code, in vgl.h 
# define MAX_INFO_LOG_SIZE 2048  // for error messages in loadShaders(...)

# include <stdio.h>  // My examples use printf, I'm not a fan of cin, cout...
# include <stdlib.h>
# include <sys/stat.h>

# ifdef __Linux__
# include <GL/glew.h>
# include <GL/freeglut.h>
// include the glm shader-like math library
# define GLM_FORCE_RADIANS  // use radians not angles
# define GLM_MESSAGES   // compiler messages
# include <glm/glm.hpp>
# include <glm/gtc/constants.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "glmUtils465.hpp"  // print matrices and vectors, ... 
# include "shader465.hpp"    // load vertex and fragment shaders
# include "triModel465.hpp"  // load AC3D *.tri model 
# endif

# ifdef __MinGW__
# endif

# ifdef __Windows__
# include <Windows.h>  
# include <GL/glew.h>
# include <GL/freeglut.h>
// include the glm shader-like math library
# define GLM_FORCE_RADIANS  // use radians not angles
# define GLM_MESSAGES   // compiler messages
# include <glm/glm.hpp>
# include <glm/gtc/constants.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "glmUtils465.hpp"  // print matrices and vectors, ... 
# include "shader465.hpp"    // load vertex and fragment shaders
# include "triModel465.hpp"  // load AC3D *.tri model
# include "texture.hpp"  // load texture
# endif

# ifdef __Xcode__      
# include <GLUT/glut.h>
// include the glm shader-like math library
# define GLM_FORCE_RADIANS  // use radians not angles
# define GLM_MESSAGES   // compiler messages
# include <glm/glm.hpp>
# include <glm/gtc/constants.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "../includes465/glmUtils465.hpp"  // print matrices and vectors, ... 
# include "../includes465/shader465.hpp"    // load vertex and fragment shaders
# include "../includes465/triModel465.hpp"  // load AC3D *.tri model 
# endif

const float PI = 3.14159f;