/* 
 * File:   myCommon.h
 * Author: takeshi
 *
 * Created on April 4, 2016, 8:37 PM
 */

#ifndef MYCOMMON_H
#define MYCOMMON_H

/***
 * Const values
 ***/

/***
 * Macros
 ***/

/***
 * Enum definitions
 ***/
typedef enum {
	RET_OK            = 0,
	RET_WAR_NO_RESULT = 1,
	RET_ERR           = -1,
	RET_ERR_PRM       = -2,
	RET_ERR_CONFLICT  = -3,
	RET_ERR_REGISTER  = -4,
} RET;

#endif /* MYCOMMON_H */

