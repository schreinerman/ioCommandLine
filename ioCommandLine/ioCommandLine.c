//
//  ioCommandLine.c
//  ioCommandLine
//
//  Created by Manuel Schreiner on 06.02.16.
//  Copyright © 2016 io-expert.com. All rights reserved.
//

#include "ioCommandLine.h"

#if IOCOMMANDLINE_MAXBUFFER > 0
static uint8_t au8InternBuffer[IOCOMMANDLINE_MAXBUFFER];
static volatile uint16_t u16CurrentPos;
#endif

static stc_iocommandline_item_t* pRoot = NULL;
static pfn_iocommandline_putchar_t myPutChar = NULL;

static void _CmdHelp(char_t* args[], uint8_t argv);


#define COMMAND_COUNT 1
static stc_iocommandline_item_t astcCommands[] = {{_CmdHelp,"help","Print help"}};

static void myPutString(char_t* pcString);


static void _CmdHelp(char_t* args[], uint8_t argv)
{
    stc_iocommandline_item_t* pCurrent = pRoot;
    uint8_t i;
    myPutString("\r\n");
    myPutString("\r\n");
    myPutString("Available command line options:\r\n");
    while(pCurrent != NULL)
    {
            if ((NULL == pCurrent->pcCommand) || (NULL == pCurrent->pfnCallback))
            {
                myPutString("\r\n");
                myPutString(pCurrent->pcHelpText);
                myPutString("\r\n");
            }
            else
            {
                myPutString("  ");
                myPutString(pCurrent->pcCommand);
                for(i = strlen(pCurrent->pcCommand);i < 20;i++)
                {
                    myPutString(" ");
                }
                myPutString(pCurrent->pcHelpText);
                myPutString("\r\n");
            }
        pCurrent = pCurrent->nextListItm;
    }
    myPutString("\r\n");
    myPutString("\r\n");
}

static void myPutString(char_t* pcString)
{
    if (myPutChar == NULL) return;
    while(*pcString != '\0')
    {
        myPutChar(*pcString);
        pcString++;
    }
}

void IoCommandLine_Init(pfn_iocommandline_putchar_t pfnPutChar)
{
    memset(au8InternBuffer,0,IOCOMMANDLINE_MAXBUFFER);
    myPutChar = pfnPutChar;
    IoCommandLine_AddCommands(astcCommands,COMMAND_COUNT);
    
}

void IoCommandLine_AddCommands(stc_iocommandline_item_t apfnCommands[], uint8_t u8CommandCount)
{
    uint8_t i;
    stc_iocommandline_item_t* pLastItem = pRoot;
    if (pRoot != NULL)
    {
        while(pLastItem->nextListItm != NULL)
        {
            pLastItem = pLastItem->nextListItm;
        }
    }
    for(i = 0;i <u8CommandCount;i++)
    {
        if (pRoot == NULL)
        {
            pRoot = &apfnCommands[i];
            pLastItem = pRoot;
            apfnCommands[i].nextListItm = NULL;
        }
        else
        {
            pLastItem->nextListItm = &apfnCommands[i];
            pLastItem = pLastItem->nextListItm;
        }
    }
}

void IoCommandLine_Execute(char_t *pcString)
{
    char_t* pcTmp = pcString;
    while(*pcTmp != '\0')
    {
        if (*pcTmp == ' ')
        {
            *pcTmp = '\0';
            pcTmp++;
            IoCommandLine_ExecuteCommand(pcString,pcTmp);
            return;
        }
        pcTmp++;
    }
    IoCommandLine_ExecuteCommand(pcString,NULL);
}

void IoCommandLine_ExecuteCommand(char_t* pcCommand,char_t* pcArgumentString)
{
    char_t* args[IOCOMMANDLINE_MAXCMDARGS];
    volatile uint8_t argv = 0;
    char_t* pcTmp = pcArgumentString;
    if (pcArgumentString != NULL)
    {
      while(*pcTmp != '\0')
      {
        if (*pcTmp != ' ')
        {
            args[argv] = pcTmp;
            argv++;
            while(*pcTmp != '\0')
            {
                if (*pcTmp == ' ')
                {
                    *pcTmp = '\0';
                    pcTmp++;
                    break;
                }
                pcTmp++;
            }
        }
        else
        {
            pcTmp++;
        }
      }
    }
    IoCommandLine_ExecuteCommandArgs(pcCommand,args,argv);
}

void IoCommandLine_ExecuteCommandArgs(char_t* pcCommand,char_t* args[],uint8_t argv)
{
    uint8_t i;
    stc_iocommandline_item_t* pCurrent = pRoot;
    while(pCurrent != NULL)
    {
        if (strcmp(pcCommand,pCurrent->pcCommand) == 0)
        {
            if (pCurrent->pfnCallback != NULL)
            {
                pCurrent->pfnCallback(args,argv);
                return;
            }
        }
        pCurrent = pCurrent->nextListItm;
    }
    myPutString("Command not found...\r\n");
}
    
void IoCommandLine_Update(void)
{
    static uint16_t u16LastCheck = 0;
    if (u16LastCheck != u16CurrentPos)
    {
        for(;u16LastCheck <= u16CurrentPos;u16LastCheck++)
        {
            if ((au8InternBuffer[u16LastCheck] == '\r') || (au8InternBuffer[u16LastCheck] == '\n') ||(u16LastCheck == IOCOMMANDLINE_MAXBUFFER))
            {
                au8InternBuffer[u16LastCheck] = '\0';
                IoCommandLine_Execute((char_t*)&au8InternBuffer[0]);
                memcpy(&au8InternBuffer[0],&au8InternBuffer[u16LastCheck],(IOCOMMANDLINE_MAXBUFFER - u16LastCheck));
                u16LastCheck = 0;
                u16CurrentPos = 0;
                                                                           
            }
            if (au8InternBuffer[u16LastCheck] == 0) break;
        }
    }
}

#if IOCOMMANDLINE_MAXBUFFER > 0
void IoCommandLine_AddChar(char_t cChar)
{
    if ((cChar == '\b') && (u16CurrentPos > 0))
    {
        u16CurrentPos--;
    }
    
    if (u16CurrentPos < (IOCOMMANDLINE_MAXBUFFER - 1))
    {
        au8InternBuffer[u16CurrentPos] = cChar;
        au8InternBuffer[u16CurrentPos+1] = '\0';
        u16CurrentPos++;
    }
}
void IoCommandLine_AddString(char_t* pcChar)
{
    while(*pcChar != '\0')
    {
        IoCommandLine_AddChar(*pcChar);
        pcChar++;
    }
}
#endif