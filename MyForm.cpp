#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Ensure "BloodBank" matches your Project Name 
    // and "MyForm" matches your Form Name
    BloodBank::MyForm form;
    Application::Run(% form);
    return 0;
}