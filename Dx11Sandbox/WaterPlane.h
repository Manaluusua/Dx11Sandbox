#ifndef WATERPLANE_H
#define WATERPLANE_H

#include "SceneManager.h"

class WaterPlane: public Dx11Sandbox::RenderStartListener
{
public:
    WaterPlane(void);
    ~WaterPlane(void);


    virtual void renderingStarted(Dx11Sandbox::RenderContext* context,Dx11Sandbox::SceneManager* mngr, double fTime, float fElapsedTime);
};

#endif
