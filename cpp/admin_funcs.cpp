#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <vector>

#include <iostream>

#include "admin_funcs.h"
#include "utils.h"

using json = nlohmann::json;

struct Data {
	std::vector<std::string> labels_array;
	std::vector<std::string> data_to_edit;
	std::string api_endpoint;
	double salary;
	int employee_id;
};

void create_edit_window(nana::form* parent_window, Data data, int db_row_id, std::optional<std::string> combox_value = {}) {
	nana::form edit_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	edit_window.caption("Edytuj dane");
	nana::place edit_place{ edit_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/stanowiska"));
	json parsed_json = json::parse(response.text);

	nana::label label_1{ edit_window, data.labels_array[0]},
		label_2{ edit_window, data.labels_array[1]},
		label_3{ edit_window },
		label_4{ edit_window };
	label_1.typeface(nana::paint::font("", 10, true));
	label_2.typeface(nana::paint::font("", 10, true));
	label_3.typeface(nana::paint::font("", 10, true));
	label_4.typeface(nana::paint::font("", 10, true));

	nana::textbox textbox_1{ edit_window },
		textbox_2{ edit_window },
		textbox_3{ edit_window }, 
		textbox_4{ edit_window };

	textbox_1.caption(data.data_to_edit[0]);
	textbox_2.caption(data.data_to_edit[1]);

	textbox_1.multi_lines(false);
	textbox_2.multi_lines(false);
	textbox_3.multi_lines(false);
	textbox_4.multi_lines(false);

	nana::combox combox{ edit_window };

	nana::button confirm_button{ edit_window }, delete_button{ edit_window };
	confirm_button.events().click([&]() {
		json edited_data;

		if (data.api_endpoint == "konto" && (textbox_1.text() != data.data_to_edit[0] || textbox_2.text() != data.data_to_edit[1])) {
			edited_data["id_konta"] = db_row_id;
			edited_data["login"] = textbox_1.text();
			edited_data["haslo"] = textbox_2.text();

			std::string success_text[2] = { "OK", "Zedytowano dane konta" };
			std::string fail_text[2] = { "Błąd", "Błąd przy edytowaniu danych konta" };

			post_request(edited_data, "http://localhost:8080/edytuj-konto", success_text, fail_text, &edit_window);
			nana::API::close_window(*parent_window);
		} 
		else if (data.api_endpoint == "stanowisko" && (textbox_1.text() != data.data_to_edit[0] || textbox_2.text() != data.data_to_edit[1])) {
			std::string amount = textbox_2.text();
			std::size_t found = amount.find(",");
			if (found != std::string::npos)
				amount.replace(found, 1, ".");

			edited_data["id"] = db_row_id;
			edited_data["nazwa_stanowiska"] = textbox_1.text();
			edited_data["pensja_bazowa"] = amount;

			std::string success_text[2] = { "OK", "Zedytowano dane stanowiska" };
			std::string fail_text[2] = { "Błąd", "Błąd przy edytowaniu danych stanowiska" };

			post_request(edited_data, "http://localhost:8080/edytuj-stanowisko", success_text, fail_text, &edit_window);
			nana::API::close_window(*parent_window);
		} 
		else if(data.api_endpoint == "pracownika") {
			if (textbox_1.text() != data.data_to_edit[0] || textbox_2.text() != data.data_to_edit[1] || textbox_3.text() != data.data_to_edit[2] || textbox_4.text() != data.data_to_edit[3]) {
				edited_data["id"] = db_row_id;
				edited_data["imie"] = textbox_1.text();
				edited_data["nazwisko"] = textbox_2.text();
				edited_data["id_stanowiska"] = parsed_json[combox.option()]["id"];
				edited_data["identyfikator_firmowy_pracownika"] = textbox_4.text();

				std::string success_text[2] = { "OK", "Zedytowano dane pracownika" };
				std::string fail_text[2] = { "Błąd", "Błąd przy edytowaniu danych pracownika" };

				post_request(edited_data, "http://localhost:8080/edytuj-pracownika", success_text, fail_text, &edit_window);
				nana::API::close_window(*parent_window);
			}
		}
	});
	delete_button.events().click([&]() {
		nana::msgbox msgbox{ *parent_window, "?", nana::msgbox::yes_no};
		msgbox.icon(nana::msgbox::icon_question);
		msgbox << nana::charset("Czy na pewno chcesz usunąć te dane?").to_bytes(nana::unicode::utf8);
		if (msgbox() == msgbox.pick_yes) {
			json id_to_delete;
			if (data.api_endpoint == "konto")
				id_to_delete["id_konta"] = db_row_id;
			else
				id_to_delete["id"] = db_row_id;
			cpr::Response response = cpr::Delete(cpr::Url{ "http://localhost:8080/usun-" + data.api_endpoint }, cpr::Body{ id_to_delete.dump() },
				cpr::Header{ {"Content-Type", "application/json"} });
			json parsed_json = json::parse(response.text);
			if (parsed_json["res"] == "success") {
				nana::msgbox msgbox{ *parent_window, "OK", nana::msgbox::ok};
				msgbox.icon(nana::msgbox::icon_information);
				msgbox << nana::charset("Pomyślnie usunięto dane").to_bytes(nana::unicode::utf8);
				if (msgbox() == msgbox.pick_ok) {
					nana::API::close_window(edit_window);
					nana::API::close_window(*parent_window);
				}
			}
			else {
				nana::msgbox msgbox{ *parent_window, "", nana::msgbox::ok};
				msgbox.icon(nana::msgbox::icon_error);
				msgbox << nana::charset("Błąd przy usuwaniu danych").to_bytes(nana::unicode::utf8);
				msgbox();
			}
			
		}
		
	});
	confirm_button.caption(nana::charset("Zatwierdź zmiany").to_bytes(nana::unicode::utf8));
	delete_button.caption(nana::charset("Usuń").to_bytes(nana::unicode::utf8));

	if (data.labels_array.size() == 2) {
		edit_place.div("<margin=[15%, 25%, 30%, 25%] <vertical <<vertical <margin=[5%, 0, 0, 0] label_1><margin=[0, 0, 5%, 0] label_2>><vertical <margin=[0, 0, 35%, 0] weight=40 textbox_1><margin=[35%, 0, 0, 0] weight=40 textbox_2>>><margin=[0, 0, 10%, 0] weight=50 confirm_button><margin=[10%, 0, 0, 0] weight=50 delete_button>>>");
		if (data.labels_array[0] == "Login:")
			textbox_2.mask('*');
		else {
			textbox_2.caption(data.data_to_edit[1].replace(data.data_to_edit[1].length() - 3, 1, ","));
		}
	}
	else
		edit_place.div("<margin=[15%, 25%, 25%, 25%] <vertical <<vertical <margin=[5%, 0, 0, 0] label_1><margin=[5%, 0, 0, 0] label_2><label_3><label_4>><vertical <margin=[0, 0, 8%, 0] weight=25 textbox_1><margin=[30%, 0, 0, 0] weight=33 textbox_2><margin=[33%, 0, 0, 0] weight=33 combox><margin=[33%, 0, 0, 0] weight=33 textbox_4>>><margin=[0, 0, 10%, 0] weight=50 confirm_button><margin=[10%, 0, 0, 0] weight=50 delete_button>>>");

	edit_place["label_1"] << label_1;
	edit_place["label_2"] << label_2;

	edit_place["textbox_1"] << textbox_1;
	edit_place["textbox_2"] << textbox_2;

	if (data.data_to_edit.size() > 2) {
		label_3.caption(data.labels_array[2]);
		label_4.caption(data.labels_array[3]);

		int i = 0;
		for (auto& office : parsed_json) {
			combox.push_back(office["nazwa_stanowiska"]);
			if (office["nazwa_stanowiska"] == combox_value.value())
				combox.option(i);
			i++;
		}
		
		textbox_4.caption(data.data_to_edit[3]);

		edit_place["label_3"] << label_3;
		edit_place["label_4"] << label_4;

		edit_place["combox"] << combox;
		edit_place["textbox_4"] << textbox_4;
	}


	edit_place["confirm_button"] << confirm_button;
	edit_place["delete_button"] << delete_button;
	edit_place.collocate();

	edit_window.show();
	edit_window.modality();

	nana::exec();

	return;
}

void new_employee(nana::form* parent_window) {
	nana::form new_employee_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>()};
	new_employee_window.caption("Dodaj pracownika");
	nana::place new_employee_place{ new_employee_window };

	nana::label name_label{ new_employee_window, "Imie:" }, 
		surname_label{ new_employee_window, "Nazwisko:" }, 
		office_label{ new_employee_window, "Stanowisko:" }, 
		id_label{new_employee_window, "Identyfikator pracownika:"};
	name_label.typeface(nana::paint::font("", 10, true));
	surname_label.typeface(nana::paint::font("", 10, true));
	office_label.typeface(nana::paint::font("", 10, true));
	id_label.typeface(nana::paint::font("", 10, true));

	nana::textbox name_textbox{ new_employee_window },
		surname_textbox{ new_employee_window },
		id_textbox{ new_employee_window };
	name_textbox.multi_lines(false);
	surname_textbox.multi_lines(false);
	id_textbox.multi_lines(false);

	nana::combox office_combox{ new_employee_window };
	
	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/stanowiska"));
	json parsed_json = json::parse(response.text);
	for (auto& office : parsed_json) {
		office_combox.push_back(office["nazwa_stanowiska"]);
	}

	nana::button add_btn{ new_employee_window, nana::rectangle(nana::point(50, 50), nana::size(120, 50))};
	add_btn.events().click([&]() {
		json employee_data;
		employee_data["imie"] = name_textbox.text();
		employee_data["nazwisko"] = surname_textbox.text();
		employee_data["id_stanowiska"] = parsed_json[office_combox.option()]["id"];
		employee_data["identyfikator_firmowy_pracownika"] = id_textbox.text();

		std::string success_text[2] = { "OK", "Utworzono nowe konto" };
		std::string fail_text[2] = { "Błąd", "Błąd przy tworzeniu nowego konta" };

		post_request(employee_data, "http://localhost:8080/nowy-pracownik", success_text, fail_text, &new_employee_window);
	});
	add_btn.caption("Dodaj");

	new_employee_place.div("<margin=[15%, 25%, 40%, 25%] <vertical <<vertical <margin=[12%, 0, 0, 0] name_label><margin=[15%, 0, 0, 0] surname_label><margin=[15%, 0, 0, 0] office_label><margin=[8%, 0, 0, 0] id_label>><vertical <margin=[0, 0, 38%, 0] weight=33 name_textbox><margin=[15%, 0, 25%, 0] weight=33 surname_textbox><margin=[20%, 0, 20%, 0] weight=33 office_combobox><margin=[20%, 0, 20%, 0] weight=33 id_textbox>>><weight=40 add_btn>>>");
	 
	new_employee_place["name_label"] << name_label;
	new_employee_place["surname_label"] << surname_label;
	new_employee_place["office_label"] << office_label;
	new_employee_place["id_label"] << id_label;

	new_employee_place["name_textbox"] << name_textbox;
	new_employee_place["surname_textbox"] << surname_textbox;
	new_employee_place["office_combobox"] << office_combox;
	new_employee_place["id_textbox"] << id_textbox;

	new_employee_place["add_btn"] << add_btn;
	new_employee_place.collocate();

	new_employee_window.show();
	new_employee_window.modality();

	nana::exec();

	return;
}

void edit_employee(nana::form* parent_window) {
	nana::form edit_employee_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	edit_employee_window.caption("Edytuj pracownika");
	nana::place edit_employee_place{ edit_employee_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/pracownicy"));
	json parsed_json = json::parse(response.text);

	nana::listbox employees_listbox{ edit_employee_window };
	employees_listbox.append_header("Użytkownicy", 351);
	employees_listbox.sortable(false);

	nana::button show_button{ edit_employee_window };
	show_button.caption(nana::charset("Pokaż konto").to_bytes(nana::unicode::utf8));
	show_button.events().click([&]() {
		int i = 0;
		for (auto& item : employees_listbox.at(0)) {
			if (item.selected()) {
				std::string* str_ptr = employees_listbox.anyobj<std::string>(0, i);

				Data data = { {nana::charset("Imię:").to_bytes(nana::unicode::utf8), "Nazwisko:", "Stanowisko:", "Identyfikator pracownika:"}, {parsed_json[i]["imie"], parsed_json[i]["nazwisko"], parsed_json[i]["stanowiska"]["nazwa_stanowiska"], *str_ptr}, "pracownika" };

				create_edit_window(&edit_employee_window, data, parsed_json[i]["id"], parsed_json[i]["stanowiska"]["nazwa_stanowiska"].get<std::string>());
				break;
			}
			i++;
		}
	});

	int i = 0;
	for (auto& employee : parsed_json) {
		std::string str = employee["imie"].get<std::string>() + " " + employee["nazwisko"].get<std::string>();
		employees_listbox.at(0).append(str);
		employees_listbox.anyobj(0, i, employee["identyfikator_firmowy_pracownika"].get<std::string>());
		i++;
	}

	edit_employee_place.div("<margin=[10%, 20%, 30%, 20%] vertical <employees_listbox><margin=[40%, 0, 0, 0] weight=80 show_button>>");
	edit_employee_place["employees_listbox"] << employees_listbox;
	edit_employee_place["show_button"] << show_button;

	edit_employee_place.collocate();

	edit_employee_window.show();
	edit_employee_window.modality();

	nana::exec();

	return;
}

void new_account(nana::form* parent_window) {
	nana::form new_account_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	new_account_window.caption(nana::charset("Dodaj konto księgowego").to_bytes(nana::unicode::utf8));
	nana::place new_account_place{ new_account_window };

	nana::label username_label{ new_account_window, "Nazwa użytkownika:" }, password_label{ new_account_window, "Hasło:" };
	username_label.typeface(nana::paint::font("", 10, true));
	password_label.typeface(nana::paint::font("", 10, true));

	nana::textbox username_textbox{ new_account_window }, password_textbox{ new_account_window };
	nana::button add_btn{ new_account_window, nana::rectangle(nana::point(10, 200), nana::size(120, 60)) };

	username_textbox.multi_lines(false);
	password_textbox.multi_lines(false).mask('*');

	add_btn.caption("Dodaj");
	add_btn.events().click([&]() {
		json account_data;
		account_data["login"] = username_textbox.text();
		account_data["haslo"] = password_textbox.text();

		std::string success_text[2] = { "OK", "Utworzono nowe konto" };
		std::string fail_text[2] = { "Błąd", "Błąd przy tworzeniu nowego konta" };

		post_request(account_data, "http://localhost:8080/nowe-konto", success_text, fail_text, &new_account_window);
	});

	new_account_place.div("<margin=[35%, 25%, 20%, 25%] <vertical <weight=100 margin=[0, 0, 8%, 0] <vertical <username_label><password_label>><vertical <margin=[0, 0, 33%, 0] weight=33 username_textbox><margin=[33%, 0, 0, 0] weight=33 password_textbox>>><weight=40 <add_btn>>>>");
	
	new_account_place["username_label"] << username_label;
	new_account_place["password_label"] << password_label;
	new_account_place["username_textbox"] << username_textbox;
	new_account_place["password_textbox"] << password_textbox;

	new_account_place["add_btn"] << add_btn;

	new_account_place.collocate();

	new_account_window.show();
	new_account_window.modality();

	nana::exec();

	return;
}

void edit_account(nana::form* parent_window) {
	nana::form edit_account_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	edit_account_window.caption(nana::charset("Edytuj konto księgowego").to_bytes(nana::unicode::utf8));
	nana::place edit_account_place{ edit_account_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/konta"));
	json parsed_json = json::parse(response.text);

	nana::listbox users_listbox{ edit_account_window };
	users_listbox.append_header("Użytkownicy", 351);
	users_listbox.sortable(false);

	nana::button show_button{ edit_account_window };
	show_button.caption(nana::charset("Pokaż konto").to_bytes(nana::unicode::utf8));
	show_button.events().click([&]() {
		int i = 0;
		for (auto& item : users_listbox.at(0)) {
			if (item.selected()) {	
				std::string* str_ptr = users_listbox.anyobj<std::string>(0, i);
				
				Data data = { {"Login:", "Hasło:"}, { *str_ptr, parsed_json[i]["haslo"] }, "konto"};

				create_edit_window(&edit_account_window, data, parsed_json[i]["id_konta"]);
				break;
			}
			i++;
		}
	});

	int i = 0;
	for (auto& user : parsed_json) {
		std::string str = user["login"].get<std::string>();
		users_listbox.at(0).append(str);
		users_listbox.anyobj(0, i, str);
		i++;
	}

	edit_account_place.div("<margin=[10%, 20%, 30%, 20%] vertical <users_listbox><margin=[40%, 0, 0, 0] weight=80 show_button>>");
	edit_account_place["users_listbox"] << users_listbox;
	edit_account_place["show_button"] << show_button;

	edit_account_place.collocate();

	edit_account_window.show();
	edit_account_window.modality();

	nana::exec();

	return;
}

void new_office(nana::form* parent_window) {
	nana::form new_office_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	new_office_window.caption("Dodaj stanowisko");
	nana::place new_office_place{ new_office_window };

	nana::label office_name_label{ new_office_window, "Nazwa stanowiska:" }, base_salary_label{ new_office_window, "Pensja bazowa:" };
	office_name_label.typeface(nana::paint::font("", 10, true));
	base_salary_label.typeface(nana::paint::font("", 10, true));

	nana::textbox office_name_textbox{ new_office_window }, base_salary_textbox{ new_office_window };
	nana::button add_btn{ new_office_window, nana::rectangle(nana::point(10, 200), nana::size(120, 60)) };

	office_name_textbox.multi_lines(false);
	base_salary_textbox.multi_lines(false);

	add_btn.caption("Dodaj");
	add_btn.events().click([&]() {
		json office_data;
		
		std::string amount = base_salary_textbox.text();
		std::size_t found = amount.find(",");
		if (found != std::string::npos)
			amount.replace(found, 1, ".");

		office_data["nazwa_stanowiska"] = office_name_textbox.text();
		office_data["pensja_bazowa"] = amount;

		std::cout << base_salary_textbox.text();

		std::string success_text[2] = { "OK", "Dodano nowe stanowisko" };
		std::string fail_text[2] = { "Błąd", "Błąd przy dodawaniu nowego stanowiska" };

		post_request(office_data, "http://localhost:8080/nowe-stanowisko", success_text, fail_text, &new_office_window);
	});

	new_office_place.div("<margin=[35%, 25%, 20%, 25%] <vertical <weight=100 margin=[0, 0, 8%, 0] <vertical <office_name_label><base_salary_label>><vertical <margin=[0, 0, 33%, 0] weight=33 office_name_textbox><margin=[33%, 0, 0, 0] weight=33 base_salary_textbox>>><weight=40 <add_btn>>>>");

	new_office_place["office_name_label"] << office_name_label;
	new_office_place["base_salary_label"] << base_salary_label;
	new_office_place["office_name_textbox"] << office_name_textbox;
	new_office_place["base_salary_textbox"] << base_salary_textbox;

	new_office_place["add_btn"] << add_btn;

	new_office_place.collocate();

	new_office_window.show();
	new_office_window.modality();

	nana::exec();

	return;
}

void edit_office(nana::form* parent_window) {
	nana::form edit_office_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	edit_office_window.caption(nana::charset("Edytuj stanowisko").to_bytes(nana::unicode::utf8));
	nana::place edit_office_place{ edit_office_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/stanowiska"));
	json parsed_json = json::parse(response.text);

	nana::listbox offices_listbox{ edit_office_window };
	offices_listbox.append_header("Użytkownicy", 351);
	offices_listbox.sortable(false);

	nana::button show_button{ edit_office_window };
	show_button.caption(nana::charset("Pokaż konto").to_bytes(nana::unicode::utf8));
	show_button.events().click([&]() {
		int i = 0;
		for (auto& item : offices_listbox.at(0)) {
			if (item.selected()) {
				std::string* str_ptr = offices_listbox.anyobj<std::string>(0, i);
				Data data = { {"Nazwa stanowiska:", "Pensja:"}, { *str_ptr, parsed_json[i]["pensja_bazowa"].get<std::string>() }, "stanowisko"};

				create_edit_window(&edit_office_window, data, parsed_json[i]["id"]);
				break;
			}
			i++;
		}
	});

	int i = 0;
	for (auto& user : parsed_json) {
		std::string str = user["nazwa_stanowiska"].get<std::string>();
		offices_listbox.at(0).append(str);
		offices_listbox.anyobj(0, i, str);
		i++;
	}

	edit_office_place.div("<margin=[10%, 20%, 30%, 20%] vertical <offices_listbox><margin=[40%, 0, 0, 0] weight=80 show_button>>");
	edit_office_place["offices_listbox"] << offices_listbox;
	edit_office_place["show_button"] << show_button;

	edit_office_place.collocate();

	edit_office_window.show();
	edit_office_window.modality();

	nana::exec();

	return;
}