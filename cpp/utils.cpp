#include <iostream>
#include <string.h>
#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <assert.h>

#include "admin_funcs.h"
#include "user_funcs.h"
#include "utils.h"

using json = nlohmann::json;

void post_request(json data, std::string url, std::string success_text[2], std::string fail_text[2], nana::form * parent_window)
{
	cpr::Response response = cpr::Post(cpr::Url{ url }, cpr::Body{ data.dump() },
		cpr::Header{ {"Content-Type", "application/json"} }, cpr::Timeout{5000});
    if (response.elapsed > 5) {
        nana::msgbox msgbox{ *parent_window, "B³¹d", nana::msgbox::ok};
        msgbox.icon(nana::msgbox::icon_error);
        msgbox << nana::charset("B³¹d ³¹czenia z serwerem lub baz¹ danych").to_bytes(nana::unicode::utf8);
        msgbox();
    }
    else {
        json parsed_json = json::parse(response.text);
        std::string state = parsed_json["res"];

        if (parsed_json.contains("konto")) {
            std::string account_type = parsed_json["konto"];
            nana::API::exit_all();
            if (account_type == "admin")
                render_admin_panel();
            else
                render_user_panel();
        }
        else {
            if (state == "success") {
                nana::msgbox msgbox{ *parent_window, success_text[0], nana::msgbox::ok };
                msgbox.icon(nana::msgbox::icon_information);
                msgbox << nana::charset(success_text[1]).to_bytes(nana::unicode::utf8);
                auto msg_res = msgbox();
                if (msg_res == msgbox.pick_ok)
                    nana::API::close_window(*parent_window);
            }
            else {
                nana::msgbox msgbox{ *parent_window, fail_text[0], nana::msgbox::ok };
                msgbox.icon(nana::msgbox::icon_error);
                msgbox << nana::charset(fail_text[1]).to_bytes(nana::unicode::utf8);
                msgbox();
            }
        }
    }
	
}

void render_login_form() {
    nana::form login_form{ nana::API::make_center(600, 400), nana::appear::decorate<>() };
    login_form.caption(nana::charset("Ksiêgowoœæ 2.137").to_bytes(nana::unicode::utf8));

    nana::place login_form_place{ login_form };

    nana::label username_label{ login_form, "Nazwa u¿ytkownika:" }, password_label{ login_form, "Has³o:" };
    username_label.typeface(nana::paint::font("", 10, true));
    password_label.typeface(nana::paint::font("", 10, true));

    nana::textbox username_textbox{ login_form }, password_textbox{ login_form };
    nana::button login_btn{ login_form, nana::rectangle(nana::point(10, 200), nana::size(120, 60)) };

    username_textbox.multi_lines(false);
    password_textbox.multi_lines(false).mask('*');

    login_btn.caption("Zaloguj");
    login_btn.events().click([&]() {
        json log_data;
        log_data["login"] = username_textbox.text();
        log_data["haslo"] = password_textbox.text();

        std::string success_text[2] = { "", "" };
        std::string fail_text[2] = { "B³¹d", "Konto nie istnieje lub wprowadzono b³êdne dane logowania!" };

        post_request(log_data, "http://localhost:8080/logowanie", success_text, fail_text, &login_form);
    });

    login_form_place.div("<margin=[35%, 25%, 20%, 25%] <vertical <weight=100 margin=[0, 0, 8%, 0] <vertical <username_label><password_label>><vertical <margin=[0, 0, 33%, 0] weight=33 username_textbox><margin=[33%, 0, 0, 0] weight=33 password_textbox>>><weight=40 <login_btn>>>>");
    login_form_place["username_label"] << username_label;
    login_form_place["password_label"] << password_label;
    login_form_place["username_textbox"] << username_textbox;
    login_form_place["password_textbox"] << password_textbox;

    login_form_place["login_btn"] << login_btn;
    login_form_place.collocate();

    login_form.show();

    nana::exec();
}

void render_admin_panel() {
    nana::form admin_panel{ nana::API::make_center(600, 400) };
    admin_panel.caption(nana::charset("Ksiêgowoœæ 2.137").to_bytes(nana::unicode::utf8));

    nana::place admin_panel_place{ admin_panel };

    nana::button new_employee_btn{ admin_panel },
        edit_employee_btn{ admin_panel },
        new_account_btn{ admin_panel },
        edit_account_btn{ admin_panel },
        new_office_btn{ admin_panel },
        edit_office_btn{ admin_panel };

    new_employee_btn.caption("Dodaj pracownika").events().click([&admin_panel]() {
        new_employee(&admin_panel);
        });
    edit_employee_btn.caption("Edytuj pracownika").events().click([&admin_panel]() {
        edit_employee(&admin_panel);
        });
    new_account_btn.caption(nana::charset("Dodaj nowe konto ksiêgowego").to_bytes(nana::unicode::utf8)).events().click([&admin_panel]() {
        new_account(&admin_panel);
        });
    edit_account_btn.caption(nana::charset("Edytuj konto ksiêgowego").to_bytes(nana::unicode::utf8)).events().click([&admin_panel]() {
        edit_account(&admin_panel);
        });
    new_office_btn.caption("Dodaj nowe stanowisko").events().click([&admin_panel]() {
        new_office(&admin_panel);
        });
    edit_office_btn.caption("Edytuj stanowisko").events().click([&admin_panel]() {
        edit_office(&admin_panel);
        });

    nana::button logout_btn{ admin_panel, nana::rectangle(nana::point(50, 50), nana::size(120, 50)) };
    logout_btn.caption("Wyloguj");
    logout_btn.events().click([&]() {
        nana::API::exit_all();
        render_login_form();
    });

    admin_panel_place.div("<margin = [20 %, 15 %] <vertical <vertical <margin=[15%, 0] weight=33% <margin=[0, 5%, 0, 0] new_employee_btn><margin=[0, 0, 0, 5%] edit_employee_btn>><margin=[15%, 0] weight=33% <margin=[0, 5%, 0, 0] new_account_btn><margin=[0, 0, 0, 5%] edit_account_btn>><margin=[15%, 0, 20%, 0] weight=33% <margin=[0, 5%, 0, 0] new_office_btn><margin=[0, 0, 0, 5%] edit_office_btn>>> <weight = 20% logout_btn >> >");

    admin_panel_place["new_employee_btn"] << new_employee_btn;
    admin_panel_place["new_account_btn"] << new_account_btn;
    admin_panel_place["new_office_btn"] << new_office_btn;

    admin_panel_place["edit_employee_btn"] << edit_employee_btn;
    admin_panel_place["edit_account_btn"] << edit_account_btn;
    admin_panel_place["edit_office_btn"] << edit_office_btn;

    admin_panel_place["logout_btn"] << logout_btn;
    admin_panel_place.collocate();

    admin_panel.show();

    nana::exec();
}

void render_user_panel() {
    nana::form user_panel{ nana::API::make_center(600, 400) };
    user_panel.caption(nana::charset("Ksiêgowoœæ 2.137").to_bytes(nana::unicode::utf8));

    nana::place user_panel_place{ user_panel };

    nana::button get_operations_btn{ user_panel },
        get_employees_btn{ user_panel },
        new_operation_btn{ user_panel },
        get_faculties_btn{ user_panel },
        pay_salary_btn{ user_panel },
        get_salaries_btn{ user_panel };

    get_operations_btn.caption("Poka¿ operacje finansowe").events().click([&user_panel]() {
        get_operations(&user_panel);
        });
    get_employees_btn.caption(nana::charset("Poka¿ pracowników").to_bytes(nana::unicode::utf8)).events().click([&user_panel]() {
        get_employees(&user_panel);
        });
    new_operation_btn.caption(nana::charset("Dodaj operacjê finansow¹").to_bytes(nana::unicode::utf8)).events().click([&user_panel]() {
        new_operation(&user_panel);
        });
    get_faculties_btn.caption("Poka¿ wydzia³y").events().click([&user_panel]() {
        get_faculties(&user_panel);
        });
    pay_salary_btn.caption(nana::charset("Wyp³aæ pensjê").to_bytes(nana::unicode::utf8)).events().click([&user_panel]() {
        pay_salary(&user_panel);
        });
    get_salaries_btn.caption(nana::charset("Poka¿ wyp³acone pensje").to_bytes(nana::unicode::utf8)).events().click([&user_panel]() {
        get_salaries(&user_panel);
        });

    nana::button logout_btn{ user_panel, nana::rectangle(nana::point(50, 50), nana::size(120, 50)) };
    logout_btn.caption("Wyloguj");
    logout_btn.events().click([&]() {
        nana::API::exit_all();
        render_login_form();
    });

    user_panel_place.div("<margin = [20 %, 15 %] <vertical <vertical <margin=[15%, 0] weight=33% <margin=[0, 5%, 0, 0] get_operations_btn><margin=[0, 0, 0, 5%] get_employees_btn>><margin=[15%, 0] weight=33% <margin=[0, 5%, 0, 0] new_operation_btn><margin=[0, 0, 0, 5%] get_faculties_btn>><margin=[15%, 0] weight=33% <margin=[0, 5%, 0, 0] pay_salary_btn><margin=[0, 0, 0, 5%] get_salaries_btn>>> <weight = 20% logout_btn >> >");

    user_panel_place["get_operations_btn"] << get_operations_btn;
    user_panel_place["get_employees_btn"] << get_employees_btn;

    user_panel_place["new_operation_btn"] << new_operation_btn;
    user_panel_place["get_faculties_btn"] << get_faculties_btn;

    user_panel_place["pay_salary_btn"] << pay_salary_btn;
    user_panel_place["get_salaries_btn"] << get_salaries_btn;

    user_panel_place["logout_btn"] << logout_btn;
    user_panel_place.collocate();

    user_panel.show();

    nana::exec();
}