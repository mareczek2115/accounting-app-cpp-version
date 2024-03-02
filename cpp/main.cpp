#include <nana/gui.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <Windows.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

#include "admin_funcs.h"
#include "user_funcs.h"
#include "utils.h"

using json = nlohmann::json;

void render_login_form();
void render_admin_panel();
void render_user_panel();

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    //ShowWindow(GetConsoleWindow(), SW_SHOW);

    render_login_form();

    return 0;
}