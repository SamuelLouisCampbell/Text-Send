#include "cApp.h"


wxIMPLEMENT_APP(cApp);

cApp::cApp()
{
}

cApp::~cApp()
{
}

bool cApp::OnInit()
{
    fram0 = new cMain();
    
    fram0->SetIcons(wxICON(aaaaIDI_ICON1));
    fram0->Show();
    return true;
}
