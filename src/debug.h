/*
 *  debug system
 *
 *  Copyright (C) 2013 Lipeng<runangaozhong@163.com>
 *
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

void PRINT(const char *fmt, ...);

/* 
 * debug control, you can switch on this (delete 'x' suffix)
 * to enable log output and assert mechanism
 */
#define CONFIG_ENABLE_DEBUG

/*
 * default, compiler use uart_puts etc. for 
 * printf, if we need use ITM debug,
 * should re-direct these interface to ITM_SendChar
 */
#define CONFIG_DEBUG_USE_ITMx
#define CONFIG_DEBUG_USE_UART

#ifdef CONFIG_ENABLE_DEBUG
    /*===========================================
     *          internal definition
     *=========================================*/
    
    /* debug level,
     * if is DEBUG_LEVEL_DISABLE, on log is allowed output,
     * if is DEBUG_LEVEL_ERR, only AMBE_ERR is allowed output,
     * if is DEBUG_LEVEL_INFO, AMBE_ERR and AMBE_INFO are allowed output,
     * if is DEBUG_LEVEL_DEBUG, all log are allowed output,
     */
    enum debug_level {
        DEBUG_LEVEL_DISABLE = 0,
        DEBUG_LEVEL_ERR,
        DEBUG_LEVEL_INFO,
        DEBUG_LEVEL_DEBUG
    };

    #define ASSERT()\
    do {\
        PRINT(" %d  %s  %s",\
                __LINE__, __FUNCTION__, __FILE__);\
        while (1);\
    } while (0)

    #define ERR(...)\
    do {\
        if (debug >= DEBUG_LEVEL_ERR) {\
            PRINT(__VA_ARGS__);\
        }\
    } while (0)

    #define INFO(...)\
    do {\
        if (debug >= DEBUG_LEVEL_INFO) {\
            PRINT(__VA_ARGS__);\
        }\
    } while (0)

    #define DEBUG(...)\
    do {\
        if (debug >= DEBUG_LEVEL_DEBUG) {\
            PRINT(__VA_ARGS__);\
        }\
    } while (0)
#else
    #define ASSERT()   do { } while (0)
    #define ERR(...)   do { } while (0)
    #define INFO(...)  do { } while (0)
    #define DEBUG(...) do { } while (0)
#endif

#endif
