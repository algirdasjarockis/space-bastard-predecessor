//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("LevelMaker.res");
USEFORM("Main.cpp", Form1);
USEUNIT("funcs.cpp");
USE("funcs.h", File);
USEUNIT("..\TinyXml\xmltest.cpp");
USEUNIT("..\TinyXml\tinystr.cpp");
USE("..\TinyXml\tinystr.h", File);
USEUNIT("..\TinyXml\tinyxml.cpp");
USE("..\TinyXml\tinyxml.h", File);
USEUNIT("..\TinyXml\tinyxmlerror.cpp");
USEUNIT("..\TinyXml\tinyxmlparser.cpp");
USEUNIT("Level.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
         Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
