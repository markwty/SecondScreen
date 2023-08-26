/***************************************************************
 * Name:      SecondScreenApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Mark ()
 * Created:   2022-11-27
 * Copyright: Mark ()
 * License:
 **************************************************************/

#include "SecondScreenApp.h"

//(*AppHeaders
#include "SecondScreenMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(SecondScreenApp);

bool SecondScreenApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	SecondScreenFrame* Frame = new SecondScreenFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
