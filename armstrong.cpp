#include "armstrong.h"

void armInitialize() {
	ancInitialize(0);

	adiInitialize();
	aaiInitialize();
}

void armUpdate() {
  aaiUpdate();
  adiUpdate();
  ancUpdate();  
}

