#include "LandingPage.h" 

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Create and run the Landing Page
    BloodBank::LandingPage form;
    Application::Run(% form);

    return 0;
}