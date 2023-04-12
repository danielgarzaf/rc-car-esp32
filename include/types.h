#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define UINT8_MAX 255
#define INT8_MAX 127


typedef uint8_t   u8;
// typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float  f32;
typedef double f64;

enum class DeviceState {
    Initialized,
    Uninitialized,
};

enum Colors {
        /** r = 255, g = 0, b = 0 */
        Red = 0xFF0000,
        /** r = 0, g = 255, b = 0 */
        Green = 0xFF00,
        /** r = 0, g = 0, b = 255 */
        Blue = 0xFF,

        /** r = 255, g = 235, b = 4 */
        Yellow = 0xFFEB04,
        /** r = 0, g = 255, b = 255 */
        Lightblue = 0xFFFF,
        /** r = 255, g = 0, b = 255 */
        Purple = 0xFF00FF,
        Purble = 0xFF00FF,

        /** r = 255, g = 255, b = 255 */
        White = 0xFFFFFF,
        /** r = 0, g = 0, b = 0 */
        Off = 0x00,
};

typedef struct StickPosition {
    u8 x = 0;
    u8 y = 0;
} StickPosition;

#endif
