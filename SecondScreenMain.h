/***************************************************************
 * Name:      SecondScreenMain.h
 * Purpose:   Defines Application Frame
 * Author:    Mark ()
 * Created:   2022-11-27
 * Copyright: Mark ()
 * License:
 **************************************************************/

#ifndef SECONDSCREENMAIN_H
#define SECONDSCREENMAIN_H

//(*Headers(SecondScreenFrame)
#include <wx/frame.h>
//*)

#include <wx/app.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcscreen.h>
#include <wx/display.h>
#include <wx/timer.h>

class SecondScreenFrame: public wxFrame
{
    public:

        SecondScreenFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~SecondScreenFrame();
        bool OnInit();

        int x = 0, y = 0;
        double scale = 1;
        wxBitmap *m_bmp = nullptr;

    private:

        //(*Handlers(SecondScreenFrame)
        void OnPaint(wxPaintEvent& event);
        void OnEraseBackground(wxEraseEvent& event);
        //*)

        //(*Identifiers(SecondScreenFrame)
        //*)

        //(*Declarations(SecondScreenFrame)
        //*)

        DECLARE_EVENT_TABLE()
};

class ShapedFrame : public wxFrame
{
    public:
        ShapedFrame(SecondScreenFrame *frame);
        //void SetWindowShape();

        void OnLeftDown(wxMouseEvent& evt);
        void OnLeftUp(wxMouseEvent& evt);
        void OnMouseMove(wxMouseEvent& evt);
        void OnExit(wxMouseEvent& evt);
        void OnPaint(wxPaintEvent& evt);
        void OnWindowCreate(wxWindowCreateEvent& evt);

    private:
        wxBitmap *m_bmp;
        wxPoint m_delta, pos;
        wxDisplay *display;
        bool displayValid = false, screenSet = false;
        double scaleScreen = 1;

        SecondScreenFrame* frame;

        DECLARE_EVENT_TABLE()
};
#endif // SECONDSCREENMAIN_H
