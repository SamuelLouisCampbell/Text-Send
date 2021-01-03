#include "text.h"

Text::Text(const wxString& title)
	:
	wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
{
	Connect(wxEVT_PAINT, wxPaintEventHandler(Text::OnPaint));
	Centre();
}

void Text::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
	wxColor fillColor = { 0, 0, 0 };
	wxBrush bgBrush = {fillColor, wxBrushStyle::wxBRUSHSTYLE_SOLID };
	dc.SetBackground(bgBrush);
	dc.Clear();
	dc.SetTextForeground({ 255,255,255 });

	wxFont font = { 72, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString };
	font.SetFaceName("Tahoma");
	dc.SetFont(font);

	dc.DrawText(wxT("Hello! How are you?"), 40, 60);
	dc.DrawText(wxT("I am well many thanks for asking"),0, 80+72);

}
