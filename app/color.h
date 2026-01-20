#include <stdint.h>

#define RED          (Color){255, 0, 0, 1}
#define GREEN        (Color){0, 255, 0, 1}
#define BLUE         (Color){0, 0, 255, 1}
#define YELLOW       (Color){255, 255, 0, 1}
#define CYAN         (Color){0, 255, 255, 1}
#define MAGENTA      (Color){255, 0, 255, 1}
#define FUCHSIA      (Color){255, 0, 255, 1}  // Same as magenta

// Neutrals
#define BLACK        (Color){0, 0, 0, 1}
#define WHITE        (Color){255, 255, 255, 1}

// Additional common colors
#define ORANGE       (Color){255, 165, 0, 1}
#define PURPLE       (Color){128, 0, 128, 1}
#define PINK         (Color){255, 192, 203, 1}
#define BROWN        (Color){165, 42, 42, 1}
#define GRAY         (Color){128, 128, 128, 1}
#define LIGHT_GRAY   (Color){211, 211, 211, 1}
#define DARK_GRAY    (Color){64, 64, 64, 1}


typedef struct Color {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
} Color;

/**
 * Color struct with RGBA values.
 *
 * @param R - Red 0 to 255
 * @param G - Green 0 to 255
 * @param B - Blue 0 to 255
 * @param A - Alpha 0 to 255
*/

Color ColorConstructor(
    uint8_t R,
    uint8_t G,
    uint8_t B,
    uint8_t A
);