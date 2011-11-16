#ifndef DX11SANDBOX_DEMOAPPLICATION_H
#define DX11SANDBOX_DEMOAPPLICATION_H

#include "Root.h"
using namespace Dx11Sandbox;
class DemoApplication: public Application
{
    public:
        virtual void OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown);
        virtual  void OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                            bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                            int xPos, int yPos );
        virtual void createWorld(SceneManager* mngr);
        virtual void update(SceneManager* mngr,double fTime, float fElapsedTime);
        virtual void shutDown(SceneManager* mngr);
};

#endif