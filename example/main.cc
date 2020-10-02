/*
*****************************************************************************

  File        : main.cpp

  The MIT License (MIT)
  Copyright (c) 2018 STMicroelectronics
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*****************************************************************************
*/

#include<iostream>

#include "GUI.h"
#include "HUI.h"
#include "SEQ.h"
#include "SCREEN.h"
#include "OPT.h"

#include "poulpe.h"


DEFINE_LISTENERS(GUI, SCREEN, HUI, OPT<Notifier>)

// notifier factory instantiation
NotifierFactory<
Notifier,
SEQ,
SignalTypes<
	const SEQ_signal&, 
	const OPT_signal&, 
	const HUI_signal&>,
	uint8_t> seq(5);

// notifier factory instantiation
NotifierFactory<
Notifier,
HUI,
SignalTypes<const HUI_signal&>> hui;

// notifier instantiation
OPT<Notifier> opt(5, 4, 3);


GUI gui; 			// receiver
SCREEN screen; 		// receiver


CREATE_POULPE(gui, screen, hui, opt)


int main(void){

	hui.Test();
	seq.Test();

	return 0;
}
