#include "Lines.h"

Lines::Lines(const wxString& title) 
	: 
	wxFrame(nullptr, wxID_ANY, title, wxPoint(30, 30), wxSize(1280, 720))
{
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(Lines::OnPaint));
	this->Centre();
}

void Lines::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this);

	wxCoord x1 = 0, y1 = 60;
	wxCoord x2 = 1280, y2 = 60;

	dc.DrawLine(x1, y1, x2, y2);
}
