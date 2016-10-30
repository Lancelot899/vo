#include <stdio.h>

#include "voSystem.h"

#include "ImgIO.h"
#include "Frame.h"

voSystem::voSystem()
{
    imgIO = std::make_shared<ImgIO>();
}

bool voSystem::running()
{
    trackThread = new boost::thread(std::bind(&voSystem::tracking, this));
    optThread = new boost::thread(std::bind(&voSystem::optimize, this));

    return true;
}

void voSystem::tracking()
{
    printf("tracking!\n");
}

void voSystem::optimize()
{
    printf("optimize!\n");
}
