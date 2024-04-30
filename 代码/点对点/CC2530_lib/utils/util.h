//-------------------------------------------------------------------
// Filename: util.h
// Description: Utility library header file
//-------------------------------------------------------------------
#ifndef _UTIL_H_
#define _UTIL_H_
//-------------------------------------------------------------------
// TYPEDEFS
//-------------------------------------------------------------------
typedef struct
{
    const char *szDescr; // Textual description
    const uint8 value; // Value
} menuItem_t;

typedef struct
{
    const menuItem_t *pMenuItems;
    const uint8 nItems;
} menu_t;

//-------------------------------------------------------------------
// CONSTANTS AND DEFINES
//-------------------------------------------------------------------
// Number of elements in an array
#define N_ITEMS(arr)                sizeof(arr)/sizeof(arr[0])

//-------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-------------------------------------------------------------------
char *convInt32ToText(int32 value);
const char *utilChipIdToStr(uint8 chipID);
void utilReverseBuf(uint8 *pBuf, uint8 length);

/*!
 * \brief Initializes the pseudo random generator initial value
 *
 * \param [IN] seed Pseudo random generator initial value
 */
void srand1( uint32 seed );

/*!
 * \brief Computes a random number between min and max
 *
 * \param [IN] min range minimum value
 * \param [IN] max range maximum value
 * \retval random random value in range min..max
 */
int32 randr( int32 min, int32 max );

#endif
