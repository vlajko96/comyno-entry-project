#ifndef _TEST_UTILS_H_
#define  _TEST_UTILS_H_
#include <iostream>
#include <cstring>
#include <sstream>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
typedef void (*item_handler_f)(void);

#define T_ARRAY_SIZE(items) static_cast<int>(sizeof(items)/sizeof(items[0]))

typedef struct
{
    const char *name;
    item_handler_f handle;
} t_item_t;

typedef struct
{
    const char *name;
    int item_len;
    t_item_t *item_list;
} t_module_t;


/*-----------------------------------------------------------------------------------------------*/
/** 
  @brief Read a int value from stdin 
  @param[out] val, Return read data
  @return 
  0 - successful
  1 - read an enter 
  -1 - invalid input
  */
/*-----------------------------------------------------------------------------------------------*/
int t_get_int(int *val);

#endif /* _TEST_UTILS_H_ */
