#ifndef OPENCLERROR_HPP
# define OPENCLERROR_HPP

# include <string>
# include <iostream>
# include "OpenCLInc.hpp"

void			checkCLError(cl_int err, std::string action);

#endif
