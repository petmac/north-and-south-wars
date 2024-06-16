#pragma once

using InterruptHandler = void();

void TakeSystem();
void FreeSystem();
void SetInterruptHandler(InterruptHandler *interrupt);
