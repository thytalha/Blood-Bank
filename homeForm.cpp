#include "homeForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int homeFormEntry(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Launch the animated home form
    BloodBank::homeForm home;
    Application::Run(% home);

    return 0;
}