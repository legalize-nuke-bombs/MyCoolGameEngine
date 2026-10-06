#ifndef MYCOOLGAMEENGINE_API_H
#define MYCOOLGAMEENGINE_API_H

#if defined(_WIN32) && defined(MCGE_EXPORT)
#define MCGE_API __declspec(dllexport)
#else
#define MCGE_API
#endif

#endif //MYCOOLGAMEENGINE_API_H
