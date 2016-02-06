//
//  ioCommandLine.h
//  ioCommandLine
//
//  Created by Manuel Schreiner on 06.02.16.
//  Copyright © 2016 io-expert.com. All rights reserved.
//

#ifndef ioCommandLine_h
#define ioCommandLine_h

#include "string.h"
#include <stdio.h>
#include "base_types.h"

#define IOCOMMANDLINE_MAXBUFFER  512
#define IOCOMMANDLINE_MAXCMDARGS   5

typedef void (*pfn_iocommandline_callback_t)(char_t* args[], uint8_t argv);

typedef int (*pfn_iocommandline_putchar_t)(char_t c);

struct stc_iocommandline_item;

typedef struct stc_iocommandline_item
{
    pfn_iocommandline_callback_t pfnCallback;
    char_t* pcCommand;
    char_t* pcHelpText;
    struct stc_iocommandline_item* nextListItm;
    
} stc_iocommandline_item_t;

void IoCommandLine_Init(pfn_iocommandline_putchar_t pfnPutChar);
void IoCommandLine_AddCommands(stc_iocommandline_item_t apfnCommands[], uint8_t u8CommandCount);
void IoCommandLine_Execute(char_t *pcString);
void IoCommandLine_ExecuteCommand(char_t* pcCommand,char_t* pcArgumentString);
void IoCommandLine_ExecuteCommandArgs(char_t* pcCommand,char_t* args[],uint8_t argv);
void IoCommandLine_Update(void);
#if IOCOMMANDLINE_MAXBUFFER > 0
void IoCommandLine_AddChar(char_t cChar);
void IoCommandLine_AddString(char_t* pcChar);
#endif

#endif /* ioCommandLine_h */
