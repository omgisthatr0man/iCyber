#include "EditCustomer.h"
#include <sstream>

int EditCustomer::findCustomer(string searchName) {
	bool foundCustomer;
	int returnIndex;

	// initialize
	foundCustomer = false;
	returnIndex = 0;

	// get a copy of the customer vector for search
	vector<Customer> SearchCustomerVector = *customers;
	while (!SearchCustomerVector.empty() && !foundCustomer) {
		if (SearchCustomerVector.front().GetName() == searchName) {
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

void EditCustomer::render_main(zr_window *window) {
	zr_context context;
	zr_begin(&context, window);
	{
		static const char *ratings[] = { "very interested",
				"somewhat interested", "not interested", "never call again" };
		zr_header(&context, "Add Customer", 0, 0, ZR_HEADER_LEFT);
		zr_layout_row_dynamic(&context, 30, 1);
		zr_label(&context, "Note: Fields left empty will retain their current "
				"value", ZR_TEXT_LEFT);

		// name box
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "Name:", ZR_TEXT_LEFT);
		zr_editbox(&context, &nameBox);

		// street box
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "Street:", ZR_TEXT_LEFT);
		zr_editbox(&context, &streetBox);

		// state zipcode box
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "State and Zip Code:", ZR_TEXT_LEFT);
		zr_editbox(&context, &stateZipCodeBox);

		// username box
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "Username:", ZR_TEXT_LEFT);
		zr_editbox(&context, &usernameBox);
		if (isUsernameDuplicate) {
			zr_label(&context, "Could not change: Username already exists",
					ZR_TEXT_LEFT);
		} else if (isUsernameInvalid) {
			zr_label(&context, "Could not change: Spaces disallowed",
					ZR_TEXT_LEFT);
		}

		// password box
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "Password:", ZR_TEXT_LEFT);
		zr_editbox(&context, &passwordBox);
		if (isPasswordInvalid) {
			zr_label(&context, "Could not change: Spaces disallowed",
					ZR_TEXT_LEFT);
		}

		// product spinners
		stringStream.str("");
		stringStream.clear();
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "Minimal Packages:", ZR_TEXT_LEFT);
		zr_spinner_int(&context, 0, &product1Value, 9999, 1, &product1State);

		stringStream.str("");
		stringStream.clear();
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "Extreme Packages:", ZR_TEXT_LEFT);
		zr_spinner_int(&context, 0, &product2Value, 9999, 1, &product2State);

		stringStream.str("");
		stringStream.clear();
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "Ultimate Packages:", ZR_TEXT_LEFT);
		zr_spinner_int(&context, 0, &product3Value, 9999, 1, &product3State);

		// Rating drop down menu
		zr_layout_row_dynamic(&context, 30, 4);
		zr_label(&context, "Rating: ", ZR_TEXT_LEFT);
		zr_combo_begin(&context, &ratingComboBox, ratings[currentRatingInt],
				&ratingComboBoxState);
		{
			zr_layout_row_dynamic(&ratingComboBox, 25, 1);
			for (int i = 0; i < 4; i++) {
				if (zr_combo_item(&ratingComboBox, ratings[i], ZR_TEXT_LEFT)) {
					currentRatingInt = i;
				}
			}
		}
		zr_combo_end(&context, &ratingComboBox, &ratingComboBoxState);

		// Received Pamphlet and Key Customer Box checkboxes
		zr_layout_row_dynamic(&context, 30, 4);
		zr_checkbox(&context, "Requested Pamphlet", &isPamphletBoxUnTicked);

		zr_checkbox(&context, "Key Customer", &isKeyBoxUnTicked);

		// submit box row
		zr_layout_row_dynamic(&context, 30, 1);
		if (zr_button_text(&context, "Submit", ZR_BUTTON_DEFAULT)) {
			// initialize
			isCustomerChanged = false;
			isNameDuplicate = false;
			isInvalidCustomer = false;
			isUsernameDuplicate = false;
			isUsernameInvalid = false;
			isPasswordInvalid = false;
			name = "";
			street = "";
			searchIndex = 0;
			stateZipCode = "";
			username = "";
			password = "";

			// get the characters in the name box
			for (unsigned int i = 0; i < nameBox.glyphes; i++) {
				name += ((char*) nameBox.buffer.memory.ptr)[i];
			}
			// check if name empty
			if (name.length() == 0) {
				// if so make name the same as current value
				name = customers->at(*customer_index).GetName();
			} else {
				// else search for duplicates
				// trim spaces
				name = trimSpaces(name);
				SearchCustomerVector = *customers;
				searchIndex = 0;
				while (!SearchCustomerVector.empty() && !isNameDuplicate) {
					if (SearchCustomerVector.front().GetName() == name && searchIndex != *customer_index) {
						isNameDuplicate = true;
						isInvalidCustomer = true;
					} else {
						searchIndex++;
						SearchCustomerVector.erase(
								SearchCustomerVector.begin());
					}
				}
			}

			// get the characters in the street box
			for (unsigned int i = 0; i < streetBox.glyphes; i++) {
				street += ((char*) streetBox.buffer.memory.ptr)[i];
			}
			// check if street empty
			if (street.length() == 0) {
				// if so make street equal to current value
				street = customers->at(*customer_index).GetStreet();
			} else {
				// trim spaces
				street = trimSpaces(street);
			}

			// get the characters in the state zip code box
			for (unsigned int i = 0; i < stateZipCodeBox.glyphes; i++) {
				stateZipCode += ((char*) stateZipCodeBox.buffer.memory.ptr)[i];
			}
			// check if state zip code empty
			if (stateZipCode.length() == 0) {
				// if so make street equal to current value
				stateZipCode = customers->at(*customer_index).GetStateZipCode();
			} else {
				// trim spaces
				stateZipCode = trimSpaces(stateZipCode);
			}

			// get the characters in the username box
			for (unsigned int i = 0; i < usernameBox.glyphes; i++) {
				username += ((char*) usernameBox.buffer.memory.ptr)[i];
			}
			// check if username empty, if so generate a default username
			if (username.length() == 0) {
				// if so make username equal to current value
				username = customers->at(*customer_index).GetUsername();
				// otherwise search for spaces
			} else if (hasSpaces(username)) {
				// if so username is invalid
				isUsernameInvalid = true;
				isInvalidCustomer = true;
			} else {
				// otherwise search for duplicates
				SearchCustomerVector = *customers;
				searchIndex = 0;
				while (!SearchCustomerVector.empty() && !isUsernameDuplicate
						&& searchIndex != *customer_index) {
					if (SearchCustomerVector.front().GetUsername()
							== username) {
						isUsernameDuplicate = true;
						isInvalidCustomer = true;
					} else {
						searchIndex++;
						SearchCustomerVector.erase(
								SearchCustomerVector.begin());
					}
				}
			}

			// get the characters in the password box
			for (unsigned int i = 0; i < passwordBox.glyphes; i++) {
				password += ((char*) passwordBox.buffer.memory.ptr)[i];
			}
			// check if password empty
			if (password.length() == 0) {
				// if so make username equal to current value
				password = customers->at(*customer_index).GetPassword();
			} else if (hasSpaces(password)) {
				// otherwise check for spaces
				isPasswordInvalid = true;
				isInvalidCustomer = true;
			}

			// get whether key
			if (!isKeyBoxUnTicked) {
				isKey = true;
			} else {
				isKey = false;
			}

			// get pamphlet
			if (!isPamphletBoxUnTicked) {
				isPamphletReceived = true;
			} else {
				isPamphletReceived = false;
			}
			// get rating
			rating = ratings[currentRatingInt];

			if (!isInvalidCustomer) {
				customers->at(*customer_index).SetValues(name, street, stateZipCode,
						isPamphletReceived, isKey, rating, username, password,
						product1Value, product2Value, product3Value);

				std::sort(customers->begin(), customers->end());
				*customer_index = findCustomer(name);
				SaveCustomers(customers);
				issue_update();
				isCustomerChanged = true;
			}

		}
		zr_layout_row_dynamic(&context, 30, 1);
		if (zr_button_text(&context, "Customer List", ZR_BUTTON_DEFAULT)) {
			changeWindow(CUSTOMER_LIST);
		}
		if (zr_button_text(&context, "Main Menu", ZR_BUTTON_DEFAULT)) {
			changeWindow(ADMIN);
		}
		if (isCustomerChanged) {
			output = "You have changed customer "
					+ customers->at(*customer_index).GetName()
					+ "'s information";
			zr_layout_row_dynamic(&context, 30, 1);
			zr_label(&context, output.c_str(), ZR_TEXT_LEFT);
		}
	}
	zr_end(&context, window);
}

