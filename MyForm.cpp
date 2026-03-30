#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    // Initializing visual styles for the Windows app
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // 'BloodBank' is your namespace (Project Name)
    // 'MyForm' is your class name (Form Name)
    BloodBank::MyForm form;

    // This starts the application and opens your window
    Application::Run(% form);

    return 0;
}