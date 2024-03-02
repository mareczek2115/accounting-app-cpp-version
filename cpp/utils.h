#pragma once

void post_request(nlohmann::json data, std::string url, std::string success_text[2], std::string fail_text[2], nana::form* parent_window);
void render_login_form();
void render_admin_panel();
void render_user_panel();