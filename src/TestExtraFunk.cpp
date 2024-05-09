#include "TestExtraFunk.h"
#include <iostream>
#include <koboplatformfunctions.h>
#include "kobodevicedescriptor.h"

void TestingStuff::Dodo()
{
    if ( KoboPlatformFunctions::getKoboDeviceDescriptor().device ==  KoboDevice::KoboLibraColour )
        std::cout << "Hello Colour" << std::endl;

    std::cout << "dodo" << std::endl;
}
