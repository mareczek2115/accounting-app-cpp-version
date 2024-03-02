#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <algorithm>


#include "user_funcs.h"
#include "utils.h"

using json = nlohmann::json;

#include <iostream>

void get_operations(nana::form* parent_window) {
	nana::form get_operations_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>()};
	get_operations_window.caption(nana::charset("Operacje finansowe").to_bytes(nana::unicode::utf8));
	nana::place get_operations_place{ get_operations_window };

	nana::listbox operations_listbox{ get_operations_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/operacje"));
	json parsed_json = json::parse(response.text);

	int max_length = 0;
	for (auto& operation : parsed_json) {
		std::string str = operation["wydzialy"]["nazwa_wydzialu"].get<std::string>() + " - " + operation["opis"].get<std::string>() + " - " + operation["kwota"].get<std::string>();
		if (str.length() > max_length)
			max_length = str.length();
	}

	if(max_length * 6.5 < 351)
		operations_listbox.append_header("Operacje", 351);
	else
		operations_listbox.append_header("Operacje", max_length * 6.5);

	for (auto& operation : parsed_json) {
		std::string str = operation["wydzialy"]["nazwa_wydzialu"].get<std::string>() + " - " + operation["opis"].get<std::string>() + " - " + operation["kwota"].get<std::string>();
		operations_listbox.at(0).append({ str });
	}

	get_operations_place.div("<margin=[10%, 20%, 40%, 20%] operations_listbox>");
	get_operations_place["operations_listbox"] << operations_listbox;

	get_operations_place.collocate();

	get_operations_window.show();
	get_operations_window.modality();

	nana::exec();

	return;
}

void get_employees(nana::form* parent_window) {
	nana::form get_employees_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	get_employees_window.caption(nana::charset("Pracownicy").to_bytes(nana::unicode::utf8));
	nana::place get_employees_place{ get_employees_window };

	nana::listbox employees_listbox{ get_employees_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/pracownicy"));
	json parsed_json = json::parse(response.text);

	int max_length = 0;
	for (auto& employee : parsed_json) {
		std::string str = employee["imie"].get<std::string>() + " " + employee["nazwisko"].get<std::string>() + " - " + employee["stanowiska"]["nazwa_stanowiska"].get<std::string>() + " - " + employee["identyfikator_firmowy_pracownika"].get<std::string>();
		if (str.length() > max_length)
			max_length = str.length();
	}

	if (max_length * 6.5 < 351) {
		employees_listbox.append_header("Pracownicy", 175);
		employees_listbox.append_header(nana::charset("Identyfikatory pracowników").to_bytes(nana::unicode::utf8), 175);
	}
	else {
		employees_listbox.append_header("Pracownicy", max_length * 3.25);
		employees_listbox.append_header(nana::charset("Identyfikatory pracowników").to_bytes(nana::unicode::utf8), max_length * 3.25);
	}
		

	for (auto& employee : parsed_json) {
		std::string str = employee["imie"].get<std::string>() + " " + employee["nazwisko"].get<std::string>() + " - " + employee["stanowiska"]["nazwa_stanowiska"].get<std::string>();
		std::string str_2 = employee["identyfikator_firmowy_pracownika"].get<std::string>();
		employees_listbox.at(0).append({ str, str_2 });
	}

	get_employees_place.div("<margin=[10%, 20%, 40%, 20%] employees_listbox>");
	get_employees_place["employees_listbox"] << employees_listbox;

	get_employees_place.collocate();

	get_employees_window.show();
	get_employees_window.modality();

	nana::exec();

	return;
}

void new_operation(nana::form* parent_window) {
	nana::form new_operation_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>()};
	new_operation_window.caption(nana::charset("Nowa operacja finansowa").to_bytes(nana::unicode::utf8));
	nana::place new_opeartion_place{ new_operation_window };

	nana::label faculty_label{new_operation_window, nana::charset("Dzia³ zlecaj¹cy:").to_bytes(nana::unicode::utf8)}, amount_label{new_operation_window, "Kwota:"}, description_label{new_operation_window, "Opis"};
	faculty_label.typeface(nana::paint::font("", 10, true));
	amount_label.typeface(nana::paint::font("", 10, true));
	description_label.typeface(nana::paint::font("", 10, true));

	nana::textbox amount_textbox{ new_operation_window }, description_textbox{ new_operation_window };
	amount_textbox.multi_lines(false);
	description_textbox.line_wrapped(true);

	nana::combox faculty_combox{ new_operation_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/wydzialy"));
	json parsed_json = json::parse(response.text);
	for (auto& faculty : parsed_json) {
		faculty_combox.push_back(faculty["nazwa_wydzialu"]);
	}

	nana::button confirm_btn{ new_operation_window };
	confirm_btn.caption(nana::charset("ZatwierdŸ").to_bytes(nana::unicode::utf8));
	confirm_btn.events().click([&]() {
		std::string amount = amount_textbox.text();
		std::size_t found = amount.find(",");
		if (found != std::string::npos)
			amount.replace(found, 1, ".");

		json operation_data;
		operation_data["id_wydzialu"] = parsed_json[faculty_combox.option()]["id_wydzialu"];
		operation_data["kwota"] = amount;
		operation_data["opis"] = description_textbox.text();

		std::string success_text[2] = { "OK", "Pomyœlnie dodano now¹ operacjê finansow¹" };
		std::string fail_text[2] = { "B³¹d", "Wyst¹pi³ b³¹d przy dodawaniu nowej operacji" };

		post_request(operation_data, "http://localhost:8080/nowa-operacja", success_text, fail_text, &new_operation_window);
	});
	
	new_opeartion_place.div("<margin=[12%, 22%, 15%, 22%] <vertical <vertical <<faculty_label><margin=[0, 0, 75%, 0] faculty_combox>><<amount_label><margin=[0, 0, 75%, 0] amount_textbox>><<description_label><margin=[0, 0, 10%, 0] description_textbox>>><weight=40 confirm_btn>>>");

	new_opeartion_place["faculty_label"] << faculty_label;
	new_opeartion_place["amount_label"] << amount_label;
	new_opeartion_place["description_label"] << description_label;
	new_opeartion_place["faculty_combox"] << faculty_combox;
	new_opeartion_place["amount_textbox"] << amount_textbox;
	new_opeartion_place["description_textbox"] << description_textbox;

	new_opeartion_place["confirm_btn"] << confirm_btn;

	new_opeartion_place.collocate();

	new_operation_window.show();
	new_operation_window.modality();

	nana::exec();
}

void get_faculties(nana::form* parent_window) {
	nana::form get_faculties_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	get_faculties_window.caption(nana::charset("Wydzia³y").to_bytes(nana::unicode::utf8));
	nana::place get_faculties_place{ get_faculties_window };

	nana::listbox faculties_listbox{ get_faculties_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/wydzialy"));
	json parsed_json = json::parse(response.text);

	int max_length = 0;
	for (auto& faculty : parsed_json) {
		std::string str = faculty["nazwa_wydzialu"].get<std::string>();
		if (str.length() > max_length)
			max_length = str.length();
	}

	if (max_length * 6.5 < 351)
		faculties_listbox.append_header("Wydzialy", 351);
	else
		faculties_listbox.append_header("Wydzialy", max_length * 6.5);

	for (auto& faculty : parsed_json) {
		std::string str = faculty["nazwa_wydzialu"].get<std::string>();
		faculties_listbox.at(0).append({ str });
	}

	get_faculties_place.div("<margin=[10%, 20%, 40%, 20%] faculties_listbox>");
	get_faculties_place["faculties_listbox"] << faculties_listbox;

	get_faculties_place.collocate();

	get_faculties_window.show();
	get_faculties_window.modality();

	nana::exec();

	return;
}

void pay_salary(nana::form* parent_window) {
	nana::form search_for_employee_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>()};
	search_for_employee_window.caption(nana::charset("Pensje pracowników").to_bytes(nana::unicode::utf8));
	nana::place pay_salary_place{ search_for_employee_window };

	nana::label id_label{ search_for_employee_window };
	id_label.caption("Wpisz identyfikator pracownika:");
	id_label.text_align(nana::align::center);
	id_label.typeface(nana::paint::font("", 10, true));

	nana::textbox id_textbox{ search_for_employee_window };
	id_textbox.multi_lines(false);

	nana::button search_button{ search_for_employee_window };
	search_button.caption("Szukaj");
	search_button.events().click([&]() {
		std::string id_text = id_textbox.text();
		if (id_text == "" || id_text.length() != 8 || id_text.find_first_not_of("0123456789") != std::string::npos) {
			id_textbox.caption("");
			id_textbox.tip_string("Wpisz poprawny identyfikator!");
		} else {
			std::string employee_id = id_textbox.text();
			employee_id.erase(remove(employee_id.begin(), employee_id.end(), ' '), employee_id.end());
			id_textbox.caption(employee_id);

			json search_data;
			search_data["employee_inner_id"] = id_textbox.text();

			cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:8080/wyplaty" }, cpr::Body{ search_data.dump() }, cpr::Header{ {"Content-Type", "application/json"} });

			json parsed_json = json::parse(response.text);

			if (parsed_json.size() == 0) {
				nana::msgbox msgbox{ *parent_window, "B³¹d", nana::msgbox::ok };
				msgbox.icon(nana::msgbox::icon_error);
				msgbox << nana::charset("Brak pracownika o podanym identyfikatorze!").to_bytes(nana::unicode::utf8);
				msgbox();
			}
			else {
				nana::form pay_salary_window{ search_for_employee_window, nana::rectangle(0, 0, 600, 400) };
				pay_salary_window.caption(nana::charset("Wyp³aæ pensjê").to_bytes(nana::unicode::utf8));
				nana::place pay_salary_place{ pay_salary_window };

				std::string name = parsed_json["imie"].get<std::string>() + " " + parsed_json["nazwisko"].get<std::string>();
				nana::label employee_label{ pay_salary_window }, amount_label{ pay_salary_window };
				employee_label.caption(name + " | " + parsed_json["stanowiska"]["nazwa_stanowiska"].get<std::string>() + "\nPensja bazowa: " + parsed_json["stanowiska"]["pensja_bazowa"].get<std::string>());
				employee_label.text_align(nana::align::center);
				employee_label.typeface(nana::paint::font("", 10, true));

				amount_label.caption("Kwota:");
				amount_label.typeface(nana::paint::font("", 10, true));

				nana::textbox salary_textbox{ pay_salary_window };
				salary_textbox.multi_lines(false);

				nana::button confirm_button{ pay_salary_window };
				confirm_button.caption(nana::charset("Wyp³aæ pensjê").to_bytes(nana::unicode::utf8));
				confirm_button.events().click([&]() {
					json salary_data;

				std::string amount = salary_textbox.text();
				std::size_t found = amount.find(",");
				if (found != std::string::npos)
					amount.replace(found, 1, ".");

				salary_data["id_pracownika"] = parsed_json["id"];
				salary_data["kwota"] = amount;

				std::string success_text[2] = { "OK", "Wyp³acono pensjê pomyœlnie" };
				std::string fail_text[2] = { "B³¹d", "B³¹d przy wyp³acaniu pensji" };

				post_request(salary_data, "http://localhost:8080/nowa-wyplata", success_text, fail_text, &pay_salary_window);
				id_textbox.caption("");

					});

				pay_salary_place.div("<margin=[25%, 25%, 40%, 25%] <vertical <employee_label><margin=[0, 25%, 40%, 25%] weight=30 salary_textbox><margin=[40%, 0, 0, 0] weight=60 confirm_button>>>");
				pay_salary_place["employee_label"] << employee_label;
				pay_salary_place["salary_textbox"] << salary_textbox;
				pay_salary_place["confirm_button"] << confirm_button;

				pay_salary_place.collocate();

				pay_salary_window.show();
				pay_salary_window.modality();

				nana::exec();
			}
		}
	});

	pay_salary_place.div("<margin=[25%, 25%, 35%, 25%] <vertical <id_label><margin=[0, 20%, 65%, 20%] id_textbox><weight=40 search_button>>>");
	pay_salary_place["id_label"] << id_label;
	pay_salary_place["id_textbox"] << id_textbox;
	pay_salary_place["search_button"] << search_button;

	pay_salary_place.collocate();

	search_for_employee_window.show();
	search_for_employee_window.modality();

	nana::exec();

	return;
}

void get_salaries(nana::form* parent_window) {
	nana::form get_salaries_window{ *parent_window, nana::rectangle(0, 0, 600, 400), nana::appear::decorate<>() };
	get_salaries_window.caption(nana::charset("Wyp³acone pensje").to_bytes(nana::unicode::utf8));
	nana::place get_salaries_place{ get_salaries_window };

	nana::listbox salaries_listbox{ get_salaries_window };

	cpr::Response response = cpr::Get(cpr::Url("http://localhost:8080/wyplaty"));
	json parsed_json = json::parse(response.text);

	int max_length = 0;
	for (auto& faculty : parsed_json) {
		std::string str = faculty["pracownicy"]["imie"].get<std::string>() + faculty["pracownicy"]["nazwisko"].get<std::string>();
		if (str.length() > max_length)
			max_length = str.length();
	}

	if (max_length * 2.16 < 117) {
		salaries_listbox.append_header("Pracownicy", 117);
	}
	else {
		salaries_listbox.append_header("Pracownicy", max_length * 2.16);
	}

	salaries_listbox.append_header("Data", 117);
	salaries_listbox.append_header("Kwota", 117);
		

	for (auto& faculty : parsed_json) {
		std::string str = faculty["pracownicy"]["imie"].get<std::string>() + " " + faculty["pracownicy"]["nazwisko"].get<std::string>();
		salaries_listbox.at(0).append({ str, faculty["data"].get<std::string>(), faculty["kwota"].get<std::string>() });
	}

	get_salaries_place.div("<margin=[10%, 20%, 40%, 20%] salaries_listbox>");
	get_salaries_place["salaries_listbox"] << salaries_listbox;

	get_salaries_place.collocate();

	get_salaries_window.show();
	get_salaries_window.modality();

	nana::exec();

	return;
}
