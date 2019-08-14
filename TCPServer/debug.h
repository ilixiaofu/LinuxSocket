#ifndef __DEBUG__
#define __DEBUG__

#include <cstdio>

#define log(stream, msg)(fprintf(stream, "%s@line%d:%s\n", __func__, __LINE__, msg))
#define logm(stream, format, m)(fprintf(stream, "%s@line%d:"#format"\n", __func__, __LINE__, m))
#define logi(stream, format, i)(fprintf(stream, "%s@line%d:"#format"\n", __func__, __LINE__, i))
#define logmi(stream, format, msg, i)(fprintf(stream, "%s@line%d:"#format"\n", __func__, __LINE__, msg, i))

#endif // !DEBUG