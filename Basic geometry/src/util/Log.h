#ifndef LOG_H
#define LOG_H

#define ENABLE_LOGGING 0

#define LOG(x) \
if (ENABLE_LOGGING == 1) \
{\
    std::cout << __FUNCTION__\
    << " " /*<< __LINE__ << " "*/\
    << x << std::endl;\
}

#endif // !LOG_H
