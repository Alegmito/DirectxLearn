#include <Windows.h>
#include "app.h"
#include "exceptions/directxException.h"


int CALLBACK WinMain(HINSTANCE , HINSTANCE , LPSTR , int ) {
    try {
        App {}.Run();
    }
    catch (DirectxException &ex) {
        MessageBox(nullptr, ex.what(), ex.getType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (std::exception &ex) {
        MessageBox(nullptr, ex.what(), "Standard exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBox(nullptr, "No details avaliable", "Standard exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}
