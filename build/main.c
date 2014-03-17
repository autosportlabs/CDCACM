#include <stdio.h>
#include <stm32f4xx.h>
 
void main (void) {
   printf("Hello, world!");
}

/**
   @file fputc_debug.c
   @brief Trying to redirect printf() to debug port
   @date 2012/06/25
*/

 
int fputc(int c, FILE *stream)
{
   return(ITM_SendChar(c));
}