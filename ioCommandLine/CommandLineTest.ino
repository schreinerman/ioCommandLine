//
//  CommandLineTest.ino
//  ioCommandLine
//
//  Created by Manuel Schreiner on 31.10.2018.
//  Copyright © 2018 io-expert.com. All rights reserved.
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this condition and the following disclaimer.
//
// This software is provided by the copyright holder and contributors "AS IS"
// and any warranties related to this software are DISCLAIMED.
// The copyright owner or contributors be NOT LIABLE for any damages caused
// by use of this software.

#include "iocommandline.h"
#include "stdio.h"

//command prototype section
static void _Cmd1(char_t* args[], uint8_t argv);
static void _Cmd2(char_t* args[], uint8_t argv);

//command list
#define COMMAND_COUNT (sizeof(astcCommands) / sizeof(astcCommands[0]))
static stc_iocommandline_item_t astcCommands[] = {
    {NULL,"","Commands in main.c"},
    {_Cmd1,"cmd1","Command 1"},
    {_Cmd2,"cmd2","Command 2"},
};

//command 1 implementation
static void _Cmd1(char_t* args[], uint8_t argv)
{
    Serial.write("Command1\r\n");
}

//command 2 implementation
static void _Cmd2(char_t* args[], uint8_t argv)
{
    Serial.write("Command2\r\n");
}

//serial write implementation
void serialWrite(uint8_t c)
{
    Serial.write(c);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IoCommandLine_Init(serialWrite);
  IoCommandLine_AddCommands(astcCommands, COMMAND_COUNT);
  IoCommandLine_AddString("help\r");
}

void loop() {
  // put your main code here, to run repeatedly:
  IoCommandLine_Update();
}

//receive event implementation
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    IoCommandLine_AddChar(inChar);
    Serial.write(inChar);
    if (inChar == '\r') Serial.write('\n');
  }
}
