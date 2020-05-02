/* Originally from  MUSL */
#ifndef _STDBOOL_H
#define _STDBOOL_H

#ifndef __cplusplus

#define bool unsigned short
#define true 1
#define false (!true)

#endif

#define __bool_true_false_are_defined 1

#endif
