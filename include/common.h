#ifndef MEM_MACROS_H
#define MEM_MACROS_H

#define MAX_CUSTOMS 32 // Maximum number of custom songs allowed

/* Maximum number of characters associated with any custom song attribute.
 * This goes for filename, artist name, song display name, etc. */
#define MAX_CUSTOM_LEN 48
#define MAX_FILE_PATH_LEN 256

/* ALLEGREX MACROS */
#define JAL_OPCODE	0x0C000000
#define J_OPCODE	0x08000000

#define MAKE_CALL(f) JAL_OPCODE | (((unsigned int)(f) >> 2)  & 0x03ffffff)
#define MAKE_JUMP(f) (J_OPCODE | (((unsigned int)(f) >> 2)  & 0x03FFFFFF))
#define JUMP_TARGET(i) (0x08000000 | (((unsigned int)(i) & 0x03ffffff) << 2))

#endif