/***************************************************************
 * Name:      SecondScreenMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Mark ()
 * Created:   2022-11-27
 * Copyright: Mark ()
 * License:
 **************************************************************/

#include "SecondScreenMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(SecondScreenFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(SecondScreenFrame)
//*)

BEGIN_EVENT_TABLE(SecondScreenFrame,wxFrame)
    //(*EventTable(SecondScreenFrame)
    //*)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ShapedFrame, wxFrame)
    EVT_LEFT_DOWN(ShapedFrame::OnLeftDown)
    EVT_LEFT_UP(ShapedFrame::OnLeftUp)
    EVT_MOTION(ShapedFrame::OnMouseMove)
    EVT_RIGHT_UP(ShapedFrame::OnExit)

    EVT_PAINT(ShapedFrame::OnPaint)

#ifdef __WXGTK__
    EVT_WINDOW_CREATE(ShapedFrame::OnWindowCreate)
#endif
END_EVENT_TABLE()

SecondScreenFrame::SecondScreenFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(SecondScreenFrame)
    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxFRAME_NO_TASKBAR|wxFRAME_SHAPED|wxBORDER_NONE, _T("wxID_ANY"));
    SetClientSize(wxSize(42,25));

    Connect(wxEVT_PAINT,(wxObjectEventFunction)&SecondScreenFrame::OnPaint);
    Connect(wxEVT_ERASE_BACKGROUND,(wxObjectEventFunction)&SecondScreenFrame::OnEraseBackground);
    //*)
    //0|
    OnInit();
}

SecondScreenFrame::~SecondScreenFrame()
{
    //(*Destroy(SecondScreenFrame)
    //*)
}

bool SecondScreenFrame::OnInit()
{
    wxInitAllImageHandlers();

    ShapedFrame *frame = new ShapedFrame(this);
    frame->Show(true);
    //SetTopWindow(frame);
    return true;
}

void SecondScreenFrame::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    if(m_bmp != nullptr){
        dc.SetUserScale(scale, scale);
        dc.SetBackground(*wxBLACK_BRUSH);
        dc.Clear();
        dc.DrawBitmap(*m_bmp, wxPoint(x/scale, y/scale), false);
    }
}

/*
void SecondScreenFrame::OnCropBtnClick(wxCommandEvent& event)
{
	wxScreenDC dcScreen;

	wxCoord screenWidth, screenHeight;
	dcScreen.GetSize(&screenWidth, &screenHeight);
	wxBitmap screenshot(screenWidth, screenHeight,-1);

	wxMemoryDC memDC;
	memDC.SelectObject(screenshot);

	memDC.Blit( 0, //Copy to this X coordinate
				0, //Copy to this Y coordinate
				screenWidth, //Copy this width
				screenHeight, //Copy this height
				&dcScreen, //From where do we copy?
				0, //What's the X offset in the original DC?
				0  //What's the Y offset in the original DC?
			);

	memDC.SelectObject(wxNullBitmap);
	screenshot.SaveFile("screenshot.jpg",wxBITMAP_TYPE_JPEG);
}
*/

ShapedFrame::ShapedFrame(SecondScreenFrame *_frame)
       : wxFrame((wxFrame *)NULL, wxID_ANY, wxEmptyString,
                  wxDefaultPosition, wxSize(100, 100), //wxDefaultSize,
                  0
                  | wxFRAME_SHAPED
                  | wxSIMPLE_BORDER
                  | wxFRAME_NO_TASKBAR
                  | wxSTAY_ON_TOP
            )
{
    wxScreenDC dcScreen;
    wxRect rect = wxDisplay().GetGeometry();
	wxCoord screenWidth(rect.GetWidth()), screenHeight(rect.GetHeight());
	wxCoord _screenWidth, _screenHeight;
	dcScreen.GetSize(&_screenWidth, &_screenHeight);
    m_bmp = new wxBitmap(screenWidth, screenHeight, -1);
    SetSize(wxSize(m_bmp->GetWidth(), m_bmp->GetHeight()));

    wxMemoryDC memDC;
	memDC.SelectObject(*m_bmp);
	//scaleScreen = screenHeight/(_screenHeight + 0.0);
	//memDC.SetUserScale(scaleScreen, scaleScreen);
	memDC.Blit( 0, //Copy to this X coordinate
				0, //Copy to this Y coordinate
				_screenWidth, //Copy this width
				_screenHeight, //Copy this height
				&dcScreen, //From where do we copy?
				0, //What's the X offset in the original DC?
				0  //What's the Y offset in the original DC?
            );
	memDC.SelectObject(wxNullBitmap);

	if(wxDisplay::GetCount() >= 2){
        display = new wxDisplay(1);
        displayValid = true;
    }
    frame = _frame;

#ifndef __WXGTK__
    Refresh();
#endif
}
/*
void ShapedFrame::SetWindowShape()
{
	wxMemoryDC mdc(*m_bmp2);
	mdc.Clear();
	mdc.DrawBitmap(*m_bmp, wxCoord(0), wxCoord(0));
	mdc.SetBrush(wxTransparentColor);
    mdc.SetPen(wxPen(wxColor(1,1,1), 4));
	mdc.DrawRectangle(m_delta.x, m_delta.y,
                      pos.x - m_delta.x, pos.y - m_delta.y);
	mdc.SelectObject(wxNullBitmap);
	m_hasShape = SetShape(wxRegion(*m_bmp2, wxColor(1,1,1), 0));
}
*/
void ShapedFrame::OnLeftDown(wxMouseEvent& evt)
{
    CaptureMouse();
    pos = ClientToScreen(evt.GetPosition());
    wxPoint origin = GetPosition();
    int dx =  pos.x - origin.x;
    int dy = pos.y - origin.y;
    m_delta = wxPoint(dx, dy);
}

void ShapedFrame::OnLeftUp(wxMouseEvent& WXUNUSED(evt))
{
    if (HasCapture())
    {
        ReleaseMouse();
        if(pos.x - m_delta.x > 5 && pos.y - m_delta.y > 5)
        {
            if(displayValid)
            {
                wxRect rect = display->GetGeometry();
                frame->SetPosition(wxPoint(rect.GetLeft(), rect.GetTop()));
                frame->SetSize(wxSize(rect.width, rect.height));
                double wScale = 1, hScale = 1, scale;
                wScale = rect.width/(pos.x - m_delta.x + 0.0);
                hScale = rect.height/(pos.y - m_delta.y + 0.0);
                if(wScale > hScale){
                    scale = hScale;
                    frame->x = (rect.width - (pos.x - m_delta.x) * scale)/2;
                    frame->y = 0;
                } else {
                    scale = wScale;
                    frame->x = 0;
                    frame->y = (rect.height - (pos.y - m_delta.y) * scale)/2;
                }
                frame->scale = scale;

                if(screenSet){
                    delete frame->m_bmp;
                }
                frame->m_bmp = new wxBitmap(m_bmp->GetSubBitmap(wxRect(m_delta.x, m_delta.y,
                                            pos.x - m_delta.x, pos.y - m_delta.y)));
                frame->Refresh();
                frame->Update();
                //wxMessageBox(wxString::Format("%d,%d,%d,%d", m_delta.x, m_delta.y,
                //                              pos.x - m_delta.x, pos.y - m_delta.y));
                screenSet = true;
            }

        }
    }
}

void ShapedFrame::OnMouseMove(wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    if (evt.Dragging() && evt.LeftIsDown())
    {
        pos = ClientToScreen(pt);
        Refresh();
    }
}

void ShapedFrame::OnExit(wxMouseEvent& WXUNUSED(evt))
{
	delete m_bmp;
	delete frame->m_bmp;
	//delete mdc;
	frame->Close();
    Close();
}

void ShapedFrame::OnWindowCreate(wxWindowCreateEvent& WXUNUSED(evt))
{
    Refresh();
}


void ShapedFrame::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    //dc.Clear();
    dc.DrawBitmap(*m_bmp, wxCoord(0), wxCoord(0));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(wxPen(wxColor(1,1,1), 4));
	dc.DrawRectangle(m_delta.x, m_delta.y,
                    pos.x - m_delta.x, pos.y - m_delta.y);
}

void SecondScreenFrame::OnEraseBackground(wxEraseEvent& event)
{
}
