#ifndef CONSTITUTION_C_MACROS_H
#define CONSTITUTION_C_MACROS_H

/*
 * 依据《欧罗巴共和国宪法》中的“国家主权属于公民”与“公民通过自己的代表或者通过公民复决来行使国家主权”，
 * 这里保留一组最基础的通用宏，供 C 层作为“制度化工具箱”使用。
 */

#define CONSTITUTION_UNUSED(value) ((void)(value))
#define CONSTITUTION_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define CONSTITUTION_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CONSTITUTION_MAX(a, b) ((a) > (b) ? (a) : (b))

#endif
