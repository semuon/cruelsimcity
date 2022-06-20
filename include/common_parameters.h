#ifndef COMMON_PARAMETERS_H_
#define COMMON_PARAMETERS_H_

#include "common.h"

const double c_flt_epsilon = __FLT_EPSILON__;

extern std::string pLogsDirPath;
extern std::string pDataDirPath;
extern std::string pPrefix;
extern bool pOverwriteLogs;

extern bool pSetSeed;
extern int pSeed;

extern int pNthreads;

extern DataDir pLogsDir;
extern DataDir pDataDir;

#endif /* COMMON_PARAMETERS_H_ */
