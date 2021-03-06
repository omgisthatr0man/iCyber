#include "Login.h"

int Login::GetCustomerLogin(string username, string password) {
	bool foundCustomer;
	int returnIndex;

	// initialize
	foundCustomer = false;
	returnIndex = 0;

	// get a copy of the customer vector for search
	vector<Customer> SearchCustomerVector = *customers;
	while (!SearchCustomerVector.empty() && !foundCustomer) {
		if (SearchCustomerVector.front().GetUsername() == username
				&& SearchCustomerVector.front().GetPassword() == password) {
			foundCustomer = true;
		} else {
			returnIndex++;
			SearchCustomerVector.erase(SearchCustomerVector.begin());
		}
	}
	if (!foundCustomer) {
		returnIndex = -1;
	}

	return returnIndex;
}

void Login::render_main(zr_window *window) {
	zr_context context;
	zr_begin(&context, window);
	{
		oldPasswordBoxLength = passwordBoxLength;

		if (state == 1) {
			if (adminLogin) {
				zr_header(&context, "Welcome Administrator", 0, 0,
						ZR_HEADER_LEFT);
				zr_layout_row_dynamic(&context, 30, 1);
				if (zr_button_text(&context, "Enter", ZR_BUTTON_DEFAULT)) {
					changeWindow(ADMIN);
				}
			} else {
				std::ostringstream customerStream;
				string output;
				output = "Welcome " + customers->at(*customer_index).GetName();
				zr_header(&context, output.c_str(), 0, 0, ZR_HEADER_LEFT);
				zr_layout_row_dynamic(&context, 30, 1);
				if (zr_button_text(&context, "Enter", ZR_BUTTON_DEFAULT)) {
					changeWindow(CUSTOMER_MENU);
				}
			}

		} else {
			zr_header(&context, "Login", 0, 0, ZR_HEADER_LEFT);
			zr_layout_row_dynamic(&context, 30, 1);
			if (state == 2) {
				zr_label(&context, "Error! Wrong username/password",
						ZR_TEXT_LEFT);
			}
			// username label row
			zr_layout_row_dynamic(&context, 30, 1);
			zr_label(&context, "Username:", ZR_TEXT_LEFT);

			// username box row
			zr_layout_row_static(&context, 30, 240, 3);
			zr_editbox(&context, &usernameBox);

			// password label row
			zr_layout_row_dynamic(&context, 30, 6);
			zr_label(&context, "Password:", ZR_TEXT_LEFT);

			// password box row
			zr_layout_row_static(&context, 30, 240, 8);
			zr_edit(&context, password_buffer, &passwordBoxLength, MAX_BUFFER, &passwordBoxState, &cursor, ZR_INPUT_DEFAULT);

			if (oldPasswordBoxLength < passwordBoxLength) {
				password += password_buffer[passwordBoxLength - 1];
				password_buffer[passwordBoxLength - 1] = '*';
			} else if (oldPasswordBoxLength > passwordBoxLength) {
				password.erase(password.size() - 1);
			}
			//zr_label(&context, password.c_str(), ZR_TEXT_LEFT);

			// submit box row
			zr_layout_row_dynamic(&context, 30, 9);
			if (zr_button_text(&context, "Submit", ZR_BUTTON_DEFAULT)) {
				username = "";
				// get the characters in the username box
				for (unsigned int i = 0; i < usernameBox.glyphes; i++) {
					username += ((char*) usernameBox.buffer.memory.ptr)[i];
				}

				if ((username == AdminClass().GetUsername())
						&& (password == AdminClass().GetPassword())) {
					state = 1;
					adminLogin = true;
				} else {
					*customer_index = GetCustomerLogin(username, password);
					issue_update();
					if (*customer_index != -1) {
						state = 1;
					} else {
						state = 2;
					}
				}
			}
			if (zr_button_text(&context, "Exit", ZR_BUTTON_DEFAULT)) {
				exit();
			}
			/*
			zr_layout_row_dynamic(&context, 30, 1);
			if (zr_button_text(&context, "DEBUG ONLY: Quick Admin Login",
					ZR_BUTTON_DEFAULT)) {
				changeWindow(ADMIN);
			}
			if (zr_button_text(&context, "DEBUG ONLY: Quick Customer Login",
					ZR_BUTTON_DEFAULT)) {
				changeWindow(CUSTOMER_MENU);
			}
			*/
		}

	}
	zr_end(&context, window);
}
