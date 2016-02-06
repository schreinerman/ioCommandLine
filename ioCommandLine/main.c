//
//  main.c
//  ioCommandLine
//
//  Created by Manuel Schreiner on 06.02.16.
//  Copyright © 2016 io-expert.com. All rights reserved.
//

#include <stdio.h>
#include "iocommandline.h"

volatile boolean_t bExit = FALSE;
static void _Cmd1(char_t* args[], uint8_t argv);
static void _Cmd2(char_t* args[], uint8_t argv);
static void _CmdExit(char_t* args[], uint8_t argv);
#define COMMAND_COUNT 4
static stc_iocommandline_item_t astcCommands[] = {
    {NULL,"","Commands in main.c"},
    {_Cmd1,"cmd1","Command 1"},
    {_Cmd2,"cmd2","Command 2"},
    {_CmdExit,"exit","Quit"}};


static void _Cmd1(char_t* args[], uint8_t argv)
{

}
static void _Cmd2(char_t* args[], uint8_t argv)
{
    
}
static void _CmdExit(char_t* args[], uint8_t argv)
{
    bExit = TRUE;
}
    
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    IoCommandLine_Init(putchar);
    IoCommandLine_AddCommands(astcCommands, COMMAND_COUNT);
    while(bExit == FALSE)
    {
        IoCommandLine_AddChar(getchar());
        IoCommandLine_Update();
    }
    /*IoCommandLine_AddString("he");
    IoCommandLine_Update();
    IoCommandLine_AddString("lp");
    IoCommandLine_Update();
    IoCommandLine_AddString(" myparm1 tes2 test3");
    IoCommandLine_Update();
    IoCommandLine_AddString("\r");*/
    
    return 0;
}
