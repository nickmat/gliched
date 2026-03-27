#include <wx/wx.h>
#include "geMainFrame.h"

#include <glc/hic.h>

class GeInOut : public glich::InOut

{
public:
    std::string get_input( const std::string& prompt ) override;
};

std::string GeInOut::get_input( const std::string& prompt )
{
    wxTextEntryDialog dialog(
        nullptr, prompt,
        _( "Gliched Input" ), "", wxOK | wxCANCEL
    );
    if( dialog.ShowModal() == wxID_OK ) {
        return dialog.GetValue().ToStdString();
    }
    return std::string();
}


class GlichedApp : public wxApp
{
public:
    bool OnInit() override
    {
        glich::StdStrVec args;
        for( int i = 0; i < argc; i++ ) {
            args.push_back( std::string( argv[i] ) );
        }
        glich::InitLibrary lib = glich::InitLibrary::Hics;
        wxString filename;
        if( argc > 1 ) {
            if( argv[1] == "--lib" && argc > 2 ) {
                wxString libname = argv[2];
                if( libname == "none" ) {
                    lib = glich::InitLibrary::None;
                }
                else if( libname == "hics" ) {
                    lib = glich::InitLibrary::Hics;
                }
                if( argc > 3 ) {
                    filename = argv[3];
                }
            }
            else {
                filename = argv[1];
            }
        }
        glich::init_hic( lib, new GeInOut, args );

        geMainFrame* frame = new geMainFrame();
        frame->Show();
        return true;
    }

    int OnExit() override
    {
        glich::exit_hic();
        return 0;
    }
};

wxIMPLEMENT_APP( GlichedApp );
